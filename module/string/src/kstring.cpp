//
// Created by bytedance on 2021/3/18.
//

#include "kstring.h"

const char *toString(void *a, sysytype_t type) {
    switch (type) {
        case SYSYTYPE_INT: {
            int *tmp = static_cast<int *>(a);
            int tmp_num = *tmp;
            auto str = std::to_string(tmp_num);
            char *str_mem = new char[str.size() + 1];
            memcpy(str_mem, str.c_str(), str.size() + 1);
            return str_mem;
        }
        case SYSYTYPE_DOUBLE: {
            auto *tmp = static_cast<double *>(a);
            auto tmp_num = *tmp;
            auto str = std::to_string(tmp_num);
            char *str_mem = new char[str.size() + 1];
            memcpy(str_mem, str.c_str(), str.size() + 1);
            return str_mem;
        }
        case SYSYTYPE_LONG: {
            auto *tmp = static_cast<long *>(a);
            auto tmp_num = *tmp;
            auto str = std::to_string(tmp_num);
            char *str_mem = new char[str.size() + 1];
            memcpy(str_mem, str.c_str(), str.size() + 1);
            return str_mem;
        }
        default:
            return "unknown type";
    }
}
