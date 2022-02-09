//
// Created by 金韬 on 2021/3/20.
//
#include <iostream>

#include "ksql.h"

int main() {
    _ksql_connect_db("39.107.226.2", "weather", "jintao123", "weather", 3306, "utf8");
//    auto a = strToJson(_ksql_query_db("select * from subject"));
//    cout<<jsonToStr(a);

    // std::cout << _ksql_query_db("select * from weather");
    // std::cout << _ksql_insert_db("insert into `weather` (`area`,`temp`) values('哈','30')");
    // std::cout<< _ksql_update_db("update `weather` set `area` = '石家庄' where `temp`='30' ");
    std::cout<< _ksql_delete_db("delete from `weather` where `area`='哈'");
}