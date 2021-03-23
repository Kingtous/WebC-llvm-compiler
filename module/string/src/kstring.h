//
// Created by 金韬 on 2021/3/18.
//

#ifndef SYSYPLUS_COMPILER_KSTRING_H
#define SYSYPLUS_COMPILER_KSTRING_H

#include <string>
#include <cstring>

typedef int sysytype_t;

#define SYSYTYPE_INT 0
#define SYSYTYPE_LONG 1
#define SYSYTYPE_DOUBLE 2

extern "C" {
/**
 * 转string
 * @param 变量地址
 * @param type 如上
 * @return
 */
const char *toString(void *a, sysytype_t type);
};

#endif //SYSYPLUS_COMPILER_KSTRING_H
