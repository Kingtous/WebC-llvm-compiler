//
// Created by 金韬 on 2021/3/23.
//
#include <iostream>

#include "kjson.h"

int main() {
    SYSY_JSON_DATA data = strToJson("{\"hello\":123}");
    try {
        auto tree = data->pt->get_child("hello2");
    } catch (boost::property_tree::ptree_bad_path &e) {
        std::cout << e.what();
    }
    auto string = jsonToStr(data);
    std::cout << "toString result" << string;
    return 0;
}