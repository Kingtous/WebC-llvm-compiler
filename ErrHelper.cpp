//
// Created by 金韬 on 2020/9/22.
//

#include "ErrHelper.h"

std::unique_ptr<NodeAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

llvm::Value *LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}

llvm::Value *LogWarn(const char *Str) {
    LogError(Str);
    return nullptr;
}

llvm::Type *LogErrorTy(const char *Str) {
    LogError(Str);
    return nullptr;
}
