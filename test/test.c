str hi() {
    connect_db('127.0.0.1','root','kcr200123','school');
    int b=query_db('select * from student');
    str a = '<h1>hello from kingtous compiler</h1>';
    ret a;
}

int main() {
    echo('init...');
    echo(hi());
    str host = '127.0.0.1';
    int port = 9000;
    int core = 2;
    echo('get server...');
    int server_id = getServer(host, port, core);
    echo('server id is', server_id);
    addUrlHandler(server_id, 'GET', '/compiler', 'text/html', hi);
    echo('start server in host', host, ',port is', port);
    startServer(server_id);
    ret
    0;
}
//int main(){
//    int server_id=getServer('127.0.0.1', 9000, 2);
//    addUrlHandler(server_id, 'GET', '/compiler', 'text/html', hi);
//    connect_db('127.0.0.1','root','kcr200123",'school');
//    query_db('select * from student');
////    int a =12;
////    echo(a);
//    ret 0;
//}