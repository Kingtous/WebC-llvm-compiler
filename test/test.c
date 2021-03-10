int recursive_call(int a){
    if (a == 0){
        ret a;
    } el {
        ret recursive_call(a-1);
    }
}

int main(){
    //str a = repeated 0 25;
    echo('init web framework');
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