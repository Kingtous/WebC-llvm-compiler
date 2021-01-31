//
// Created by kingtous on 2021/1/31.
//

#include "CodeGen.h"

void CodeGenContext::push_block(BasicBlock *block) {
    auto bb = new CodeGenBlock();
    bb->block = block;
    this->blocks.push(bb);

}

CodeGenBlock *CodeGenContext::pop_block() {
    CodeGenBlock *top = this->blocks.top();
    this->blocks.pop();
    return top;
}

CodeGenBlock *CodeGenContext::get_current_locals() {
    if (blocks.empty()) {
        return nullptr;
    } else {
        return blocks.top();
    }
}
