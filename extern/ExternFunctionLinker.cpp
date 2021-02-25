//
// Created by 金韬 on 2021/2/25.
//

#include "ExternFunctionLinker.h"

Function *ExternFunctionLinker::getExternFunc(LLVMContext& context,Module& module, const std::string &func_name) {
    // 判断是否可用加
    if (func_name == "echo"){
        return ExternFunctionLinker::getOrAddPrintfFunc(context,module);
    } else if (func_name == "time") {
        return ExternFunctionLinker::getOrAddTimeFunc(context, module);
    } else if (func_name == "sleep") {
        return ExternFunctionLinker::getOrAddSleepFunc(context, module);
    }
    // 还未适配
    return NIL;
}

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

Function *ExternFunctionLinker::getOrAddTimeFunc(LLVMContext &context, Module &module) {
    auto funcs = module.functions();
    auto it = funcs.begin();
    for (; it != funcs.end(); it++) {
        if ((*it).getName() == "time") {
            return &(*it);
        }
    }
//    std::vector<Type*> args;
//    args.push_back(Type::getInt64PtrTy(context));
    FunctionType *ty = FunctionType::get(Type::getInt64Ty(context), Type::getInt64PtrTy(context), false);
    auto func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "time", module);
    return func;
}

Function *ExternFunctionLinker::getOrAddSleepFunc(LLVMContext &context, Module &module) {
    auto funcs = module.functions();
    auto it = funcs.begin();
    for (; it != funcs.end(); it++) {
        if ((*it).getName() == "sleep") {
            return &(*it);
        }
    }
//    std::vector<Type*> args;
//    args.push_back(Type::getInt64PtrTy(context));
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), Type::getInt32Ty(context), false);
    auto func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "sleep", module);
    return func;
}
