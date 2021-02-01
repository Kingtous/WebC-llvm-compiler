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

class CodeGenBlock {
public:
    BasicBlock *block; // BasicBlock
    std::map<std::string, Value *> localVars; //本地变量
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;
    Function *function;
public:
    void push_block(BasicBlock *block);

    CodeGenBlock *pop_block();

    CodeGenBlock *get_current_locals();
};

#endif //LLVM_KALEIDOSCOPE_CODEGEN_H
