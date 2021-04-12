//
// Created by 金韬 on 2021/3/20.
//

#include "ksql.h"

mysql::MySQL_Driver *driver;
Connection *conn;
Statement *statement;
ResultSet *resultSet;
unique_ptr<char[]> ch;
vector<string> ans;

int _ksql_connect_db(const char *host, const char *user, const char *passwd, const char *database) {
    driver = mysql::get_mysql_driver_instance();
    //连接到Mysql
    conn = driver->connect(host, user, passwd);
    if (!conn) {
        cout << "连接Mysql失败" << endl;
        return NOT_CONNECT;
    }
    //连接到指定数据库
    conn->setSchema(database);
    return SUCCESS;
}

//int _free_connect() {
//    delete resultSet;
//    delete statement;
//    conn->close();
//    delete conn;
//    resultSet = nullptr;
//    statement = nullptr;
//    conn = nullptr;
//    return SUCCESS;
//}

const char *_ksql_query_db(const char *sqlSentence) {
    delete resultSet;
    delete statement;
    resultSet = nullptr;
    statement = nullptr;
    string temp = sqlSentence;
    replace(temp.begin(), temp.end(), '\"', '\'');
    statement = conn->createStatement();
    sqlSentence = temp.c_str();
    resultSet = statement->executeQuery(sqlSentence);
    if (!resultSet->next()) {
        char *resNull = "您所查询的表为空\n";
        return resNull;
    }
    if (ch.get() == nullptr) {
        ch.reset(new char[_ksql_resToJson(resultSet).size() + 1]);
    }
    strcpy(ch.get(), _ksql_resToJson(resultSet).data());
    return ch.get();
}

string _ksql_resToJson(ResultSet *result) {
    string s;
    s += "{";
    s += "\"result\":";
    s += "[";
    //列数
    int count = result->getMetaData()->getColumnCount();
    if (ans.empty()) {
        result->beforeFirst();
        while (result->next()) {
            string temp;
            for (int i = 1; i <= count; ++i) {
                if (i == 1) {
                    temp += "{";
                }
                temp += "\"";
                temp += result->getMetaData()->getColumnLabel(i);
                temp += "\":";
                auto type = result->getMetaData()->getColumnTypeName(i);
                if (type == "VARCHAR") {
                    temp += "\"";
                    temp += result->getString(i);
                    temp += "\"";
                } else {
                    temp += result->getString(i);
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
}


