//
// Created by 金韬 on 2020/9/21.
// Description: from word to lex
//

#ifndef LLVM_KALEIDOSCOPE_LEXER_H
#define LLVM_KALEIDOSCOPE_LEXER_H

#include <string>
#include <cctype>
#include <utility>

#include "../Global.h"
#include "IFileReader.h"
#include "FileReader.h"
#include "Parser.hpp"
#include "StringReader.h"

#define NEXTCHAR (last_char=getchar())

class Lexer {
public:
    std::string identifierStr = ""; //如果是tok_identifier就传入
    double NumVal = INIT_NUM; //如果是tok_number则表示数字
    int currToken; //当前的token


    /// lexer构造函数：传入代码
    /// params: rawStr 代码全文
//    explicit代表这个构造函数不能进行隐式的类型转换
    explicit Lexer(IFileReader *fileReader) : reader(fileReader) {}

    /// 获取下一个token
    int getNextToken();

    unsigned int getCLineNumber();

    unsigned int getCCol();

private:
    /// 当前的Token
    IFileReader* reader;

    int last_char = ' ';

    int _getNextToken();

    int getchar();

    int seek();
};

extern Lexer *TheLexer;

int yylex();

#endif //LLVM_KALEIDOSCOPE_LEXER_H
