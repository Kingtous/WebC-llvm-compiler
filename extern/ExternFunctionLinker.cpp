//
// Created by 金韬 on 2021/2/25.
//

#include "ExternFunctionLinker.h"

Function* ExternFunctionLinker::getOrAddPrintfFunc(LLVMContext& context, Module &module) {
    auto funcs = module.functions();
    auto it = funcs.begin();
    for (; it != funcs.end(); it++) {
        if ((*it).getName() == "printf"){
            return &(*it);
        }
    }
    FunctionType* ty = FunctionType::get(Type::getInt32Ty(context),{Type::getInt8PtrTy(context)},true);
    auto func = Function::Create(ty,llvm::GlobalValue::ExternalLinkage,"printf",module);
    return func;
}

Function *ExternFunctionLinker::getExternFunc(LLVMContext& context,Module& module, const std::string &func_name) {
    // 判断是否可用加
    if (func_name == "echo"){
        return ExternFunctionLinker::getOrAddPrintfFunc(context,module);
    }
    return NIL;
}
