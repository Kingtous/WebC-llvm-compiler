//
// Created by 金韬 on 2021/2/1.
//
#include <stdio.h>

//char* a = "1234";

extern int f();

extern "C" {
char getStr();
}

int main() {
    char str = getStr();
    printf("%c",str);
}