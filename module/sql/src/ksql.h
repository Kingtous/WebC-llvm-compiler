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

//mysql连接状态码
#define NOT_CONNECT -2
#define FAILED -1
//操作成功状态码
#define SUCCESS 0

using namespace sql;
using namespace std;

struct SQLData {
    Statement *statement;
    ResultSet *resultSet;
};

typedef SQLData WEBC_SQL_DATA;

extern "C" {
/**
 * 连接到mysql
 * @param hostname
 * @param username
 * @param password
 * @param schema
 * @param port
 * @return
 */
int _ksql_connect_db(const char *hostname, const char *username, const char *password, const char *schema, int port);
/**
 * 释放资源
 * @return
 */
int _ksql_free_memory();
/**
 * 查询数据
 * @param sqlSentence
 * @return
 */
const char *_ksql_query_db(const char *sqlSentence);
/**
 * 判断mysql是否连接
 * @return
 */
int _ksql_isMysqlConnected();
}

#endif //SYSYPLUS_COMPILER_KSQL_H
