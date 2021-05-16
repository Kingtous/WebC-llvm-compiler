//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

#include <mutex>

#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "iostream"
#include "module/json/src/kjson.h"
#include "vector"

//mysql连接状态码
#define NOT_CONNECT -2
#define FAILED -1
//操作成功状态码
#define SUCCESS 0

//extern sql::mysql::MySQL_Driver * get_mysql_driver_instance();

using namespace std;

struct SQLData {
    sql::Statement *statement;
    sql::ResultSet *resultSet;
};

typedef SQLData WEBC_SQL_DATA;

sql::mysql::MySQL_Driver *driver;
sql::Connection *conn;
WEBC_SQL_DATA webcSqlData;

std::mutex mysql_mutex;

string _ksql_resToJson(WEBC_SQL_DATA &sqlData);

int _ksql_connect_db(const char *hostname, const char *username, const char *password, const char *schema, int port,
                     const char *charset) {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        //连接到Mysql
        sql::ConnectOptionsMap connection_properties;
        connection_properties["hostName"] = hostname;
        connection_properties["userName"] = username;
        connection_properties["password"] = password;
        connection_properties["schema"] = schema;
        connection_properties["port"] = port;
        connection_properties["OPT_RECONNECT"] = true;
        connection_properties["OPT_CHARSET_NAME"] = charset;
        conn = driver->connect(connection_properties);
        if (!conn) {
            cout << "连接Mysql失败" << endl;
            return FAILED;
        }
        return SUCCESS;
    } catch (sql::SQLException &e) {
        cout << "The error code is : " << e.getErrorCode() << endl;
        cout << e.what() << endl;
    }
    return FAILED;
}

const char *_ksql_query_db(const char *sqlSentence) {
    mysql_mutex.lock();
    try {
        if (_ksql_isMysqlConnected() != 0) {
            return "mysql未能成功连接";
        }
        _ksql_free_memory();
        string temp = sqlSentence;
        replace(temp.begin(), temp.end(), '\"', '\'');
        webcSqlData.statement = conn->createStatement();
        sqlSentence = temp.c_str();
        webcSqlData.resultSet = webcSqlData.statement->executeQuery(sqlSentence);
        if (!webcSqlData.resultSet->next()) {
            return "您所查询的表为空\n";
        }
        // FIXME: 会有内存泄露
        auto jsondata = _ksql_resToJson(webcSqlData);
        auto cstrdata = new string(jsondata);
        mysql_mutex.unlock();
        return cstrdata->c_str();
    } catch (sql::SQLException &e) {
        cout << "The error code is : " << e.getErrorCode() << endl;
        cout << e.what() << endl;
        mysql_mutex.unlock();
    }
    return "";
}

string _ksql_resToJson(WEBC_SQL_DATA &sqlData) {
    vector<string> ans;
    try {
        string s;
        s += "{";
        s += "\"result\":";
        s += "[";
        //列数
        unsigned int count = sqlData.resultSet->getMetaData()->getColumnCount();
        if (ans.empty()) {
            sqlData.resultSet->beforeFirst();
            while (sqlData.resultSet->next()) {
                auto metadata = sqlData.resultSet->getMetaData();
                string temp;
                for (int i = 1; i <= count; ++i) {
                    if (i == 1) {
                        temp += "{";
                    }
                    temp += "\"";
                    temp += metadata->getColumnLabel(i);
                    temp += "\":";
                    auto type = metadata->getColumnTypeName(i);
                    if (type == "VARCHAR") {
                        temp += "\"";
                        temp += sqlData.resultSet->getString(i);
                        temp += "\"";
                    } else {
                        temp += sqlData.resultSet->getString(i);
                    }
                    if (i == count) temp += "}";
                    temp += ",";
                }
                ans.push_back(temp);
            }
            ans[ans.size() - 1].pop_back();
        }
        for (const auto &str:ans) {
            s += str;
        }
        s += "]}";
        return s;
    } catch (sql::SQLException &e) {
        cout << "The error code is : " << e.getErrorCode() << endl;
        cout << e.what() << endl;
    }
    return "";
}

int _ksql_free_memory() {
    delete webcSqlData.resultSet;
    delete webcSqlData.statement;
    webcSqlData.resultSet = nullptr;
    webcSqlData.statement = nullptr;
    return SUCCESS;
}

int _ksql_isMysqlConnected() {
    return conn->isClosed() ? NOT_CONNECT : SUCCESS;
}


