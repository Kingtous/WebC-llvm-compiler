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
    return OK;
}

int _web_connect(int sId, const char *baseUrlOrIp, const char *port) {
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
    return socket->is_open() ? OK : CONNECT_FAILED;
}

int _web_closeWeb(int sId) {
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return SOCKET_NOT_EXISTS;
    }
    boost::system::error_code ec;
    socketIt->second->shutdown(tcp::socket::shutdown_both,ec);
    return OK;
}

int _web_isConnected(int sId) {
    auto socketIt = _web_tcp_socket_map.find(sId);
    if (socketIt == _web_tcp_socket_map.end()) {
        return SOCKET_NOT_EXISTS;
    }
    return socketIt->second->is_open() ? OK : NOT_CONNECTED;
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
    return string;
}
