//
// Created by 金韬 on 2020/9/21.
//

#include "ExprAST.h"
#include "ErrHelper.h"


llvm::Value *NumberExprAST::codegen() {
    // 在LLVM IR中，数字常量用ConstantFP类表示 ，它在APFloat 内部保存数值（APFloat能够保持任意精度的浮点常量）
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}

llvm::Value *VariableExprAST::codegen() {
    // 从符号表中取出
    llvm::Value *v = NamedValues[Name];
    if (!v)
        LogErrorV((std::string("Unknown Variable Name:") + Name).c_str());
    return v;
}

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = LEA->codegen();
    llvm::Value *R = REA->codegen();
    if (!L || !R)
        return nullptr;

    switch (Oper) {
        case '+':
            return Builder.CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder.CreateFSub(L, R, "subtmp");
        case '*':
            return Builder.CreateFMul(L, R, "multmp");
        case '/':
            return Builder.CreateFDiv(L, R, "divtmp");
        case '<':
            L = Builder.CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext),
                                        "booltmp");
        default:
            return LogErrorV("invalid binary operator");
    }
}

llvm::Value *CallExprAST::codegen() {
    // Look up the name in the global module table.
    llvm::Function *CalleeF = TheModule->getFunction(callName);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value *> argsV;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        argsV.push_back(args[i]->codegen());
        if (!argsV.back())
            return nullptr;
    }

    return Builder.CreateCall(CalleeF, argsV, "calltmp");
}

llvm::Function *PrototypeAST::codegen() {
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type *> Doubles(args.size(),
                                      llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *FT =
            llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), Doubles, false);

    llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(args[Idx++]);

    return F;
}

llvm::Function *ConditionAST::codegen() {
    // Make the function type:  double(double,double) etc.
    llvm::FunctionType *FT =
            llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), Doubles, false);

    llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(args[Idx++]);

    return F;
}


const std::string &PrototypeAST::getName() const {
    return name;
}

llvm::Function *FunctionAST::codegen() {
    llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;
    // 如果函数不为空，则表示已定义过了
    if (!TheFunction->empty())
        return (llvm::Function *) LogErrorV("Function cannot be redefined.");

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "jintao_entry", TheFunction);
    Builder.SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[Arg.getName()] = &Arg;

    if (llvm::Value *RetVal = Body->codegen()) {
        // Finish off the function.
        Builder.CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}
