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
//    _print_json(metaData);
    cout<<_resToJson(resultSet);
    return SUCCESS;
}

void _print_json(ResultSetMetaData *metaData) {
    cout<<strToJson(reinterpret_cast<SYSY_STR>(metaData));
}

string _resToJson(ResultSet *result) {
    string s;
    s+="[";
    //列数
    int count = result->getMetaData()->getColumnCount();
    vector<string> ans;
    while (result->next()){
        string temp;
        for (int i = 1; i <= count ; ++i) {
            if(i==1){
                temp+="{";
            }
            temp+="\"";
            temp+=result->getMetaData()->getColumnLabel(i);
            temp+="\":";
            auto type = result->getMetaData()->getColumnTypeName(i);
            if(type=="VARCHAR"){
                temp+="\"";
                temp+=result->getString(i);
                temp+="\"";
            }
            else{
                temp+=result->getString(i);
            }
            if (i==count) temp+="}";
            temp+=",";
        }
        ans.push_back(temp);
    }
    ans[ans.size()-1].pop_back();
    for (auto str:ans){
        s+=str;
    }
    s+="]";
return s;
    //获取值
}


