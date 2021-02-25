//
// Created by bytedance on 2021/2/25.
//

#ifndef SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H
#define SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H

#include "Global.h"

class ExternFunctionLinker {
public:
    static Function *getOrAddPrintfFunc(LLVMContext &context, Module &module);

    static Function *getOrAddTimeFunc(LLVMContext &context, Module &module);

    static Function *getOrAddSleepFunc(LLVMContext &context, Module &module);

    static Function *getExternFunc(LLVMContext &context, Module &module, const std::string &func_name);
};


#endif //SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H
