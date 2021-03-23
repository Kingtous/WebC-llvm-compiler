//
// Created by 金韬 on 2021/3/23.
//

#ifndef SYSYPLUS_COMPILER_KJSON_H
#define SYSYPLUS_COMPILER_KJSON_H

#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// TODO 之后改为const char*
struct JsonData {
    std::shared_ptr<boost::property_tree::ptree> pt;
};
typedef JsonData *SYSY_JSON_DATA;
typedef const char *SYSY_STR;

extern "C" {

/**
 * @brief string转json类
 * @param str string数据
 * @return json data
 */
SYSY_JSON_DATA strToJson(SYSY_STR str);


/**
 * @brief json转string
 * @param json json数据
 * @return string
 */
SYSY_STR jsonToStr(SYSY_JSON_DATA json);

}
#endif //SYSYPLUS_COMPILER_KJSON_H
