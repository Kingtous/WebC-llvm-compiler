//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

int main() {
    _ksql_connect_db("127.0.0.1", "root", "123456", "school", 3306);
//    auto a = strToJson(_ksql_query_db("select * from subject"));
//    cout<<jsonToStr(a);
    cout << _ksql_query_db("select * from subject");
}