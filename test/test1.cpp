//
// Created by 金韬 on 2021/2/1.
//
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stack>

extern "C" {
void myfunc(int (*my)()) {
    my();
}

int ok() {
    return 0;
}
}


int main() {
    myfunc(ok);
}
//class A {
//    public:
//    int a;
//    int b;
//    int print() {
//        return a;
//    }
//    static int print2(){
//        return 5;
//    }
//};
//
//
//int af(int a){
//    return af(a-3) + af(a-2);
//}
//
//int get(int maxv) {
//    if (maxv > 0) {
//        return get(maxv - 1);
//    }
//    return maxv;
//}
//
//int main() {
//    int a;
//    printf("%d",a);
//    A::print2();
//}