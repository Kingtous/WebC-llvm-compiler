//
// Created by 金韬 on 2021/2/25.
//

#include "ExternFunctionLinker.h"

std::vector<ExternFunctionHandler*> ExternFunctionLinker::handlers;

Value* ExternFunctionLinker::tryHandleFuncCall(LLVMContext &context, Module &module, const string &func_name,
                                            std::vector<Value *> *vector) {
    for (ExternFunctionHandler* handler : handlers) {
        auto v = handler->tryhandle(context,module,func_name,vector);
        if (v != NIL){
            return v;
        }
    }
    return NIL;
}

void ExternFunctionLinker::registerHandler(ExternFunctionHandler *handler) {
    if (handler != NIL && std::find(handlers.begin(),handlers.end(),handler) == handlers.end()) {
        handlers.push_back(handler);
        std::sort(handlers.begin(), handlers.end(), ExternFunctionHandler::externFunctionHandlerCompRule);
    }
}
