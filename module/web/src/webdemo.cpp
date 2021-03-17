//
// Created by 金韬 on 2021/3/8.
//

#include "web.hpp"

int main(){
    auto ip = "39.107.226.2";
    auto port = "80";
    int socketId = _web_getSocket();
    int code = _web_connectSocket(socketId,ip,port);
//    auto resp = _web_callGetRequest(socketId,"file.kingtous.cn","/");
    auto resp = _web_callPostRequest(socketId,"file.kingtous.cn","/","name=root");
    cout<<resp;
    _web_closeSocket(socketId);
    return code;
}