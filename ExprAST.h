// AST语法树结点
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_EXPRAST_H
#define LLVM_KALEIDOSCOPE_EXPRAST_H

#include <string>
#include <utility>
#include <vector>

#include "global.h"


/// 基结点
class ExprAST {
public:
    virtual ~ExprAST() = default;

    virtual llvm::Value *codegen() = 0;
};

/// 数值语法结点
class NumberExprAST : public ExprAST {
    double Val;
public:
    explicit NumberExprAST(double val) : Val(val) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

/// 变量结点
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    explicit VariableExprAST(std::string &Name) : Name(Name) {}

    llvm::Value *codegen() override;
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

    llvm::Value *codegen() override;
};

/// 函数调用结点
class CallExprAST : public ExprAST {
    std::string callName;
    std::vector<std::unique_ptr<ExprAST>> args;

public:
    CallExprAST(std::string callName, std::vector<std::unique_ptr<ExprAST>> args) : callName(std::move(callName)),
                                                                                    args(std::move(args)) {};

    llvm::Value *codegen() override;
};

/// 函数描述结点
/// 用于描述函数名字，参数名称，参数个数
class PrototypeAST : public ExprAST {
    std::string name;
    std::vector<std::string> args;

public:
    PrototypeAST(std::string name, std::vector<std::string> args) : name(std::move(name)), args(std::move(args)) {}

    llvm::Function *codegen() override;

    const std::string &getName() const;
};

/// 函数结点
class FunctionAST : public ExprAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)),
                                                                                      Body(std::move(Body)) {}

    llvm::Function *codegen() override;
};

/// 一元结点 - Expression class for a unary operator.
/// 从JIT弄来的代码，目前不需要
class UnaryExprAST : public ExprAST {
    char Opcode;
    std::unique_ptr<ExprAST> Operand;

public:
    UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}
};


#endif //LLVM_KALEIDOSCOPE_EXPRAST_H
