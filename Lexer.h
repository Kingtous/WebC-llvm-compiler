//
// Created by 金韬 on 2020/9/21.
// Description: from word to lex
//

#ifndef LLVM_KALEIDOSCOPE_LEXER_H
#define LLVM_KALEIDOSCOPE_LEXER_H

#include <string>
#include <utility>

#include "global.h"
#include "file_reader_wrapper.h"
#include "FileReader.h"

/// Token类型
enum Token {
    tok_eof = -1, // EOF
    tok_def = -2, // 函数定义
    tok_extern = -3, //调用的标准库函数
    tok_identifier = -4, //标识符
    tok_number = -5 //数字
};

class Lexer {
public:
    std::string identifierStr; //如果是tok_identifier就传入
    double NumVal = INIT_NUM; //如果是tok_number则表示数字
    int currToken; //当前的token

    /// 当前的Token
    FileReader reader;

    /// lexer构造函数：传入代码
    /// params: rawStr 代码全文
    explicit Lexer(FileReader fileReader) : reader(std::move(fileReader)) {}

    /// 获取下一个token
    int getNextToken();

private:
    int _getNextToken();

    char getchar();

    char seek();

};


#endif //LLVM_KALEIDOSCOPE_LEXER_H
