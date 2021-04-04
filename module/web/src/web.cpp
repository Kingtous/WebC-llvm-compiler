//
// Created by 金韬 on 2021/2/26.
//

#include "web.hpp"

#include <utility>

boost::asio::io_context *_web_io_context = nullptr;
boost::asio::io_context *_server_context = nullptr;
tcp::resolver *_web_resolver = nullptr;
map<int, tcp::socket *> _web_tcp_socket_map;
map<int, pair<_web_HttpWorker *, pair<int, std::vector<std::thread *> *>>> _web_http_server_map;

// 默认证书，目前在Safari上会出现stream truncated，在chrome里需要在@link chrome://flags启用本地证书信任
std::string _web_cert = "-----BEGIN CERTIFICATE-----\n"
                        "MIIDhTCCAm0CFB6C9HfrypjV3phwViMQr9kX8tm+MA0GCSqGSIb3DQEBCwUAMH8x\n"
                        "CzAJBgNVBAYTAkNOMQswCQYDVQQIDAJDTjELMAkGA1UEBwwCQ04xETAPBgNVBAoM\n"
                        "CEtpbmd0b3VzMREwDwYDVQQLDAhLaW5ndG91czERMA8GA1UEAwwIS2luZ3RvdXMx\n"
                        "HTAbBgkqhkiG9w0BCQEWDm1lQGtpbmd0b3VzLmNuMB4XDTIxMDMyODE1MjM0NVoX\n"
                        "DTIyMDMyODE1MjM0NVowfzELMAkGA1UEBhMCQ04xCzAJBgNVBAgMAkNOMQswCQYD\n"
                        "VQQHDAJDTjERMA8GA1UECgwIS2luZ3RvdXMxETAPBgNVBAsMCEtpbmd0b3VzMREw\n"
                        "DwYDVQQDDAhLaW5ndG91czEdMBsGCSqGSIb3DQEJARYObWVAa2luZ3RvdXMuY24w\n"
                        "ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDFWLlHETMNWfEisX5+5pRF\n"
                        "tCNzi5wetRzuhnR7lR+uN98FgDtFE5oWQzJ6ocLJ7HJbS5/hpLDMMcC67fvbT/9Q\n"
                        "2T5GrGa/4C+ewa/rzYf3K0Q1y3ioY8NPD03IXHqzgjqqp+IetnjWK9XQMhSTPkVg\n"
                        "d3BOMkFduQGHvfKyGGGXc+uL7mMEBPgKAC6PYFgmGTkZ9ZLbScHZvxvH5R/feOJ2\n"
                        "Y1pXb2Stfrxhiuy9n8m+6gzA9FIM6eQIooOJdF9IqYNbfGZ8Obto47KI3KI6PpfV\n"
                        "z9x4mk7DSveVrDnkunnOWkpIQUtlxT7y6oWUjFqwyTbCATCtpPyF8iY4kwj+jOjP\n"
                        "AgMBAAEwDQYJKoZIhvcNAQELBQADggEBAFgcoMlXsJqaoH9lAy+5vozKuXESU309\n"
                        "CoUTf/shp4SdOrfTMS6R7zmy7E06uHURNUiOHgpMScgojQmGUkBH2myDUK4fzALP\n"
                        "Bxuq6P/ntqj/4G/oGQ45cILzyK48rtzZNXiQxRcjwwOOYilriepVVpSVqwyIBAbA\n"
                        "vzSi1M22u7sxKf5e/33mkAD8kyAiPBfNQqN3LVNbS5Yz18ZhYc8d4Y+xHIvQJCkI\n"
                        "qbcd2NyX3sv9OCkTpjmqBvdfzkH+a/Qnb+evemwNBPYcYJkn0CkWXyETPg9KgRfb\n"
                        "noJsD01zYWXTP60gu8IGgeoCix4sYjdPjBInylLBAyKQEBzBxyE0LG0=\n"
                        "-----END CERTIFICATE-----";
std::string _web_default_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                               "MIIEowIBAAKCAQEAxVi5RxEzDVnxIrF+fuaURbQjc4ucHrUc7oZ0e5UfrjffBYA7\n"
                               "RROaFkMyeqHCyexyW0uf4aSwzDHAuu3720//UNk+Rqxmv+AvnsGv682H9ytENct4\n"
                               "qGPDTw9NyFx6s4I6qqfiHrZ41ivV0DIUkz5FYHdwTjJBXbkBh73yshhhl3Pri+5j\n"
                               "BAT4CgAuj2BYJhk5GfWS20nB2b8bx+Uf33jidmNaV29krX68YYrsvZ/JvuoMwPRS\n"
                               "DOnkCKKDiXRfSKmDW3xmfDm7aOOyiNyiOj6X1c/ceJpOw0r3law55Lp5zlpKSEFL\n"
                               "ZcU+8uqFlIxasMk2wgEwraT8hfImOJMI/ozozwIDAQABAoIBAQCq1GnWHgpeZVw6\n"
                               "lwEUIs95Iz2I+PJVFpyL0fIlRGpfKlgxb9ZermA1I0ggJcTzfG1loCovbpvotqzK\n"
                               "Y9Yf+7jcqKGjFfv/3rX6LFn3ujwXdumd05uH0Iwx9OJ1LbC9kV7teVRhF720T8/P\n"
                               "YHq1FzOBFQMvfYtd8RdlBMWA62AXdCgubezHd7xcoVagZkosEZe+bTTVS9IOwu04\n"
                               "tSxDKhLL2mOwxZJl31zvKDSX1ifdec2YEIPLKA3wEbUkWpXbgiHd2cTAPELrYWqV\n"
                               "t0ZABHdhUdvTA4FjCux+P+TZeT1KeGBrsjCOxgdNkPGq2KSKql4vwagvhtBlSlww\n"
                               "ZTp763IhAoGBAO802GvWi4C98lJRB5dvnKpNn+GSl7Mv273moBegoSQQOA9MIdvx\n"
                               "5Vbo20CH+WrzLBivrgjpyniOzsgSx+jNz0ZpwkrnMgKsd/OEtfRJPTdzINUuL3ws\n"
                               "D5RHL56H2VMae5FMUEGloGjOAHPYUZ0L5egohy02uTYY0xcTvZpiCTTRAoGBANMz\n"
                               "jJgmsJilwoy2JBtq3wo70kXqS4tv0jEvYAowWNVtRV+B+f9cTN+1dvuksfks7Nsz\n"
                               "Fh6hdebkkosckZ8R0tOpoczyZzumhSSOcnHkVwpcFK+bwQhXahqJbp4b+bfqvUIZ\n"
                               "85vsx3Vf/JZudrcKdxoVCzRKChhYnO3pBdj1FiufAoGASWDkovAFi1C5bRb+RS4v\n"
                               "2O5HBWN6tbUQq4h7w8O6qt1u1QJ9wILL34U+/OS5Vl2SQWBi+sj9G05zkFxyhq/b\n"
                               "zbXTrDaP2NIfWcx0JKPy63edssegOW8quEm+CduShzOwhDnsxLaSJpRK/z50isCD\n"
                               "nlwQW4ZV+opS9tlPaHR6boECgYBzwBbX9KxonMBJejHDzQi8hRizNLt+iiR6zGoo\n"
                               "a2LkZeTVeVjYeW+wU7YIKLVZySNu50rCMt95UeB22Yt90LjpRFjnCBtH9/WUmXVj\n"
                               "si96AAleTVC4D6CZNqUnhlGQWNis96gp1tViIN9AWv45R0ULSGlBmSp7OWWYhlOs\n"
                               "GnayJQKBgEiedhmcGk8dfVy2In+dMT8nQ4sA3R5/XaDC2HWQgqiFXIFenWLsL3Ka\n"
                               "GQp5CsSr/rVPNRofd/ms316rk0mrBAowoTC9SCc8SdMcufwuhh4jZEol1uXquPLY\n"
                               "C9++b8Bfnms5ygOnEamt2tVp1iyCT80t+Lv+PgypSY4CXDR1e33U\n"
                               "-----END RSA PRIVATE KEY-----";
auto _web_cert_buf = boost::asio::const_buffer(_web_cert.c_str(), _web_cert.size());
auto _web_key_buf = boost::asio::const_buffer(_web_default_key.c_str(), _web_default_key.size());
/**
 * HTTP 1.1
 */
auto _web_http_version = 11;
auto _web_agent = "kingtous_web_agent";

/**
 * 递增的SocketId
 */
volatile int socketId = 0;

/**
 * 递增的ServerId
 */
volatile int serverId = 0;

class _web_Session {
public:
    _web_Session(boost::asio::ip::tcp::socket &&socket,
                 boost::asio::ssl::context &context, std::shared_ptr<std::vector<WebHttpHandler>> handlers)
            : socket_(std::move(socket), context), handlers(std::move(handlers)) {
    }

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type &socket() {
        return socket_.lowest_layer();
    }

    void start() {
        socket_.async_handshake(boost::asio::ssl::stream_base::server,
                                [this](boost::system::error_code ec) {
                                    if (ec) {
                                        fprintf(stderr, "%s\n", ec.message().c_str());
                                    } else {
                                        handle_handshake(ec);
                                    }

                                });
    }

    void handle_handshake(const boost::system::error_code &error) {
        if (!error) {
            // 握手完毕，读取数据进buffer，最多8KB请求
            parser.emplace();
            http::async_read(
                    socket_,
                    buffer,
                    *parser,
                    [this](beast::error_code ec, std::size_t) {
                        if (ec) {
                            std::fprintf(stderr, "%s", ec.message().c_str());
                            close();
                        } else {
                            process_request(parser->get());
                        }
                    }
            );
        } else {
            close();
        }
    }

    void process_request(http::request<request_body_t> const &request) {
        auto target = request.target();
        auto handlers_it = this->handlers->begin();
        while (handlers_it != this->handlers->end()) {
            if ((*(*handlers_it).path) == target && (*(*handlers_it).method) == request.method_string()) {
                auto func = *(handlers_it->function);
                auto resp = func();
//            std::string resp_str(resp);
                str_resp = std::make_unique<http::response<http::string_body>>();
                str_resp->result(http::status::ok);
                str_resp->keep_alive(false);
                str_resp->set(http::field::server, SERVER_NAME);
                str_resp->set(http::field::content_type, *(handlers_it->content_type));
                str_resp->body() = resp;
                // 计算相应长度
                str_resp->prepare_payload();
                str_serializer = std::make_unique<http::response_serializer<http::string_body>>(*str_resp);
                http::async_write(
                        socket_,
                        *str_serializer,
                        [this](beast::error_code ec, std::size_t) {
                            if (ec) {
                                std::fprintf(stderr, "%s", ec.message().c_str());
                            }
                            close();
                        }
                );
                return;
            }
            handlers_it++;
        }
        sendNotExistResponse();
    }

    void sendNotExistResponse() {
        str_resp = std::make_unique<http::response<http::string_body>>();
        str_resp->result(http::status::not_found);
        str_resp->keep_alive(false);
        str_resp->set(http::field::server, SERVER_NAME);
        str_resp->set(http::field::content_type, "application/json");
        str_resp->body() = "您可能访问了一个错误的地址 | URL requested not mapped.";
        // 计算相应长度
        str_resp->prepare_payload();
        str_serializer = std::make_unique<http::response_serializer<http::string_body>>(*str_resp);
        http::async_write(
                socket_,
                *str_serializer,
                [this](beast::error_code ec, std::size_t) {
                    close();
                }
        );
    }

    void close() {
        socket_.async_shutdown([this](boost::system::error_code ec) {
            // ignore
            delete this;
        });
    }

private:
    ssl_stream socket_;
    beast::flat_static_buffer<8192> buffer;
    boost::optional<http::request_parser<http::string_body>> parser;
    std::unique_ptr<http::response_serializer<http::string_body>> str_serializer;
    std::shared_ptr<std::vector<WebHttpHandler>> handlers;
    std::unique_ptr<http::response<http::string_body>> str_resp;
};


int _web_getSocket() {
    _web_init();
    if (_web_io_context == nullptr || _web_resolver == nullptr) {
        return NOT_INITIALIZED;
    }
    auto tcp_socket = new tcp::socket(*_web_io_context);
    _web_tcp_socket_map.insert(make_pair(socketId, tcp_socket));
    return socketId++;
}

int _web_init() {
    if (_web_io_context == nullptr) {
        _web_io_context = new boost::asio::io_context();
        _web_resolver = new tcp::resolver(*_web_io_context);
    }
    return ROK;
}

int _web_connectSocket(int sId, const char *baseUrlOrIp, const char *port) {
    _web_init();
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return SOCKET_NOT_EXISTS;
    }
    auto socket = socketIt->second;
    auto results = _web_resolver->resolve(baseUrlOrIp, port);
    if (results.empty()) {
        return RESOLVE_FAILED;
    }
    // 连接
    boost::asio::connect(*socket, results.begin(), results.end());
    return socket->is_open() ? ROK : CONNECT_FAILED;
}

int _web_closeSocket(int sId) {
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return SOCKET_NOT_EXISTS;
    }
    boost::system::error_code ec;
    socketIt->second->shutdown(tcp::socket::shutdown_both, ec);
    return ROK;
}

int _web_isSocketConnected(int sId) {
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return SOCKET_NOT_EXISTS;
    }
    return socketIt->second->is_open() ? ROK : NOT_CONNECTED;
}

const char *_web_callGetRequest(int sId, char *host, char *path) {
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return nullptr;
    }
    http::request<http::string_body> request{http::verb::get, path, _web_http_version};
    request.set(http::field::host, host);
    request.set(http::field::user_agent, _web_agent);
    http::write(*socketIt->second, request);
    // 开启一个buffer
    boost::beast::flat_buffer buffer;
    // 开启一个response body
    http::response<http::string_body> res;
    // Receive the HTTP response
    http::read(*socketIt->second, buffer, res);
    auto string = res.body().c_str();
    char *str = new char[res.body().size()]{'\0'};
    memcpy(str, string, res.body().size());
    return str;
}

const char *_web_callPostRequest(int socketId, char *host, char *path, char *body) {
    auto socketIt = _web_tcp_socket_map.find(socketId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return nullptr;
    }
    int length = strlen(body);
    std::stringstream ss;
    ss << "Content-Length: " << length << "\r\n";
    ss << "Content-Type:application/x-www-form-urlencoded\r\n";
    ss << body;
    http::request<http::string_body> request{http::verb::post, path, _web_http_version, ss.str()};
    request.set(http::field::host, host);
    request.set(http::field::user_agent, _web_agent);
//    http::write_header
    http::write(*socketIt->second, request);
    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(*socketIt->second, buffer, res);
    auto string = res.body().c_str();
    char *str = new char[res.body().size()]{'\0'};
    memcpy(str, string, res.body().size());
    return str;
}
/// 服务器部分

namespace asio = boost::asio;

int _web_getServerId(const char *addr, int port, int core) {
    if (_server_context == nullptr) {
        _server_context = new boost::asio::io_context(core);
    }
    auto address = boost::asio::ip::make_address(addr);
    auto acceptor = new tcp::acceptor{*_server_context, {address, static_cast<unsigned short>(port)}};
    char *path = new char[BUFSIZ];
    getcwd(path, BUFSIZ);
    auto threads = new std::vector<std::thread *>;
    auto server = new _web_HttpWorker(_server_context, *acceptor, std::string(path));
    server->start();
    auto work_pair = make_pair(server, make_pair(core, threads));
    auto pair = make_pair(serverId++, work_pair);
    _web_http_server_map.insert(pair);
    return 0;
}

int
_web_addUrlHandler(int sId, const char *method, const char *path, const char *content_type, const char *(*handler)()) {
    auto it = _web_http_server_map.find(sId);
    if (it == _web_http_server_map.end()) {
        return SERVER_NOT_EXISTS;
    }
    auto http_handler = new WebHttpHandler();
    http_handler->method = new std::string(method);
    http_handler->path = new std::string(path);
    http_handler->content_type = new std::string(content_type);
    http_handler->function = handler;
    auto worker = it->second.first;
    worker->addHandler(*http_handler);
    return ROK;
}

int _web_startServe(int sId) {
    auto it = _web_http_server_map.find(sId);
    if (it == _web_http_server_map.end()) {
        return SERVER_NOT_EXISTS;
    }
    // 开启
    auto cores = it->second.second.first - 1;
    while (cores--) {
        auto t = new std::thread([] {
            _server_context->run();
        });
        it->second.second.second->push_back(t);
    }
    _server_context->run();
    return ROK;
}

_web_HttpWorker::_web_HttpWorker(boost::asio::io_context *context, tcp::acceptor &acceptor, const string &basePath) :
        acceptor(acceptor), base_path(basePath), io_context(context) {
    beast::error_code ec;
    acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    ssl_context.set_verify_mode(ssl::verify_client_once);
    ssl_context.use_certificate_chain(_web_cert_buf);
    ssl_context.use_private_key(_web_key_buf, boost::asio::ssl::context::pem);
    ssl_context.set_password_callback([](std::size_t, ssl::context_base::password_purpose) {
        return "kingtous";
    });
    ssl_context.set_options(
            ssl::context::default_workarounds
            | ssl::context::no_sslv2
    );
    handlers = make_shared<std::vector<WebHttpHandler>>();
}

void _web_HttpWorker::addHandler(WebHttpHandler &handler) {
    this->handlers->push_back(handler);
}

void _web_HttpWorker::process_request(http::request<request_body_t> const &request) {
    // 迁移至_web_Session
}

void _web_HttpWorker::accept() {
    acceptor.async_accept(
            asio::make_strand(*io_context),
            [this](beast::error_code ec, tcp::socket socket) {
                if (ec) {
                    std::fprintf(stderr, "%s\n", ec.message().c_str());
                } else {
                    // 创建
                    auto session = new _web_Session(std::move(socket), ssl_context, handlers);
                    session->start();
                }
                accept();
            }
    );
}

void _web_HttpWorker::readRequest() {
    parser.emplace();
    http::async_read(
            *ssl_stream_,
            buffer,
            *parser,
            [this](beast::error_code ec, std::size_t) {
                if (ec) {
                    std::fprintf(stderr, "%s", ec.message().c_str());
                } else {
                    process_request(parser->get());
                }
            }
    );
}

void _web_HttpWorker::start() {
    accept();
    checkDeadline();
}

void _web_HttpWorker::checkDeadline() {
//    if (request_deadline.expiry() <= std::chrono::steady_clock::now()) {
//        doClose();
//        accept();
//        request_deadline.expires_at(std::chrono::steady_clock::time_point::max());
//    }
//
//    request_deadline.async_wait([this](beast::error_code) {
//        checkDeadline();
//    });
}

void _web_HttpWorker::sendNotExistResponse() {
    str_resp = std::make_unique<http::response<http::string_body>>();
    str_resp->result(http::status::not_found);
    str_resp->keep_alive(false);
    str_resp->set(http::field::server, SERVER_NAME);
    str_resp->set(http::field::content_type, "application/json");
    str_resp->body() = "您可能访问了一个错误的地址 | URL requested not mapped.";
    // 计算相应长度
    str_resp->prepare_payload();
    str_serializer = std::make_unique<http::response_serializer<http::string_body>>(*str_resp);
    http::async_write(
            *ssl_stream_,
            *str_serializer,
            [this](beast::error_code ec, std::size_t) {
                // ignore
            }
    );
}

void _web_HttpWorker::doClose() {
    beast::get_lowest_layer(ssl_stream_)->shutdown();
    ssl_stream_->async_shutdown([](beast::error_code ec) {
        // ignore
        if (ec) {
            fprintf(stderr, "%s", ec.message().c_str());
        }
    });
}

boost::beast::string_view mime_type(boost::beast::string_view path) {
    using boost::beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == boost::beast::string_view::npos)
            return boost::beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm")) return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php")) return "text/html";
    if (iequals(ext, ".css")) return "text/css";
    if (iequals(ext, ".txt")) return "text/plain";
    if (iequals(ext, ".js")) return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml")) return "application/xml";
    if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (iequals(ext, ".flv")) return "video/x-flv";
    if (iequals(ext, ".png")) return "image/png";
    if (iequals(ext, ".jpe")) return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg")) return "image/jpeg";
    if (iequals(ext, ".gif")) return "image/gif";
    if (iequals(ext, ".bmp")) return "image/bmp";
    if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif")) return "image/tiff";
    if (iequals(ext, ".svg")) return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}
