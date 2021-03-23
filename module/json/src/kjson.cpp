//
// Created by 金韬 on 2021/3/23.
//

#include "kjson.h"

namespace pt =
boost::property_tree;

SYSY_JSON_DATA strToJson(SYSY_STR str) {
    auto jd = new JsonData();
    std::stringstream is;
    is << str;
    auto ptree = std::make_shared<pt::ptree>();
    pt::read_json(is, *ptree);
    jd->pt = ptree;
    return jd;
}

SYSY_STR jsonToStr(SYSY_JSON_DATA json) {
    std::stringstream ss;
    pt::write_json(ss, *(json->pt), false); // 不要美化
    auto string = std::make_shared<std::string>(ss.str());
    return string->c_str();
}
