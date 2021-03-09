//
// Created by bytedance on 2021/3/4.
//

#include "ExternFunctionHandler.h"

#include "ErrHelper.h"

Value *
EchoFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) {
    if (callName == "echo" && argV != NIL) {
        std::string format_str;
        for (auto v : *argV) {
            if (v->getType()->isIntegerTy()) {
                switch (dyn_cast<IntegerType>(v->getType())->getBitWidth()) {
                    case 32 :
                        format_str.append("%d");
                        break;
                    case 64:
                        format_str.append("%ld");
                        break;
                    default:
                        format_str.append("%d");
                }
            } else if (v->getType()->isPointerTy() && v->getType()->getContainedType(0)->isIntegerTy(8)) {
                // 字符数组
                format_str.append("%s");
            } else {
                // not implemented
                return LogErrorV(("打印函数的参数无法理解：" + v->getName()).str().c_str());
            }
            format_str.append(" ");
        }
        format_str.append("\n");
        auto symbol_value = ConstantDataArray::getString(context, format_str);
        auto symbol_mem = Builder.CreateAlloca(symbol_value->getType());
        Builder.CreateStore(symbol_value, symbol_mem);
        auto symbol_pointer = Builder.CreateInBoundsGEP(symbol_mem,
                                                        {ConstantInt::get(Type::getInt32Ty(context), 0),
                                                         ConstantInt::get(Type::getInt32Ty(context), 0)});
        argV->insert(argV->begin(), symbol_pointer);
        return Builder.CreateCall(getOrAddPrintfFunc(context, module), *argV, "echo");
    }
    return NIL;
}

EchoFunctionHandler::EchoFunctionHandler() = default;

int EchoFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

ExternFunctionHandler::ExternFunctionHandler() = default;

bool
ExternFunctionHandler::externFunctionHandlerCompRule(ExternFunctionHandler *handler1, ExternFunctionHandler *handler2) {
    return handler1->getPriority() > handler2->getPriority();
}

int ExternFunctionHandler::getPriority() {
    return MIN_IMPL;
}

Value *SleepFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName,
                                       std::vector<Value *> *argV) {
    if (callName == "sleep") {
        return Builder.CreateCall(getOrAddSleepFunc(context, module), *argV);
    }
    return NIL;
}

SleepFunctionHandler::SleepFunctionHandler() = default;

int SleepFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

Value *
TimeFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) {
    if (callName == "now") {
        auto func = getOrAddTimeFunc(context, module);
        return Builder.CreateCall(func);
    }
    return NIL;
}

TimeFunctionHandler::TimeFunctionHandler() = default;

int TimeFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}


Function *ExternFunctionHandler::getExternFunc(LLVMContext &context, Module &module, const std::string &func_name,
                                               std::vector<Value *> *vector) {
    // 判断是否可用加
    if (func_name == "echo") {
        return ExternFunctionHandler::getOrAddPrintfFunc(context, module);
    } else if (func_name == "time") {
        return ExternFunctionHandler::getOrAddTimeFunc(context, module);
    } else if (func_name == "sleep") {
        return ExternFunctionHandler::getOrAddSleepFunc(context, module);
    }
    // 还未适配
    return NIL;
}

Function *ExternFunctionHandler::getOrAddPrintfFunc(LLVMContext &context, Module &module) {
    auto funcs = module.functions();
    auto it = funcs.begin();
    for (; it != funcs.end(); it++) {
        if ((*it).getName() == "printf") {
            return &(*it);
        }
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), {Type::getInt8PtrTy(context)}, true);
    auto func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "printf", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddTimeFunc(LLVMContext &context, Module &module) {
    auto funcs = module.functions();
    auto it = funcs.begin();
    for (; it != funcs.end(); it++) {
        if ((*it).getName() == "__getms") {
            return &(*it);
        }
    }
//    std::vector<Type*> args;
//    args.push_back(Type::getInt64PtrTy(context));
    FunctionType *ty = FunctionType::get(Type::getInt64Ty(context), false);
    auto func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "__getms", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddSleepFunc(LLVMContext &context, Module &module) {
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

Function *ExternFunctionHandler::getOrAddGetSocketFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_getSocket");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_getSocket", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddConnectSocketFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_connectSocket");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context),
                                         {Type::getInt32Ty(context), Type::getInt8Ty(context)->getPointerTo(),
                                          Type::getInt8Ty(context)->getPointerTo()}, false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_connectSocket", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddCloseSocketFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_closeSocket");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), Type::getInt32Ty(context), false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_closeSocket", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddIsSocketConnectedFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_isSocketConnected");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), Type::getInt32Ty(context), false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_isSocketConnected", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddGetRequestFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_callGetRequest");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt8Ty(context)->getPointerTo(), {Type::getInt32Ty(context),
                                                                     Type::getInt8Ty(context)->getPointerTo(),
                                                                     Type::getInt8Ty(context)->getPointerTo()}, false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_callGetRequest", module);
    return func;
}

Value *
WebFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) {
    // 处理 {@link module/web/web.h}，注意名字要保持一致
    if (callName == "getSocket" && argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddGetSocketFunc(context, module);
        return Builder.CreateCall(func);
    } else if (callName == "connectSocket" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddConnectSocketFunc(context, module);
        return Builder.CreateCall(func, *argV);
    } else if (callName == "closeSocket" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddCloseSocketFunc(context, module);
        return Builder.CreateCall(func, *argV);
    } else if (callName == "getRequest" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddGetRequestFunc(context, module);
        return Builder.CreateCall(func, *argV);
    } else if (callName == "isSocketConnected" && !argV->empty()){
        auto func = ExternFunctionHandler::getOrAddIsSocketConnectedFunc(context,module);
        return Builder.CreateCall(func,*argV);
    }
    return NIL;
}

int WebFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

WebFunctionHandler::WebFunctionHandler() {
}