//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

const char *say_hello() {
    return "hello_world";
}

const char *say_hi() {
    return "hi";
}


void startServer() {
    auto const address = boost::asio::ip::make_address("127.0.0.1");
    unsigned short port = 9000;
    std::string root = "~/";

    auto handler1 = new WebHttpHandler();
    auto handler2 = new WebHttpHandler();

    handler1->path = new std::string("/hello");
    handler2->path = new std::string("/hi");

    handler1->method = new std::string("GET");
    handler2->method = new std::string("POST");

    handler1->function = say_hello;
    handler2->function = say_hi;

    boost::asio::io_context ioc{1};
    tcp::acceptor acceptor{ioc, {address, port}};
    _web_HttpWorker httpWorker1(acceptor, root);
    _web_HttpWorker httpWorker2(acceptor, root);

    httpWorker1.addHandler(*handler1);
    httpWorker1.addHandler(*handler2);
    httpWorker2.addHandler(*handler1);
    httpWorker2.addHandler(*handler2);

    httpWorker1.start();
    httpWorker2.start();
    ioc.run();
}


int main() {
    startServer();
    // for test interface
//    int serverId = _web_getServerId("127.0.0.1", 9000);
//    int res = _web_addUrlHandler(serverId, "POST", "/hello", say_hello);
//    if (res != ROK) {
//        return res;
//    }
//    return _web_startServe(serverId);
    return 0;
}