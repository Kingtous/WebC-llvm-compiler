//
// Created by 金韬 on 2020/9/21.
// Description: from word to lex
//

#ifndef LLVM_KALEIDOSCOPE_LEXER_H
#define LLVM_KALEIDOSCOPE_LEXER_H

#include <string>
#include <utility>

#include "../Global.h"
#include "IFileReader.h"
#include "FileReader.h"
#include "NewParser.tab.h"

/// Token类型
enum Token {
    tok_eof = 258, // EOF
    tok_func = 259, // 函数定义
    tok_identifier = 260, //标识符: + - * /
    tok_number = 261, //数字
    tok_if = 262,
    tok_else = 263,
    tok_elif = 264,
    tok_comma = 265, //逗号
    tok_const = 266,
    tok_extern = 267,
    tok_while = 268,
    tok_break = 269,
    tok_continue = 270,
    tok_return = 271
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
