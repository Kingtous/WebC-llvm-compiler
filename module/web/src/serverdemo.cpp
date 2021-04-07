//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

//string ans="";
const char *say_hello() {
    _connect_db("127.0.0.1", "root", "123456", "school");
    const char *ans = _query_db("select * from result");
//    _free_connect();
    return ans;
}

const char *say_hi() {
    return "hi";
}


void startServer() {
    int id = _web_getServerId("127.0.0.1", 9000, 2);
    _web_addUrlHandler(id, "GET", "/hello", "text/html", say_hello);
    _web_startServe(id);
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