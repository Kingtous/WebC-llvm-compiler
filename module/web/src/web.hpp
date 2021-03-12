//
// Created by 金韬 on 2021/3/8.
//

#ifndef SYSYPLUS_COMPILER_WEB_HPP
#define SYSYPLUS_COMPILER_WEB_HPP

#include <map>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#define NOT_INITIALIZED -1
#define RESOLVE_FAILED -2
#define CONNECT_FAILED -3
#define SOCKET_NOT_EXISTS -3
#define NOT_CONNECTED -4
#define ROK 0

using namespace std;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

extern boost::asio::io_context *_web_io_context;
extern tcp::resolver *_web_resolver;
extern map<int, tcp::socket *> _web_tcp_socket_map;

// Keep 函数名，方便Linking
extern "C" {

/**
 * 初始化网络功能
 * @return 如上的返回值
 */
int _web_init();

/**
 * 获取一个Socket
 * @return
 */
int _web_getSocket();

/**
 * 连接Web
 * @param socketId socketId
 * @param baseUrl eg: kingtous.cn
 * @param port 端口号
 * @return 连接状态
 */
int _web_connectSocket(int socketId, const char *baseUrlOrIp, const char *port);

/**
 * 关闭socket
 * @param socketId
 * @return
 */
int _web_closeSocket(int socketId);

/**
 * socket是否连接
 * @param socketId
 * @return
 */
int _web_isSocketConnected(int socketId);

/**
 * 通过GET请求发送HTTP数据
 * @param host
 * @param socketId
 * @param path eg: /index
 * @return
 */
const char *_web_callGetRequest(int socketId, char *host, char *path);
}


#endif //SYSYPLUS_COMPILER_WEB_HPP
