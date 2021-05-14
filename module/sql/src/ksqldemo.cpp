//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

int main() {
    _ksql_connect_db("39.107.226.2", "weather", "jintao123", "weather", 3306, "GBK");
//    auto a = strToJson(_ksql_query_db("select * from subject"));
//    cout<<jsonToStr(a);
    cout << _ksql_query_db("select * from weather");
}