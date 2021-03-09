//use http;
int global[5] = {1, 2, 3, 4, 5};

//int ctotal = 0;

str a = '1234';

char getStr(int c){
    lp (int i = 0; i < 4; i = i+1) {
        str b = a;
        ret b[0];
    }
    ret a[5];
}

int changeNumtoZero(int num){
    num = 2;
    ret num;
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
    ret 1;
}