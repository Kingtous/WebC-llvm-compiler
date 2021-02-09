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

class CodeGenBlock {
public:
    BasicBlock *block; // BasicBlock
    std::map<std::string, Value *> localVars; //本地变量
};

class CodeGenContext {
    std::vector<CodeGenBlock *> blocks;
    Function *function;
public:
    void push_block(BasicBlock *block);

    CodeGenBlock *pop_block();

    CodeGenBlock *get_current_locals();

    void setFunction(Function* func);
    Function* getFunc();
    void removeFunction();
    Value* findValue(std::string& name);
};

#endif //LLVM_KALEIDOSCOPE_CODEGEN_H
