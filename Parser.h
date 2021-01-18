//
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_PARSER_H
#define LLVM_KALEIDOSCOPE_PARSER_H

#include <map>
#include <llvm/ADT/STLExtras.h>

#include "ErrHelper.h"
#include "Lexer.h"

/// 语法分析器
class Parser {
public:
    Parser(Lexer lexer);

    // 词法分析器
    Lexer _lexer;

    // 解析数字
    std::unique_ptr<NodeAST> parseNumberExpr();

    // 解析标识符
    std::unique_ptr<NodeAST> parseIdentifierExpr();

    // 解析表达式
    std::unique_ptr<NodeAST> parseExpression();

    // 封装的处理方法
    std::unique_ptr<NodeAST> parsePrimary();

    // 封装条件语句
    std::unique_ptr<NodeAST> parseCondition();

    // 处理带有括号的表达式
    std::unique_ptr<NodeAST> parseParentExpr();

    // 处理extern
    std::unique_ptr<PrototypeAST> parsePrototypeExpr();

    // 处理函数def
    std::unique_ptr<FunctionAST> parseDefinition();

    // 处理extern
    std::unique_ptr<PrototypeAST> parseExtern();

    // 处理if
    std::unique_ptr<ConditionAST> parseIfExpr();

    // 测试函数：输入任意表达式进行测试
    std::unique_ptr<FunctionAST> parseTopLevelExpr();

    // 测试
    void test();

private:
    std::map<char, int> binOpPriority;

    std::unique_ptr<NodeAST> parseBinOpRHS(int priority, std::unique_ptr<NodeAST> &&uniquePtr);

    // 获取token优先级
    int getTokenPriority();

    void HandleDefinition();

    void HandleExtern();

    void HandleTopLevelExpression();
};


#endif //LLVM_KALEIDOSCOPE_PARSER_H
