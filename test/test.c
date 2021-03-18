str hi() {
    str a = 'hello from kingtous compiler';
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
    addUrlHandler(server_id, 'POST', '/compiler', hi);
    echo('start server in host', host, ',port is', port);
    startServer(server_id);
    ret 0;
}
