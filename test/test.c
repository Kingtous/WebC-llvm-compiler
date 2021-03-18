str example_func(){
    return 'this is server response string';
}

int main(){
    echo('init web framework', '123', '345');
    int socketId = getSocket();
    echo('socket id is',socketId);
    str url = 'file.kingtous.cn';
    str port = '443';
    echo('url is:',url,' port is:',port);
    int state = connectSocket(socketId,url,port);
    state = isSocketConnected(socketId);
    if (state == 0){
        echo('socket connected');
        echo('sending get request');
        str response = getRequest(socketId,url,'/');
        echo('response is:');
        echo(response);
    }
    closeSocket(socketId);
    ret 0;
}

/**
 * 获取path对应的mime type
 * @param path
 * @return mime type
 */
boost::beast::string_view mime_type(boost::beast::string_view path);