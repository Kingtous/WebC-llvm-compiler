//
// Created by kingtous on 2021/1/31.
//

#include "CodeGen.h"

void CodeGenContext::push_block(BasicBlock *block) {
    auto bb = new CodeGenBlock();
    bb->block = block;
    this->blocks.push_back(bb);

}

CodeGenBlock *CodeGenContext::pop_block() {
    CodeGenBlock *top = this->blocks[this->blocks.size() - 1];
    this->blocks.pop_back();
    return top;
}

CodeGenBlock *CodeGenContext::get_current_locals() {
    if (blocks.empty()) {
        return nullptr;
    } else {
        return this->blocks[this->blocks.size() - 1];
    }
}

void CodeGenContext::setFunction(Function *func) {
    function = func;
}

Function *CodeGenContext::getFunc() {
    return function;
}

void CodeGenContext::removeFunction() {
    function = nullptr;
}

Value *CodeGenContext::findValue(std::string &name) {
    if (blocks.empty()) {
        return nullptr;
    }
    auto it = blocks.rbegin();
    for (; it != blocks.rend(); it++) {
        auto itt = (*it)->localVars.find(name);
        if (itt != (*it)->localVars.end()){
            return (*itt).second;
        }
    }
    auto gv = TheModule->getGlobalVariable(name);
    return gv;
}
