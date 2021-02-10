//
// Created by bytedance on 2021/2/1.
//
#include <iostream>

extern "C" {
    int getNum(int);
}

int main(){
    getNum(8);
}