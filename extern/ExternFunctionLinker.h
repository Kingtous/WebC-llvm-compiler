//
// Created by bytedance on 2021/2/25.
//

#ifndef SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H
#define SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H

#include <algorithm>

#include "Global.h"
#include "ast/NodeAST.hpp"
#include "ExternFunctionHandler.h"

class ExternFunctionLinker {
    static std::vector<ExternFunctionHandler*> handlers;
public:

    /**
     * 注册外部函数Handler
     * @param handler
     */
    static void registerHandler(ExternFunctionHandler* handler);

    /**
     * 尝试处理外部函数调用
     * @param context
     * @param module
     * @param func_name
     * @param vector
     * @return 成功返回non-nullptr
     */
    static Value* tryHandleFuncCall(LLVMContext &context, Module &module, const std::string &func_name,
                                  std::vector<Value *>* vector);
};

#endif //SYSYPLUS_COMPILER_EXTERNFUNCTIONLINKER_H
