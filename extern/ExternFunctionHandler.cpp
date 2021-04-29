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
        auto symbol_mem = Builder->CreateAlloca(symbol_value->getType());
        Builder->CreateStore(symbol_value, symbol_mem);
        auto symbol_pointer = Builder->CreateInBoundsGEP(symbol_mem,
                                                         {ConstantInt::get(Type::getInt32Ty(context), 0),
                                                          ConstantInt::get(Type::getInt32Ty(context), 0)});
        argV->insert(argV->begin(), symbol_pointer);
        return Builder->CreateCall(getOrAddPrintfFunc(context, module), *argV, "echo");
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
        return Builder->CreateCall(getOrAddSleepFunc(context, module), *argV);
    }
    return NIL;
}

SleepFunctionHandler::SleepFunctionHandler() = default;

int SleepFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

int KsqlFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

Value *
TimeFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) {
    if (callName == "now") {
        auto func = getOrAddTimeFunc(context, module);
        return Builder->CreateCall(func);
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
                                                                                    Type::getInt8Ty(
                                                                                            context)->getPointerTo(),
                                                                                    Type::getInt8Ty(
                                                                                            context)->getPointerTo()},
                                         false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_callGetRequest", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddPostRequestFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_callPostRequest");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt8Ty(context)->getPointerTo(), {Type::getInt32Ty(context),
                                                                                    Type::getInt8Ty(
                                                                                            context)->getPointerTo(),
                                                                                    Type::getInt8Ty(
                                                                                            context)->getPointerTo(),
                                                                                    Type::getInt8Ty(
                                                                                            context)->getPointerTo()},
                                         false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_callPostRequest", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddGetServerFunc(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_getServerId");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context),
                                         {Type::getInt8PtrTy(context),
                                          Type::getInt32Ty(context),
                                          Type::getInt32Ty(context)}, false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_getServerId", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddUrlHandler(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_addUrlHandler");
    if (func != NIL) {
        return func;
    }
    auto func_param_type = FunctionType::get(Type::getInt8PtrTy(context), false);

    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context),
                                         {Type::getInt32Ty(context),
                                          Type::getInt8Ty(context)->getPointerTo(),
                                          Type::getInt8Ty(context)->getPointerTo(),
                                          Type::getInt8Ty(context)->getPointerTo(),
                                          func_param_type->getPointerTo()}, false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_addUrlHandler", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddStartServer(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_web_startServe");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context),
                                         Type::getInt32Ty(context), false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_web_startServe", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddtoString(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("toString");
    if (func != NIL) {
        return func;
    }
    // const char* func(void* a,int type) void* 也为i8*
    FunctionType *ty = FunctionType::get(Type::getInt8PtrTy(context),
                                         {Type::getInt8PtrTy(context),
                                          Type::getInt32Ty(context)}, false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "toString", module);
    return func;
}

Function *ExternFunctionHandler::getOrAddConnectDB(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_ksql_connect_db");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), {Type::getInt8Ty(context)->getPointerTo(),
                                                                     Type::getInt8Ty(
                                                                             context)->getPointerTo(),
                                                                     Type::getInt8Ty(
                                                                             context)->getPointerTo(),
                                                                     Type::getInt8Ty(
                                                                             context)->getPointerTo()
                                         },
                                         false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_ksql_connect_db", module);
    return func;
}

//Function *ExternFunctionHandler::getOrAddFreeConnect(LLVMContext &context, Module &module){
//    Function *func = module.getFunction("_free_connect");
//    if (func != NIL){
//        return func;
//    }
//    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context),false);
//    func = Function::Create(ty,llvm::GlobalValue::ExternalLinkage,"_free_connect",module);
//    return func;
//}
Function *ExternFunctionHandler::getOrAddQueryDB(LLVMContext &context, Module &module) {
    Function *func = module.getFunction("_ksql_query_db");
    if (func != NIL) {
        return func;
    }
    FunctionType *ty = FunctionType::get(Type::getInt8Ty(context)->getPointerTo(),
                                         {Type::getInt8Ty(context)->getPointerTo()},
                                         false);
    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_ksql_query_db", module);
    return func;
}

//Function *ExternFunctionHandler::getOrAddPrintJson(LLVMContext &context, Module &module){
//    Function *func = module.getFunction("_print_json");
//    if (func != NIL) {
//        return func;
//    }
//    FunctionType *ty = FunctionType::get(Type::getInt32Ty(context), {Type::getInt32Ty(context)->getPointerTo()},
//                                         false);
//    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_print_json", module);
//    return func;
//}
//Function *ExternFunctionHandler::getOrAddResToJson(LLVMContext &context, Module &module){
//    Function *func = module.getFunction("_resToJson");
//    if (func != NIL) {
//        return func;
//    }
//    FunctionType *ty = FunctionType::get(Type::getInt8Ty(context)->getPointerTo(), {Type::getInt32Ty(
//                                                                                                context)->getPointerTo()},
//                                         false);
//    func = Function::Create(ty, llvm::GlobalValue::ExternalLinkage, "_resToJson", module);
//    return func;
//}
Value *
WebFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) {
    // 处理 {@link module/web/web.h}，注意名字要保持一致
    if (callName == "getSocket" && argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddGetSocketFunc(context, module);
        return Builder->CreateCall(func);
    } else if (callName == "connectSocket" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddConnectSocketFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "closeSocket" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddCloseSocketFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "getRequest" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddGetRequestFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "isSocketConnected" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddIsSocketConnectedFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "postRequest" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddPostRequestFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "getServer" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddGetServerFunc(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "addUrlHandler" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddUrlHandler(context, module);
        return Builder->CreateCall(func, *argV);
    } else if (callName == "startServer" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddStartServer(context, module);
        return Builder->CreateCall(func, *argV);
    }
    return NIL;
}

int WebFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

WebFunctionHandler::WebFunctionHandler() {
}

KStringFunctionHandler::KStringFunctionHandler() {

}

KsqlFunctionHandler::KsqlFunctionHandler() {

}

int KStringFunctionHandler::getPriority() {
    return INTERNAL_IMPL;
}

/// 从kstring.h中拷贝过来的，注意保持一致
#define SYSYTYPE_INT 0
#define SYSYTYPE_LONG 1
#define SYSYTYPE_DOUBLE 2

Value *KStringFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName,
                                         std::vector<Value *> *argV) {
    if (callName == "toStr" && !argV->empty()) {
        if (argV->size() > 1) {
            LogWarn("toStr参数超过1个，目前只识别第一个");
            argV->erase(argV->begin() + 1, argV->end());
        }
        auto arg = argV->at(0);
        if (arg->getType()->isIntegerTy(32)) {
            // int
            argV->push_back(ConstantInt::get(getTypeFromStr("int"), SYSYTYPE_INT));
        } else if (arg->getType()->isIntegerTy(64)) {
            // long
            argV->push_back(ConstantInt::get(getTypeFromStr("int"), SYSYTYPE_LONG));
        } else if (arg->getType()->isDoubleTy()) {
            // double
            argV->push_back(ConstantInt::get(getTypeFromStr("int"), SYSYTYPE_DOUBLE));
        } else {
            return NIL;
        }
        auto arg_mem = Builder->CreateAlloca(arg->getType());
        Builder->CreateStore(arg, arg_mem);
        arg = Builder->CreateBitCast(arg_mem, Type::getInt8PtrTy(context));
        argV->erase(argV->begin());
        argV->insert(argV->begin(), arg);
        auto func = ExternFunctionHandler::getOrAddtoString(context, module);
        return Builder->CreateCall(func, *argV);
    }
    return NIL;
}

Value *KsqlFunctionHandler::tryhandle(LLVMContext &context, Module &module, std::string callName,
                                      std::vector<Value *> *argV) {
    if (callName == "connect_db" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddConnectDB(context, module);
        return Builder->CreateCall(func, *argV);
    }
//    else if(callName=="free_connect"&&argV->empty()){
//        auto func = ExternFunctionHandler::getOrAddFreeConnect(context,module);
//        return Builder->CreateCall(func);
//    }
    else if (callName == "query_db" && !argV->empty()) {
        auto func = ExternFunctionHandler::getOrAddQueryDB(context, module);
        return Builder->CreateCall(func, *argV);
    }
//    else if (callName=="resToJson"&&!argV->empty()){
//        auto func = ExternFunctionHandler::getOrAddResToJson(context,module);
//        return Builder->CreateCall(func,*argV);
//    }
}
