//
// Created by 金韬 on 2021/2/1.
//
#include <stdio.h>
#include <time.h>
//char* a = "1234";

extern int f();

extern "C" {
    char getStr();
}

int main() {
    char str = getStr();
    long t1 = time(NULL);
    printf("%c",str);
}