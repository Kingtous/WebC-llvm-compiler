//
// Created by 金韬 on 2021/3/4.
//

#ifndef SYSYPLUS_COMPILER_EXTERNFUNCTIONHANDLER_H
#define SYSYPLUS_COMPILER_EXTERNFUNCTIONHANDLER_H
#include <cmath>

#include "Global.h"

#define INTERNAL_IMPL 10
#define USER_IMPL 100

#define MIN_IMPL 0
#define MAX_PRI 100

using namespace llvm;

class ExternFunctionHandler {
private:
public:
    ExternFunctionHandler();

    virtual Value* tryhandle(LLVMContext& context,
                          Module& module,std::string callName,std::vector<Value*>* argV) = 0;

    static bool externFunctionHandlerCompRule(ExternFunctionHandler* handler1,ExternFunctionHandler* handler2);

    virtual int getPriority();

    /**
     * 获取func
     * @param context
     * @param module
     * @param func_name
     * @param vector
     * @return
     */
    static Function *getExternFunc(LLVMContext &context, Module &module, const std::string &func_name,
                                   std::vector<Value *>* vector);

    static Function *getOrAddPrintfFunc(LLVMContext &context, Module &module);

    static Function *getOrAddTimeFunc(LLVMContext &context, Module &module);

    static Function *getOrAddSleepFunc(LLVMContext &context, Module &module);

    static Function *getOrAddGetSocketFunc(LLVMContext &context, Module &module);

    static Function *getOrAddConnectSocketFunc(LLVMContext &context, Module &module);

    static Function *getOrAddCloseSocketFunc(LLVMContext &context, Module &module);

    static Function *getOrAddIsSocketConnectedFunc(LLVMContext &context, Module &module);

    static Function *getOrAddGetRequestFunc(LLVMContext &context, Module &module);

    static Function *getOrAddPostRequestFunc(LLVMContext &context, Module &module);

    static Function *getOrAddGetServerFunc(LLVMContext &context, Module &module);

    static Function *getOrAddUrlHandler(LLVMContext &context, Module &module);

    static Function *getOrAddStartServer(LLVMContext &context, Module &module);
};


class EchoFunctionHandler : public ExternFunctionHandler {

public:
    EchoFunctionHandler();

    Value* tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) override;

    int getPriority() override;
};

class TimeFunctionHandler : public ExternFunctionHandler {
public:
    int getPriority() override;

    TimeFunctionHandler();

    Value* tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) override;
};

class SleepFunctionHandler : public ExternFunctionHandler {
public:
    int getPriority() override;

    SleepFunctionHandler();

    Value* tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) override;
};

class WebFunctionHandler : public ExternFunctionHandler{
public:

    WebFunctionHandler();

    Value *tryhandle(LLVMContext &context, Module &module, std::string callName, std::vector<Value *> *argV) override;

    int getPriority() override;
};

#endif //SYSYPLUS_COMPILER_EXTERNFUNCTIONHANDLER_H
