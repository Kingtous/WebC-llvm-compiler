//
// Created by bytedance on 2021/2/1.
//
#include <iostream>

extern "C" {
    int func();
}

int main(){
    std::cout << func();
}