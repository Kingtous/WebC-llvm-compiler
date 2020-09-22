// AST语法树结点
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_EXPRAST_H
#define LLVM_KALEIDOSCOPE_EXPRAST_H

#include <string>
#include <utility>
#include <vector>

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

/// 数值语法结点
class NumberExprAST : public ExprAST {
    double Val;
public:
    explicit NumberExprAST(double val) : Val(val) {}
};

/// 变量结点
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    explicit VariableExprAST(std::string &Name) : Name(Name) {}
};

/// 二元操作结点
class BinaryExprAST : public ExprAST {
    // 操作符
    char Oper;
    std::unique_ptr<ExprAST> LEA, REA;
public:
    explicit BinaryExprAST(char oper, std::unique_ptr<ExprAST> LEA, std::unique_ptr<ExprAST> REA) : Oper(oper),
                                                                                                    LEA(std::move(LEA)),
                                                                                                    REA(std::move(
                                                                                                            REA)) {}
};

/// 函数调用结点
class CallExprAST : public ExprAST {
    std::string callName;
    std::vector<std::unique_ptr<ExprAST>> args;

public:
    CallExprAST(std::string callName, std::vector<std::unique_ptr<ExprAST>> args) : callName(std::move(callName)),
                                                                                    args(std::move(args)) {};
};

/// 函数描述结点
/// 用于描述函数名字，参数名称，参数个数
class PrototypeAST : public ExprAST {
    std::string name;
    std::vector<std::string> args;

public:
    PrototypeAST(std::string name, std::vector<std::string> args) : name(std::move(name)), args(std::move(args)) {}
};

/// 函数结点
class FunctionAST : public ExprAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)),
                                                                                      Body(std::move(Body)) {}
};

/// 一元结点 - Expression class for a unary operator.
class UnaryExprAST : public ExprAST {
    char Opcode;
    std::unique_ptr<ExprAST> Operand;

public:
    UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}
};


#endif //LLVM_KALEIDOSCOPE_EXPRAST_H
