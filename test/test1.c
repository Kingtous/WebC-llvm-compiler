//
// Created by bytedance on 2021/2/1.
//
#include <stdio.h>

extern "C" {
    int sum(int *, int);
}

int main() {
    int params = 5;
    int a[5] = {1, 2, 3, 4, 5};
    printf("num is %d", sum(a, params));
}