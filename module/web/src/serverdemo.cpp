//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

const char *say_hello() {
    const char *ans = _ksql_query_db("select * from student");
    return ans;
}

const char *say_hi() {
    const char *ans = _ksql_query_db("select * from result");
    return ans;
}

const char *say_hehe() {
    const char *ans = _ksql_query_db("select * from subject");
    return ans;
}

void startServer() {
    int id = _web_getServerId("127.0.0.1", 9000, 2);
    _web_addUrlHandler(id, "GET", "/hello", "text/html", say_hello);
    _web_addUrlHandler(id, "GET", "/hi", "text/html", say_hi);
    _web_addUrlHandler(id, "GET", "/hehe", "text/html", say_hehe);
    _web_startServe(id);
}


int main() {
    _ksql_connect_db("127.0.0.1", "root", "123456", "school",3306,"utf8");
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