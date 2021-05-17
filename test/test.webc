str getWeatherHtml() {
    ret
    '<!DOCTYPE html><html><head>    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />     <script src="https://cdn.bootcdn.net/ajax/libs/jquery/3.6.0/jquery.min.js"></script>    <link crossorigin="anonymous" integrity="sha384-5KZdSYqynSzIjQGS2M1O3HW6HVDBjfNx0v5Y2eQuE3vvQ9NTiiPK9/GWc0yYCrgw" href="https://lib.baomitu.com/materialize/1.0.0/css/materialize.min.css" rel="stylesheet">  <script crossorigin="anonymous" integrity="sha384-ZOED+d9HxogEQwD+jFvux96iGQR9TxfJO+mPF2ZS0TuKH6eWrmvPsDpO6I0OWdiX" src="https://lib.baomitu.com/materialize/1.0.0/js/materialize.min.js"></script>  <link href="https://cdn.bootcdn.net/ajax/libs/animate.css/4.1.1/animate.min.css" rel="stylesheet"></head><body>    <nav>        <div class="nav-wrapper">          <a href="#" class="brand-logo center">WebC测试Demo-天气预报查询</a>        </div>      </nav>      <div>&nbsp;</div>    <div class="container">        <div class="center">            <a class="btn" onclick="queryWeather()">                查询            </a>        </div>        <div>&nbsp;</div>        <div class="animated fadeInUp center">            <table>                <thead id="table-head">                  <tr id="tr-header">                      <th data-field="id">地区</th>                      <th data-field="name">天气</th>                  </tr>                </thead>                <tbody id="table-body" class="animated fadeInUp">                  <tr>                    <td>石家庄</td>                    <td>24°C</td>                  </tr>                  <tr>                    <td>承德</td>                    <td>21°C</td>                  </tr>                  <tr>                    <td>秦皇岛</td>                    <td>20°C</td>                  </tr>                </tbody>              </table>    </div>    <script>        function queryWeather() {            console.log("querying...");            $.ajax({                url : "/api/getWeather",                method : "GET",                error : function(err){                    alert("请求失败");                    console.log(err);                },                success: function(result) {                    let data = JSON.parse(result);                    data = data["result"];                    console.log(data);                    let children = $("#table-body").children();                    for(var i=0,l=children.length;i<l;i++){                        children[i].remove();                    }                    for(var p in data){                        console.log(p);                        var data_str = "<tr>" + "<td>"+data[p]["area"]+"</td>" + "<td>"+data[p]["temp"]+"°C</td>" + "</tr>";                        $("#table-body").last().append(data_str);                    }                    alert("查询完成");                }            })        }    </script></body>';
}

str getWeatherJsonData() {
    str ans = query_db('select * from weather');
    echo('database is ', ans);
    ret ans;
}


int main() {
    echo('init...');
    connect_db('39.107.226.2', 'weather', 'jintao123', 'weather', 3306, 'utf8');
    str host = '0.0.0.0';
    int port = 9000;
    int core = 4;
    echo('get server...');
    int server_id = getServer(host, port, core);
    echo('server id is', server_id);
    addUrlHandler(server_id, 'GET', '/api/getWeather', 'text/plain', getWeatherJsonData);
    addUrlHandler(server_id, 'GET', '/', 'text/html', getWeatherHtml);
    echo('start server in host', host, ',port is', port);
    startServer(server_id);
    ret
    0;
}
