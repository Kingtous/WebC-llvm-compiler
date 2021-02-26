//
// Created by 金韬 on 2021/2/26.
//


#include <sys/time.h>

/**
 * 获取ms数
 * @return ms
 */
long __getms(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}
