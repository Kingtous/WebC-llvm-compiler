//
// Created by kingtous on 2021/5/17.
//

#include "NetworkReader.h"
#include "Global.h"
#include "ErrHelper.h"

#include <utility>
#include <boost/asio/post.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/regex.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>

namespace http = boost::beast::http;

#define NETWORKREADER_RETRY_TIMES 3

namespace network {
    enum download_status {
        INIT = 0,
        IN_DOWNLOAD = 1,
        DOWNLOADED = 2,
        FAILED = 3
    };
    boost::regex url_regex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
}

int NetworkReader::retry_times = NETWORKREADER_RETRY_TIMES;

int NetworkReader::getchar() {
    if (isContentReady() && string_reader_ptr != nullptr) {
        return string_reader_ptr->getchar();
    }
    return END_OF_FILE;
}

int NetworkReader::seek() {
    if (isContentReady() && string_reader_ptr != nullptr) {
        return string_reader_ptr->seek();
    }
    return END_OF_FILE;
}

unsigned int NetworkReader::getLineNo() {
    if (isContentReady() && string_reader_ptr != nullptr) {
        return string_reader_ptr->getLineNo();
    }
    return END_OF_FILE;
}

unsigned int NetworkReader::getCCol() {
    if (isContentReady() && string_reader_ptr != nullptr) {
        return string_reader_ptr->getCCol();
    }
    return END_OF_FILE;
}

NetworkReader::NetworkReader(std::string url) : url(std::move(url)) {
    // init
    download_status = network::download_status::INIT;
    download();
}

int NetworkReader::download() {
    boost::asio::post(thread_pool, [this]() {
        download_status = network::download_status::IN_DOWNLOAD;
        LogInfo("正在下载代码数据\n");
        try {
            boost::cmatch url_parsed;
            if (boost::regex_match(url.c_str(), url_parsed, network::url_regex)) {
                boost::asio::io_context ioc;
                boost::asio::ip::tcp::resolver tcp_resolver(ioc);
                auto protocol = string(url_parsed[1].first, url_parsed[1].second);
                auto base_url = string(url_parsed[2].first, url_parsed[2].second);
                auto port = string(url_parsed[3].first, url_parsed[3].second);
                auto path = string(url_parsed[4].first, url_parsed[4].second);
                auto query = string(url_parsed[5].first, url_parsed[5].second);
                // preprocess
                bool isSSL = protocol == "https";
                if (port.empty()) {
                    port = isSSL ? "443" : "80";
                }
                // connect
                auto result = tcp_resolver.resolve(base_url, port);
                if (result.empty()) {
                    LogError("无法解析指定地址");
                    download_status = network::download_status::FAILED;
                    return;
                }

                if (!isSSL) {
                    auto tcp_socket = boost::asio::ip::tcp::socket(ioc);
                    boost::asio::connect(tcp_socket, result.begin(), result.end());
                    if (!tcp_socket.is_open()) {
                        LogError("无法连接指定地址");
                        download_status = network::download_status::FAILED;
                        return;
                    }
                    // start download
                    http::request <http::string_body> request{http::verb::get, path + "?" + query, 11};
                    request.set(http::field::host, base_url);
                    request.set(http::field::user_agent, APPNAME);
                    http::write(tcp_socket, request);
                    // 开启一个buffer
                    boost::beast::flat_buffer buffer;
                    // 开启一个response body
                    http::response <http::string_body> res;
                    // Receive the HTTP response
                    http::read(tcp_socket, buffer, res);
                    string_reader_ptr =
                            std::make_unique<StringReader>(new string(res.body()));
                    download_status = network::download_status::DOWNLOADED;
                    tcp_socket.close();
                } else {
                    // ssl support
                    boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::method::sslv23_client);
                    boost::asio::ssl::stream <boost::asio::ip::tcp::socket> ssl_socket(ioc, ssl_ctx);
                    boost::asio::connect(ssl_socket.lowest_layer(), result.begin(), result.end());
                    if (!ssl_socket.lowest_layer().is_open()) {
                        LogError("无法连接指定地址(SSL)");
                        download_status = network::download_status::FAILED;
                        return;
                    }
                    // handshake
                    ssl_socket.handshake(boost::asio::ssl::stream_base::client);
                    // start download
                    http::request <http::string_body> request{http::verb::get, path + "?" + query, 11};
                    request.set(http::field::host, base_url);
                    request.set(http::field::user_agent, APPNAME);
                    http::write(ssl_socket, request);
                    // 开启一个buffer
                    boost::beast::flat_buffer buffer;
                    // 开启一个response body
                    http::response <http::string_body> res;
                    // Receive the HTTP response
                    http::read(ssl_socket, buffer, res);
                    string_reader_ptr =
                            std::make_unique<StringReader>(new string(res.body()));
                    download_status = network::download_status::DOWNLOADED;
                    ssl_socket.lowest_layer().close();
                }

            } else {
                LogError("不是个有效的URL地址");
                download_status = network::download_status::FAILED;
            }
        } catch (boost::system::system_error &e) {
            LogError(e.what());
            download_status = network::download_status::FAILED;
        }
    });
    return ROK;
}

bool NetworkReader::isContentReady() {
    // download in progress, block
    while (download_status == network::download_status::IN_DOWNLOAD);
    return download_status == network::download_status::DOWNLOADED
           && string_reader_ptr != nullptr;
}
