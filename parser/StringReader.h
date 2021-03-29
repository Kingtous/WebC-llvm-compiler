//
// Created by bytedance on 2021/3/15.
//

#ifndef SYSYPLUS_COMPILER_STRINGREADER_H
#define SYSYPLUS_COMPILER_STRINGREADER_H

#include "Global.h"
#include "IFileReader.h"

/**
 * 使用字符串传入分析
 */
class StringReader : public IFileReader {

    std::string* str;

    unsigned int lineno = 0;
    unsigned int colno = 0;
    unsigned int cindex = 0;

public:
    explicit StringReader(std::string *str);

private:

    int getchar() override;

    int seek() override;

    unsigned int getLineNo() override;

    unsigned int getCCol() override;
};


#endif //SYSYPLUS_COMPILER_STRINGREADER_H
