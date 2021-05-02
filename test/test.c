str hello() {
    str ans = query_db('select * from student');
    ret ans;
}

str hi() {
    str ans = query_db('select * from result');
    ret ans;
}

str hehe() {
    str ans = query_db('select * from subject');
    ret ans;
}

int main() {
    echo('init...');
    connect_db('127.0.0.1', 'root', '123456', 'school',3306);
    str host = '0.0.0.0';
    int port = 9000;
    int core = 2;
    echo('get server...');
    int server_id = getServer(host, port, core);
    echo('server id is', server_id);
    addUrlHandler(server_id, 'GET', '/hello', 'text/html', hello);
    addUrlHandler(server_id, 'GET', '/hi', 'text/html', hi);
    addUrlHandler(server_id, 'GET', '/hehe', 'text/html', hehe);
    echo('start server in host', host, ',port is', port);
    startServer(server_id);
    ret
    0;
}
//int main(){
//    int server_id=getServer('127.0.0.1', 9000, 2);
//    addUrlHandler(server_id, 'GET', '/compiler', 'text/html', hi);
//    connect_db('127.0.0.1','root','123456','school');
//    str b = query_db('select * from subject');
//    int a =12;
//    echo(a);
//    echo(b);
//    ret 0;
//}