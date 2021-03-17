//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

const char *say_hello() {
    return "hello_world";
}

int main() {
    int serverId = _web_getServerId("127.0.0.1", 9000);
    int res = _web_addUrlHandler(serverId, "POST", "/hello", say_hello);
    if (res != ROK) {
        return res;
    }
    return _web_startServe(serverId);
}