//
// Created by 金韬 on 2021/3/8.
//

#include "web.hpp"

int main(){
    auto ip = "39.107.226.2";
    auto port = "80";
    int socketId = _web_getSocket();
    int code = _web_connectSocket(socketId,ip,port);
    auto resp = _web_callGetRequest(socketId,"file.kingtous.cn","/");
    _web_closeSocket(socketId);
    return code;
}