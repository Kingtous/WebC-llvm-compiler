//
// Created by bytedance on 2021/2/24.
//

#ifndef SYSYPLUS_COMPILER_TIMEANALYSISPASS_H
#define SYSYPLUS_COMPILER_TIMEANALYSISPASS_H

#include "Global.h"

#include "extern/ExternFunctionHandler.h"

struct TimeAnalysisPass : public FunctionPass{
    static char ID;

public:
    TimeAnalysisPass();

    bool runOnFunction(Function &func) override;
};

#endif //SYSYPLUS_COMPILER_TIMEANALYSISPASS_H
