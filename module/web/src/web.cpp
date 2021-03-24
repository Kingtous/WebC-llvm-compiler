//
// Created by 金韬 on 2021/2/26.
//

#include "web.hpp"

boost::asio::io_context *_web_io_context = nullptr;
boost::asio::io_context *_server_context = nullptr;
tcp::resolver *_web_resolver = nullptr;
map<int, tcp::socket *> _web_tcp_socket_map;
map<int, pair<_web_HttpWorker *, pair<int, std::vector<std::thread *> *>>> _web_http_server_map;

// 默认证书，目前在Safari上会出现stream truncated，在chrome里需要在@link chrome://flags启用本地证书信任
std::string _web_cert = "-----BEGIN CERTIFICATE-----\n"
                        "MIIECjCCAvKgAwIBAgIBATANBgkqhkiG9w0BAQsFADCBiDELMAkGA1UEBhMCQ04x\n"
                        "EDAOBgNVBAgMB0ppYW5nWGkxDzANBgNVBAcMBllpQ2h1bjERMA8GA1UECgwIS2lu\n"
                        "Z3RvdXMxETAPBgNVBAsMCEtpbmd0b3VzMREwDwYDVQQDDAhLaW5ndG91czEdMBsG\n"
                        "CSqGSIb3DQEJARYObWVAa2luZ3RvdXMuY24wHhcNMjEwMzIwMTMwNzI4WhcNMjIw\n"
                        "MzIwMTMwNzI4WjB3MQswCQYDVQQGEwJDTjEQMA4GA1UECAwHSmlhbmdYaTERMA8G\n"
                        "A1UECgwIS2luZ3RvdXMxETAPBgNVBAsMCEtpbmd0b3VzMREwDwYDVQQDDAhLaW5n\n"
                        "dG91czEdMBsGCSqGSIb3DQEJARYObWVAa2luZ3RvdXMuY24wgZ8wDQYJKoZIhvcN\n"
                        "AQEBBQADgY0AMIGJAoGBALfmSJY9hzcuzkf6xfwhLYwaZMB6VRDFcK+Zl7tsOrGz\n"
                        "/8dofgRwoaFcd2F1hQf13luw6t23GrdlvBt1W9Fwl/zHro6YVtbGwguepPFW7sKC\n"
                        "56oFTVrwNp2M6kc/fV8zc/GhOBPI98We+Y4+9Uy4KpxYtJwbtYa2/Pt4fW5G1INL\n"
                        "AgMBAAGjggERMIIBDTAJBgNVHRMEAjAAMCwGCWCGSAGG+EIBDQQfFh1PcGVuU1NM\n"
                        "IEdlbmVyYXRlZCBDZXJ0aWZpY2F0ZTAdBgNVHQ4EFgQU4GLOIH0yicdDXoWXeH1d\n"
                        "M67y3hIwgbIGA1UdIwSBqjCBp6GBjqSBizCBiDELMAkGA1UEBhMCQ04xEDAOBgNV\n"
                        "BAgMB0ppYW5nWGkxDzANBgNVBAcMBllpQ2h1bjERMA8GA1UECgwIS2luZ3RvdXMx\n"
                        "ETAPBgNVBAsMCEtpbmd0b3VzMREwDwYDVQQDDAhLaW5ndG91czEdMBsGCSqGSIb3\n"
                        "DQEJARYObWVAa2luZ3RvdXMuY26CFDW66CGdWzzHmQ3a2kyYh4+/aaabMA0GCSqG\n"
                        "SIb3DQEBCwUAA4IBAQAVfFTW5XHhVe9AG/3ZKc/gcz/xOW0WVt7WtF4wyoAsvwWW\n"
                        "jJoB4rwJGRyTsqhBPgD02dvKfO3NmotkZ0jhfyfbSek3abft5uUJbnCHUmJvCJC/\n"
                        "25W8KSYc/SIxpClSrS5MXT+SEVUE14lvHCEbU5rI4u0hYiOJYSXFlKt8AQTw4BI0\n"
                        "4c+xyef4bfTMFIZfEh9/2WNbL59LBOEkcz/v53lrhkhc9Z7BXNb28P82ekFWAtwe\n"
                        "W3+E4j9N13M3Xyg08JP0KN2SmfoGUueZiQV4Jfdt/q8R/9hJEKL0J4LVraivZ+nv\n"
                        "hAArz815vWGNu86xAvQt8rXI7ebZW7bH0s+VfIWF\n"
                        "-----END CERTIFICATE-----";
std::string _web_default_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                               "MIICXQIBAAKBgQC35kiWPYc3Ls5H+sX8IS2MGmTAelUQxXCvmZe7bDqxs//HaH4E\n"
                               "cKGhXHdhdYUH9d5bsOrdtxq3ZbwbdVvRcJf8x66OmFbWxsILnqTxVu7CgueqBU1a\n"
                               "8DadjOpHP31fM3PxoTgTyPfFnvmOPvVMuCqcWLScG7WGtvz7eH1uRtSDSwIDAQAB\n"
                               "AoGBAJ9D6x1i0BX0jWY9QvYm7cFmolcoGz/ZHsyVopkJFJQOJbjXQDwsJ28OG73l\n"
                               "/35za03RnLbD+3lVvGGzc/+hGZ60mmtleACtXtqF5xKwjJBRO/RzvadrCudmWFlk\n"
                               "IqBtHfqb3N8EjtKZBeKboYBF606C++De3DWf/m5pky9HWNnBAkEA3EVQFFessIot\n"
                               "CekS+28Mi+vuYqozme6Pnx6z6GGncuk2cGpcweNF62WK1vjFuV3vc8X4OePCNZyS\n"
                               "D/w19U6DGwJBANW6qmCVfp+EKJDZBV+3BaxO7HvrdXQhMzS+nSlkGQh4uh4tlWRP\n"
                               "b2tyL+hjKnjX2/H5dOquk1B08O+elCW305ECQQDQWyAsneJT++Auh7HU9G5BzKJp\n"
                               "4eV9rhNcyIYVQopKQB+mpnWnUXxJ9nsmbZtahGiEcrciyzZh0AC57jB8IhARAkAs\n"
                               "ujK7oa1tDog6v9/7Bt8+Dwo4aK8czXHc4OMc7WC7wCAa/qkb4+/KSYwzEsWpomrZ\n"
                               "+b14+23C0aY2TzZrqkuxAkBl0QZp+1AbgXFleq7WhzHhe1Tl5v+YXTJJMa7o301o\n"
                               "rF+dQabkzpaJaKOVCpIpN37jIjjls4rg9zuA+zeCoY8h\n"
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
    _web_Session(boost::asio::ip::tcp::acceptor &accepter,
                 boost::asio::ssl::context &context)
            : socket_(accepter.get_executor(), context) {
    }

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type &socket() {
        return socket_.lowest_layer();
    }

    void start() {
        socket_.async_handshake(boost::asio::ssl::stream_base::server,
                                boost::bind(&_web_Session::handle_handshake, this,
                                            boost::asio::placeholders::error));
    }

    void handle_handshake(const boost::system::error_code &error) {
        if (!error) {
            printf("handshake");
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&_web_Session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            printf("handshake failer");
            delete this;
        }
    }

    void handle_read(const boost::system::error_code &error,
                     size_t bytes_transferred) {
        if (!error) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(data_, bytes_transferred),
                                     boost::bind(&_web_Session::handle_write, this,
                                                 boost::asio::placeholders::error));
        } else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code &error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&_web_Session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }

private:
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    enum {
        max_length = 1024
    };
    char data_[max_length];
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
    ssl_context.set_verify_mode(ssl::verify_none);
    ssl_context.use_certificate_chain(_web_cert_buf);
    ssl_context.use_private_key(_web_key_buf, boost::asio::ssl::context::pem);
    ssl_context.set_password_callback([](std::size_t, ssl::context_base::password_purpose) {
        return "kingtous";
    });
    ssl_context.set_options(
            ssl::context::default_workarounds | ssl::context::no_sslv2
    );
}

void _web_HttpWorker::addHandler(WebHttpHandler &handler) {
    this->handlers.push_back(handler);
}

void _web_HttpWorker::process_request(http::request<request_body_t> const &request) {
    auto target = request.target();
    auto handlers_it = this->handlers.begin();
    while (handlers_it != this->handlers.end()) {
        if ((*(*handlers_it).path) == target && (*(*handlers_it).method) == request.method_string()) {
            auto func = *(handlers_it->function);
            auto resp = func();
//            std::string resp_str(resp);
            str_resp = std::make_unique<http::response<http::string_body>>();
            str_resp->result(http::status::not_found);
            str_resp->keep_alive(false);
            str_resp->set(http::field::server, SERVER_NAME);
            str_resp->set(http::field::content_type, *(handlers_it->content_type));
            str_resp->body() = resp;
            // 计算相应长度
            str_resp->prepare_payload();
            str_serializer = std::make_unique<http::response_serializer<http::string_body>>(*str_resp);
            http::async_write(
                    *ssl_stream_,
                    *str_serializer,
                    [this](beast::error_code ec, std::size_t) {
                        accept();
                        std::fprintf(stderr, "%s", ec.message().c_str());
                    }
            );
            return;
        }
        handlers_it++;
    }
    sendNotExistResponse();
}

void _web_HttpWorker::accept() {
    acceptor.async_accept(
            asio::make_strand(*io_context),
            [this](beast::error_code ec, tcp::socket socket) {
                if (ec) {
                    std::fprintf(stderr, "%s\n", ec.message().c_str());
                } else {
                    // SSL 握手
                    ssl_stream_ = std::make_shared<ssl_stream>(std::move(socket), ssl_context);
                    ssl_stream_->async_handshake(ssl::stream_base::server, [this](beast::error_code ec) {
                        if (ec) {
                            std::fprintf(stderr, "%s\n", ec.message().c_str());
                        } else {
                            readRequest();
                        }
                    });
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
