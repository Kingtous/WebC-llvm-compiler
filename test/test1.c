//
// Created by bytedance on 2021/2/1.
//
#include <stdio.h>

extern "C" {
int getNum();
}

int main() {
    printf("num is %d", getNum());
}