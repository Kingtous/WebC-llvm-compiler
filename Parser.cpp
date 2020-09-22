//
// Created by 金韬 on 2020/9/21.
//

#include "Parser.h"


/// 生成：数字结点 ::= 数字
/// 当token为tok_number时调用生成一个AST子树
std::unique_ptr<ExprAST> Parser::parseNumberExpr() {
    // 此处使用c++ 11"作者"不小心漏掉的然后被llvm补上的make_unique
    auto result = llvm::make_unique<NumberExprAST>(_lexer.NumVal);

    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
    std::string name = _lexer.identifierStr;
    _lexer.getNextToken();

    if (_lexer.currToken != '(') {
        // 如果不是(的话，那么就是一个标识符
        return llvm::make_unique<VariableExprAST>(name);
    } else {
        // 证明此时为一个函数调用：function(
        _lexer.getNextToken();
        // 开启获取文本函数调用
        std::vector<std::unique_ptr<ExprAST>> args;
        if (_lexer.currToken != ')') {
            // 还没碰到)
            while (true) {
                if (auto arg = parseExpression()) {
                    args.push_back(std::move(arg));
                } else {
                    // 处理表达式出错
                    return nullptr;
                }

                // TODO 查看这里是不是应该放上去？
                if (_lexer.currToken == ')') {
                    break;
                }

                if (_lexer.currToken != ',') {
                    return LogError("Expected ')' or ',' in argument list");
                }
                // 获取下一个token
                _lexer.getNextToken();
            }
        }
        _lexer.getNextToken(); // 跳过)
        return llvm::make_unique<CallExprAST>(name, std::move(args));
    }

}

std::unique_ptr<ExprAST> Parser::parseExpression() {
    auto L = parsePrimary();
    if (!L) {
        return nullptr;
    }
    return parseBinOpRHS(0, std::move(L));
}

std::unique_ptr<ExprAST> Parser::parsePrimary() {
    switch (_lexer.currToken) {
        case tok_identifier:
            return parseIdentifierExpr();
        case tok_number:
            return parseNumberExpr();
        case '(':
            return parseParentExpr();
        default:
            return LogError("unknown token");
    }
}

std::unique_ptr<ExprAST> Parser::parseParentExpr() {
    _lexer.getNextToken();//过滤(
    auto v = parseExpression(); //获取里面的内容
    if (!v) {
        // 出错
        return nullptr;
    }
    if (_lexer.currToken != ')') {
        return LogError("expected ')'");
    }
    _lexer.getNextToken();
    return v;
}

Parser::Parser() {
    // 定义算术优先级
    binOpPriority['<'] = 10;
    binOpPriority['>'] = 10;
    binOpPriority['+'] = 20;
    binOpPriority['-'] = 20;
    binOpPriority['*'] = 40;
    binOpPriority['/'] = 35;
}

std::unique_ptr<ExprAST> Parser::parseBinOpRHS(int priority, std::unique_ptr<ExprAST> &&L) {
    while (1) {
        int tokenPriority = getTokenPriority();
        if (tokenPriority < priority) {
            return std::move(L);
        } else {
            int binOper = _lexer.currToken;
            _lexer.getNextToken();
            auto R = parsePrimary();
            if (!R) {
                return nullptr;
            }
            int nextPriority = getTokenPriority();
            if (tokenPriority < nextPriority) {
                // 右边优先级更高
                R = parseBinOpRHS(tokenPriority + 1, std::move(R));
                if (!R) {
                    return nullptr;
                }
            }
            L = llvm::make_unique<BinaryExprAST>(binOper, std::move(L), std::move(R));
        }
    }
}

int Parser::getTokenPriority() {
    if (!isascii(_lexer.currToken))
        return -1;
    int priority = binOpPriority[_lexer.currToken];
    if (priority <= 0)
        return -1;
    return priority;
}

std::unique_ptr<PrototypeAST> Parser::parsePrototypeExpr() {
    if (_lexer.currToken != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = _lexer.identifierStr;
    _lexer.getNextToken();

    if (_lexer.currToken != '(')
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (_lexer.getNextToken() == tok_identifier)
        ArgNames.push_back(_lexer.identifierStr);
    if (_lexer.currToken != ')')
        return LogErrorP("Expected ')' in prototype");
    // success.
    _lexer.getNextToken();  // eat ')'.

    return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::parseDefinition() {
    _lexer.getNextToken();
    auto proto = parsePrototypeExpr(); // def sin(a)
    if (auto E = parseExpression()) {
        return llvm::make_unique<FunctionAST>(std::move(proto), std::move(E));
    } else {
        return nullptr;
    }
}

std::unique_ptr<PrototypeAST> Parser::parseExtern() {
    _lexer.getNextToken();
    return parsePrototypeExpr();
}

std::unique_ptr<FunctionAST> Parser::parseTopLevelExpr() {
    if (auto expression = parseExpression()) {
        auto proto = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
        return llvm::make_unique<FunctionAST>(std::move(proto), std::move(expression));
    }
    return nullptr;
}

void Parser::test() {
    fprintf(stderr, "输入> ");
    _lexer.getNextToken();
    while (1) {
        fprintf(stderr, "输入> ");
        switch (_lexer.currToken) {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                _lexer.getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

void Parser::HandleDefinition() {
    if (parseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        _lexer.getNextToken();
    }
}

void Parser::HandleExtern() {
    if (parseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        _lexer.getNextToken();
    }
}

void Parser::HandleTopLevelExpression() {
// Evaluate a top-level expression into an anonymous function.
    if (parseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        _lexer.getNextToken();
    }
}
