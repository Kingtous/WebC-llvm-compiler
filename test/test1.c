//
// Created by 金韬 on 2021/2/1.
//
#include <stdio.h>

extern "C" {
int sum2d(int[][2], int);
int sum(int a[], int length);
int recursive_call(int cnt);
}

int main() {
    printf("num is %d", recursive_call(100));
}