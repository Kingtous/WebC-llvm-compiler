//
// Created by 金韬 on 2021/3/20.
//

#ifndef SYSYPLUS_COMPILER_KSQL_H
#define SYSYPLUS_COMPILER_KSQL_H

#include "mysql_driver.h"
#include "mysql_connection.h"
#include "mysql_error.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/connection.h"
#include "cppconn/prepared_statement.h"
#include "iostream"
#include "module/json/src/kjson.h"

#include "unordered_map"
#include "vector"
//mysql相关参数
#define HOST "127.0.0.1""
#define USER "root"
#define PASSWD "123456"
#define DATABASE "test"

//mysql连接状态码
#define NOT_CONNECT -1
#define FAILED -2
//操作成功状态码
#define SUCCESS 0

using namespace sql;
using namespace std;

extern mysql::MySQL_Driver *driver;
extern Connection *conn;
extern Statement *statement;
extern ResultSet *resultSet;
extern PreparedStatement *preparedStatement;
extern Savepoint *savepoint;



extern "C" {
    //连接到mysql
    int _connect_db(const char *host,const char *user,const char *passwd,const char *database);
    //释放资源
    int _free_connect();
    //查询数据
    int _query_db(const char* sqlSentence);
    //将query语句返回转化为字符串
    string _resToJson(ResultSet *resultSet);
}

#endif //SYSYPLUS_COMPILER_KSQL_H
