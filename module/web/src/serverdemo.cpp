//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

const char *say_hello() {
//    const char *ans = _ksql_query_db("select * from students");
    return "select * from weather";
}

void startServer() {
    int id = _web_getServerId("127.0.0.1", 9000, 4);
    _web_addUrlHandler(id, "GET", "/hello", "text/html", say_hello);
    _web_startServe(id);
}


int main() {
    _ksql_connect_db("39.107.226.2", "weather", "jintao123", "weather",3306,"utf8");
//    _ksql_connect_db(HOST, USER, PASSWD, DATABASE);
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