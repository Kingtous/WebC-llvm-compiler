//
// Created by 金韬 on 2021/2/1.
//
#include <stdio.h>
#include <time.h>
#include <unistd.h>
//char* a = "1234";

//extern int f();
//
//extern "C" {
//    char getStr();
//}

int get(int maxv) {
    if (maxv > 0) {
        return get(maxv - 1);
    }
    return maxv;
}

int main() {
    char str = '1';
    long t1 = time(NULL);
    sleep(5);
    long t2 = time(NULL);
    printf("%ld", t2 - t1);
}