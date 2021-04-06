//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

int main() {
    _connect_db("127.0.0.1","root","kcr200123","school");
    cout<<_query_db("select * from student");
}