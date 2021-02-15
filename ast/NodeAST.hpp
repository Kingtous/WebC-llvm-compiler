// AST语法树结点
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_NODEAST_H
#define LLVM_KALEIDOSCOPE_NODEAST_H

#include <string>
#include <utility>
#include <vector>
#include <typeinfo>

#include "Global.h"

using namespace std;
using namespace llvm;

enum ASTType {
    TopAST = 0,
};

enum VarType {
    INT,
    VOID
};

enum BinaryType {
    add,
    sub,
    mul,
    divi,
    mod,
    less,
    less_equ,
    greater,
    greater_equ,
    equ,
    AND,
    OR
};

//////////////////// 基类结点 ///////////////////////

/// 基结点
class NodeAST {
public:

    virtual ~NodeAST() = default;

    virtual llvm::Value *codegen() = 0;
};

/// 表达式，如数字、代码块、赋值语句、二元操作语句
class ExpressionAST : public NodeAST {
};

/// 声明，如变量声明、函数声明
class StatementAST : public NodeAST {
};

class ExpressionStatementAST : public NodeAST {
    ExpressionAST *expr;
public:
    ExpressionStatementAST(ExpressionAST *expr);

    Value *codegen() override;
};

class ReturnStmtAST : public StatementAST {
    ExpressionAST *expr; // 可以为null
public:
    ReturnStmtAST();

    explicit ReturnStmtAST(ExpressionAST *expr);

    llvm::Value *codegen() override;
};

/// 块结点，一个块含有多种声明，也可作为一个程序的入口点
class BlockAST : public NodeAST {
public:
    vector<StatementAST *> statements;

    Value *codegen() override;
};

//////////////////// 用于存储数字的结点 ///////////////////////

// 标识符
class IdentifierExprAST : public ExpressionAST {
public:
    std::string identifier;

    IdentifierExprAST();

    explicit IdentifierExprAST(std::string identifier) : identifier(std::move(identifier)) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

// 标识符
class IdentifierArrExprAST : public IdentifierExprAST {
public:
    vector<ExpressionAST *> *arrIndex;

    IdentifierArrExprAST(const string &identifier, vector<ExpressionAST *> *arrIndex);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

// 变量声明语句
class VariableDeclarationAST : public StatementAST {
public:
    // 变量类型
    std::string type;
    // 变量名
    IdentifierExprAST *identifier;
    // 可能有赋值
    ExpressionAST *expr;
    bool isConst = false;

    void setIsConst(bool isConst);

    VariableDeclarationAST() = default;

    VariableDeclarationAST(const string &type, IdentifierExprAST *identifier, ExpressionAST *expr = nullptr,
                           bool isConst = false);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

// 变量声明语句
class VariableArrDeclarationAST : public VariableDeclarationAST {
public:
    // 变量类型
    std::string type;
    // 变量名
    IdentifierArrExprAST *identifier;
    // 可能有赋值
    vector<NodeAST *> *exprs;
    bool isConst = false;

    VariableArrDeclarationAST(const string &type, IdentifierArrExprAST *identifier, vector<NodeAST *> *exprs = NIL,
                              bool isConst = false);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;

    /**
     * 生成数组expressions
     * @return llvm::ConstantArray*
     */
    [[nodiscard]] vector<Constant *> *genExprs() const;
};

// 变量赋值句
class VariableAssignmentAST : public StatementAST {
public:
    // 变量名
    std::string identifier;
    // 可能有赋值
    ExpressionAST *expr;

    VariableAssignmentAST(const string &identifier, ExpressionAST *expr);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

class VariableArrAssignmentAST : public StatementAST {
public:
    // 变量名
    IdentifierArrExprAST *identifier;
    // 可能有赋值
    ExpressionAST *expr;

    VariableArrAssignmentAST(IdentifierArrExprAST *identifier, ExpressionAST *expr);

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};


/// 数值语法结点
class DoubleExprAST : public ExpressionAST {
public:
    double Val;

    explicit DoubleExprAST(double val) : Val(val) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

/// 数值语法结点
class IntegerExprAST : public ExpressionAST {
public:
    int Val;

    IntegerExprAST(int val) : Val(val) {}

    /// codegen（）方法表示为该AST节点发出IR及其依赖的所有内容，并且它们都返回一个LLVM Value对象。
    /// “Value”是用于表示 LLVM中的“ 静态单一分配（SSA）寄存器”或“SSA值”的类。
    /// SSA值的最独特之处在于它们的值是在相关指令执行时计算的，并且在指令重新执行之前（以及如果）它不会获得新值。
    /// 换句话说，没有办法“改变”SSA值。
    llvm::Value *codegen() override;
};

/// 变量结点
class VariableExprAST : public NodeAST {
    std::string Name;
public:
    explicit VariableExprAST(std::string &Name) : Name(Name) {}

    llvm::Value *codegen() override;
};

/// 二元操作结点
class BinaryExprAST : public ExpressionAST {
    // 操作符
    BinaryType type;
    ExpressionAST *LEA;
    ExpressionAST *REA;
public:
    BinaryExprAST(BinaryType type, ExpressionAST *lea, ExpressionAST *rea);

    llvm::Value *codegen() override;

    Value *codeGenAnd(NodeAST *l, NodeAST *r);

    Value *codeGenOr(NodeAST *l, NodeAST *r);
};

/// 函数调用结点
class CallExprAST : public ExpressionAST {
    std::string callName;
    std::vector<ExpressionAST*> args;

public:
    CallExprAST(std::string callName, std::vector<ExpressionAST*> args) : callName(std::move(callName)),
                                                                                    args(std::move(args)) {};

    llvm::Value *codegen() override;
};

/// 函数描述结点
/// 用于描述函数名字，参数名称，参数个数
class PrototypeAST : public NodeAST {
    std::string returnType;
    std::string name;
    std::vector<VariableDeclarationAST*> args;

public:
    PrototypeAST(const string &returnType, const string &name, const vector<VariableDeclarationAST *> &args);

    llvm::Function *codegen() override;

    const std::string &getName() const;
};

/// 函数结点
class FunctionAST : public StatementAST {
    PrototypeAST *Proto;
    BlockAST *Body;
public:
    FunctionAST(PrototypeAST *proto, BlockAST *body);

    /**
     * 清除CodeGenContext
     */
    void cleanCodeGenContext();

    llvm::Function *codegen() override;
};

/// Break结点
class BreakStmtAST : public StatementAST {
public:
    BreakStmtAST();

    Value *codegen() override;
};

/// Break结点
class ContinueStmtAST : public StatementAST {
public:
    ContinueStmtAST();

    Value *codegen() override;
};


/// 条件结点 TODO 加入一个exit，可以退出条件结点
class ConditionAST : public StatementAST {
    ExpressionAST *if_cond;
    BlockAST *if_stmt;
    BlockAST *else_stmt;
public:

    ConditionAST(ExpressionAST *ifCond, BlockAST *ifStmt, BlockAST *elseStmt);

    llvm::Value *codegen() override;
};

/// For结点
class ForExprAST : public StatementAST {
    // 意思为：for (Start,End,Step){Body}
    NodeAST *Start, *Cond, *Step;
    BlockAST *Body;
    // start -> End? -> Body ->

public:
    ForExprAST(NodeAST *start, NodeAST *end, NodeAST *step, BlockAST *body);

    llvm::Value *codegen() override;
};

/// While结点
class WhileStmtAST : public StatementAST {
    // 意思为：for (Start,End,Step){Body}
    NodeAST *Cond;
    BlockAST *Body;

public:
    WhileStmtAST(NodeAST *cond, BlockAST *body);

    llvm::Value *codegen() override;
};

Type *getTypeFromStr(const std::string &type);

ArrayType *buildArrayType(vector<ExpressionAST *> *v, Type *type);

/**
 * 获取数组（支持多维）
 * @param type 数组类型
 * @return 数组元素类型
 */
Type *getArrayElemType(Value *type);

Type *getArrayType(Value *value);

#endif //LLVM_KALEIDOSCOPE_NODEAST_H