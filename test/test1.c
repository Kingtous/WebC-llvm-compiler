//
// Created by bytedance on 2021/2/1.
//
#include <stdio.h>

extern "C" {
int getNum();
}

int getNum2() {
    int a = 0;
    int b = 1;
    int c = a || b && a;
    return c;
}

int main() {
    printf("num is %d", getNum());
}