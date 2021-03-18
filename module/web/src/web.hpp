//
// Created by 金韬 on 2021/3/8.
//

#ifndef SYSYPLUS_COMPILER_WEB_HPP
#define SYSYPLUS_COMPILER_WEB_HPP

#include <map>
#include <set>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

/// 客户端状态码
#define NOT_INITIALIZED -1
#define RESOLVE_FAILED -2
#define CONNECT_FAILED -3
#define SOCKET_NOT_EXISTS -3
#define NOT_CONNECTED -4
/// 服务器状态码
#define PORT_IS_IN_USE -5
#define SERVER_NOT_EXISTS -6
/// 通用状态码
#define ROK 0

#define SERVER_NAME "Kingtous WebServer"

using namespace std;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace beast = boost::beast;

extern boost::asio::io_context *_web_io_context;
extern tcp::resolver *_web_resolver;
extern map<int, tcp::socket *> _web_tcp_socket_map;
extern boost::asio::io_context* _server_context;

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
 * @param hosts
 * @param socketId
 * @param path eg: /index
 * @return
 */
const char *_web_callGetRequest(int socketId, char *host, char *path);

/**
* 通过POST请求发送HTTP数据
* @param socketId
* @param host
* @param path
* @param body
* @return
*/
const char *_web_callPostRequest(int socketId, char *host, char *path, char *body);

/**
 * 获取一个本地HTTP服务器的ID
 * @param addr 地址
 * @param port 端口号
 * @param core 线程数
 * @return server id
 */
int _web_getServerId(const char *addr, int port, int core);

/**
 * @param sId server id
 * @param path URL path，如：/index
 * @param content_type content_type
 * @param method 方法
 * @param handler 函数指针，用于返回一个const char*内容
 * @return 状态
 */
int _web_addUrlHandler(int sId, const char *method,
                       const char *path,
                       const char *content_type,
                       const char *handler());

/**
 * 开启服务器，程序进入阻塞状态
 * @param sId server id
 * @return 状态
 */
int _web_startServe(int sId);
}

typedef const char *HandlerFunction();

/**
 * handler结构
 */
typedef struct WebHttpHandler {
    std::string *method;
    std::string *path;
    std::string *content_type;
    HandlerFunction* function;
} WebHttpHandler;

class _web_HttpWorker {
public:
    _web_HttpWorker(_web_HttpWorker const &) = delete;

    _web_HttpWorker &operator=(_web_HttpWorker const &) = delete;

    _web_HttpWorker(tcp::acceptor &acceptor, const string &basePath);

    std::unique_ptr<http::response_serializer<http::string_body>> str_serializer;

    std::unique_ptr<http::response<http::string_body>> str_resp;

    std::vector<WebHttpHandler> handlers;

    /**
     * 接收下一个客户
     */
    void accept();

    /**
     * 开启服务器
     */
    void start();

    /**
     * 读取一个request
     */
    void readRequest();

    /**
     * 检查死连接
     */
    void checkDeadline();

    /**
     * 添加Handler
     */
    void addHandler(WebHttpHandler& handler);

private:

    /**
     * 发送url未map的信息
     */
    void sendNotExistResponse();

    tcp::acceptor &acceptor;
//    using request_body_t = http::basic_dynamic_body<beast::flat_static_buffer<1024 * 1024>>;
    using request_body_t = http::string_body;
    std::string base_path;
    tcp::socket socket{acceptor.get_executor()};
    boost::optional<http::request_parser<http::string_body>> parser;
    beast::flat_static_buffer<8192> buffer;
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> request_deadline{
            acceptor.get_executor(), (std::chrono::steady_clock::time_point::max) ()};

    void process_request(http::request<request_body_t> const &request);
};


boost::beast::string_view mime_type(boost::beast::string_view path);

#endif //SYSYPLUS_COMPILER_WEB_HPP
