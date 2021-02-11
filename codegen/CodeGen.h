//
// Created by kingtous on 2021/1/31.
//

#ifndef LLVM_KALEIDOSCOPE_CODEGEN_H
#define LLVM_KALEIDOSCOPE_CODEGEN_H

#include <stack>
#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

extern std::unique_ptr<llvm::Module> TheModule;

enum CodeGenBlockContextType {
    NONE, //无
    FOR, //for环境
    IF,  // if环境
    WHILE
};

bool isLoopCodeGenBlockContextType(CodeGenBlockContextType type);

typedef struct ForCodeGenBlockContext {
    BasicBlock *bbStart;
    BasicBlock *bbCond;
    BasicBlock *bbStep;
    BasicBlock *bbBody;
    BasicBlock *bbEndFor;

    ForCodeGenBlockContext(BasicBlock *bbStart, BasicBlock *bbCond, BasicBlock *bbStep, BasicBlock *bbBody,
                           BasicBlock *bbEndFor);

} ForCodeGenBlockContext;

typedef struct IfCodeGenBlockContext {
    BasicBlock *bbCond;
    BasicBlock *bbEnd;
    BasicBlock *bbStep;
    BasicBlock *bbBody;
    BasicBlock *bbEndFor;

    IfCodeGenBlockContext(BasicBlock *bbCond, BasicBlock *bbEnd, BasicBlock *bbStep, BasicBlock *bbBody,
                          BasicBlock *bbEndFor);
} IfCodeGenBlockContext;

typedef struct WhileCodeGenBlockContext {
    BasicBlock *bbCond;
    BasicBlock *bbBody;
    BasicBlock *bbEndWhile;

    WhileCodeGenBlockContext(BasicBlock *bbCond, BasicBlock *bbBody, BasicBlock *bbEndWhile);

} WhileCodeGenBlockContext;

union CodeGenBlockContext {
    ForCodeGenBlockContext *forCodeGenBlockContext;
    IfCodeGenBlockContext *ifCodeGenBlockContext;
    WhileCodeGenBlockContext *whileCodeGenBlockContext;
};
typedef union CodeGenBlockContext CodeGenBlockContext;

class CodeGenBlock {
public:
    BasicBlock *block; // BasicBlock
    std::map<std::string, Value *> localVars; //本地变量
    CodeGenBlockContext context;
    enum CodeGenBlockContextType contextType = CodeGenBlockContextType::NONE; //默认为NONE

    void setContextType(CodeGenBlockContextType contextType);
};

class CodeGenContext {
    std::vector<CodeGenBlock *> blocks;
    Function *function;
public:
    void push_block(BasicBlock *block);

    CodeGenBlock *pop_block();

    CodeGenBlock *get_current_locals();

    void setFunction(Function *func);

    Function *getFunc();

    void removeFunction();

    Value *findValue(std::string &name);

    /**
     * 获取顶部符合type的block
     * @param type
     * @return CodeGenBlock*
     */
    CodeGenBlock *findTopCodeGenBlockTypeBlock(CodeGenBlockContextType type);

    CodeGenBlock *findTopLoopCodeGenBlockTypeBlock();
};

#endif //LLVM_KALEIDOSCOPE_CODEGEN_H
