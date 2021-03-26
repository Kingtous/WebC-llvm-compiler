//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

mysql::MySQL_Driver *driver;
Connection *conn;
Statement *statement;
ResultSet *resultSet;
PreparedStatement *preparedStatement;
Savepoint *savepoint;

int _connect_db(const char *host, const char *user, const char *passwd, const char *database) {
    driver = mysql::get_mysql_driver_instance();
    //连接到Mysql
    conn = driver->connect(host, user, passwd);
    if (!conn) {
        cout << "连接Mysql失败" << endl;
        return NOT_CONNECT;
    }
    //连接到指定数据库
    conn->setSchema(database);
    if (conn->getSchema() == NULL) {
        cout << "连接到指定数据库失败" << endl;
        return NOT_CONNECT;
    }
    return SUCCESS;
}

int _free_connect() {
    delete resultSet;
    delete statement;
    delete preparedStatement;
    conn->close();
    delete conn;
    return SUCCESS;
}

int _query_db(const char *sqlSentence) {
    statement = conn->createStatement();
//    statement->executeQuery(sqlSentence);
    resultSet = statement->executeQuery(sqlSentence);
    ResultSetMetaData *metaData = resultSet->getMetaData();
//    _print_json(metaData);
    cout<<metaData;
    return SUCCESS;
}

void _print_json(ResultSetMetaData *metaData) {
    cout<<strToJson(reinterpret_cast<SYSY_STR>(metaData->getColumnCount()));
}

