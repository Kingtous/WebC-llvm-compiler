//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

mysql::MySQL_Driver *driver;
Connection *conn;
WEBC_SQL_DATA webcSqlData;
unique_ptr<char[]> ch;
vector<string> ans;

string _ksql_resToJson(WEBC_SQL_DATA sqlData);

int _ksql_connect_db(const char *hostname, const char *username, const char *password, const char *schema, int port,
                     const char *charset) {
    try {
        driver = mysql::get_mysql_driver_instance();
        //连接到Mysql
        ConnectOptionsMap connection_properties;
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
    } catch (SQLException e) {
        cout << "The error code is : "<<e.getErrorCode() << endl;
        cout << e.what() << endl;
    }
}

const char *_ksql_query_db(const char *sqlSentence) {
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
            char *resNull = "您所查询的表为空\n";
            return resNull;
        }
        if (ch.get() == nullptr) {
            ch.reset(new char[_ksql_resToJson(webcSqlData).size() + 1]);
        }
        strcpy(ch.get(), _ksql_resToJson(webcSqlData).data());
        return ch.get();
    } catch (SQLException e) {
        cout << "The error code is : "<<e.getErrorCode() << endl;
        cout << e.what() << endl;
    }
}

string _ksql_resToJson(WEBC_SQL_DATA sqlData) {
    try {
        string s;
        s += "{";
        s += "\"result\":";
        s += "[";
        //列数
        int count = sqlData.resultSet->getMetaData()->getColumnCount();
        if (ans.empty()) {
            sqlData.resultSet->beforeFirst();
            while (sqlData.resultSet->next()) {
                string temp;
                for (int i = 1; i <= count; ++i) {
                    if (i == 1) {
                        temp += "{";
                    }
                    temp += "\"";
                    temp += sqlData.resultSet->getMetaData()->getColumnLabel(i);
                    temp += "\":";
                    auto type = sqlData.resultSet->getMetaData()->getColumnTypeName(i);
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
        for (auto str:ans) {
            s += str;
        }
        s += "]}";
        ans.clear();
        vector<string>().swap(ans);
        return s;
    } catch (SQLException e) {
        cout << "The error code is : "<<e.getErrorCode() << endl;
        cout << e.what() << endl;
    }
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


