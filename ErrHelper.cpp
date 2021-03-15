//
// Created by 金韬 on 2020/9/22.
//

#include "ErrHelper.h"

// CGUI
#ifdef CGUI

extern void logOnUi(const char *Str);

#endif

std::unique_ptr<NodeAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
#ifdef CGUI
    logOnUi(Str);
#endif
    return nullptr;
}

std::unique_ptr<NodeAST> LogInfo(const char *Str) {
    fprintf(stdout, "Info: %s\n", Str);
#ifdef CGUI
    logOnUi(Str);
#endif
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
