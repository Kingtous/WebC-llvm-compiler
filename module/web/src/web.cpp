//
// Created by 金韬 on 2021/2/26.
//

#include "web.hpp"

boost::asio::io_context *_web_io_context = nullptr;
tcp::resolver *_web_resolver = nullptr;
map<int, tcp::socket *> _web_tcp_socket_map;
/**
 * HTTP 1.1
 */
auto _web_http_version = 11;
auto _web_agent = "kingtous_web_agent";

/**
 * 递增的SocketId
 */
volatile int socketId = 0;

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

asio::io_context server_context{1};

int _web_getServerId(const char *addr, int port) {
    _web_init();
    auto address = boost::asio::ip::make_address(addr);
    // TODO
//    asio::
    return ROK;
}

int _web_addUrlHandler(int sId, const char *method, const char *path, const char *(*handler)()) {
    // TODO
    return ROK;
}

int _web_startServe(int sId) {
    // TODO
    return ROK;
}

_web_HttpServer::_web_HttpServer(tcp::acceptor &acceptor, const string &basePath) : acceptor(acceptor),
                                                                                    base_path(basePath) {

}

void _web_HttpServer::process_request(http::request<request_body_t> const &request) {
    str_resp = std::make_unique<http::response<http::string_body>>();
    str_resp->result(200);
    str_resp->keep_alive(false);
    str_resp->set(http::field::server, "Beast");
    str_resp->set(http::field::content_type, "text/plain");
    str_resp->body() = "Hello Beast.";
    // 计算相应长度
    str_resp->prepare_payload();
    str_serializer = std::make_unique<http::response_serializer<http::string_body>>(*str_resp);
    http::async_write(
            socket,
            *str_serializer,
            [this](beast::error_code ec, std::size_t) {
                socket.shutdown(tcp::socket::shutdown_send, ec);
                accept();
            }
    );
}

void _web_HttpServer::accept() {
    beast::error_code ec;
    socket.close(ec);
    buffer.consume(buffer.size());

    acceptor.async_accept(
            socket,
            [this](beast::error_code ec) {
                if (ec) {
                    accept();
                } else {
                    request_deadline.expires_after(std::chrono::seconds(60));
                    readRequest();
                }
            }
    );
}

void _web_HttpServer::readRequest() {
    parser.emplace();
    http::async_read(
            socket,
            buffer,
            *parser,
            [this](beast::error_code ec, std::size_t) {
                if (ec) {
                    accept();
                } else {
                    process_request(parser->get());
                }
            }
    );
}

void _web_HttpServer::start() {
    accept();
    checkDeadline();
}

void _web_HttpServer::checkDeadline() {
    if (request_deadline.expiry() <= std::chrono::steady_clock::now()) {
        socket.close();
        request_deadline.expires_at(std::chrono::steady_clock::time_point::max());
    }

    request_deadline.async_wait([this](beast::error_code) {
        checkDeadline();
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
