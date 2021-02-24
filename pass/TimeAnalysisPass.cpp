//
// Created by bytedance on 2021/2/24.
//

#include "TimeAnalysisPass.h"

char TimeAnalysisPass::ID = 0;

bool TimeAnalysisPass::runOnFunction(Function &func) {
    // TODO
    return false;
}

TimeAnalysisPass::TimeAnalysisPass() : FunctionPass(ID) {
}
