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

#define NEXTCHAR (LastChar=getchar())

class Lexer {
public:
    std::string identifierStr = ""; //如果是tok_identifier就传入
    double NumVal = INIT_NUM; //如果是tok_number则表示数字
    int currToken; //当前的token


    /// lexer构造函数：传入代码
    /// params: rawStr 代码全文
    explicit Lexer(FileReader fileReader) : reader(std::move(fileReader)) {}

    /// 获取下一个token
    int getNextToken();

    unsigned int getCLineNumber();

    unsigned int getCCol();

private:
    /// 当前的Token
    FileReader reader;

    int _getNextToken();

    char getchar();

    char seek();

};

extern Lexer *TheLexer;

int yylex();

#endif //LLVM_KALEIDOSCOPE_LEXER_H
