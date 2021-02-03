//
// Created by 金韬 on 2020/9/21.
//

#include "NodeAST.h"

#include <utility>
#include "ErrHelper.h"

llvm::Value *DoubleExprAST::codegen() {
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
    if (L->getType() == getTypeFromStr("int") && R->getType() == getTypeFromStr("int")) {
        switch (type) {
            case BinaryType::add:
                return Builder.CreateAdd(L, R, "add");
            case BinaryType::sub:
                return Builder.CreateSub(L, R, "sub");
            case BinaryType::mul:
                return Builder.CreateMul(L, R, "mul");
            case BinaryType::div:
                return Builder.CreateSDiv(L, R, "div");
            case BinaryType::mod:
                return Builder.CreateSRem(L, R,
                                          "mod");
            default:
                return LogErrorV(("invalid binary operator"));
        }
    } else {
        // 类型不相同，做类型转换
        if (L->getType()->isIntegerTy()) {
            L = Builder.CreateSIToFP(L, R->getType());
        }
        if (R->getType()->isIntegerTy()) {
            R = Builder.CreateSIToFP(R, L->getType());
        }
        switch (type) {
            case BinaryType::add:
                return Builder.CreateFAdd(L, R, "add");
            case BinaryType::sub:
                return Builder.CreateFSub(L, R, "sub");
            case BinaryType::mul:
                return Builder.CreateFMul(L, R, "mul");
            case BinaryType::div:
                return Builder.CreateFDiv(L, R, "div");
            case BinaryType::mod:
                return Builder.CreateFRem(L, R, "mod");
            default:
                return LogErrorV(("invalid binary operator"));
        }
    }

}

BinaryExprAST::BinaryExprAST(BinaryType type, ExpressionAST *lea, ExpressionAST *&rea) : type(type), LEA(lea),
                                                                                         REA(rea) {
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
    std::vector<llvm::Type *> Args;
    for (auto arg : args) {
        Args.push_back(getTypeFromStr(arg->type));
    }
    llvm::FunctionType *FT = llvm::FunctionType::get(getTypeFromStr(returnType), Args, false);;
    llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        Arg.setName(args[Idx++]->identifier);
    }
    return F;
}

Value *ConditionAST::codegen() {
    // 解析if表达式
    Value *ifCondV = if_cond->codegen();
    if (!ifCondV) {
        return LogErrorV("if parse error");
    }
    // 创建一条 icnp ne指令，用于if的比较，由于是bool比较，此处直接与0比较。
    ifCondV = Builder.CreateICmpNE(ifCondV, ConstantFP::get(TheContext, APFloat(0.0)), "neuq_jintao_ifcond");
    // 获取if里面的函数体，准备创建基础块
    auto theFunction = Builder.GetInsertBlock()->getParent();
    if (!theFunction) {
        return LogErrorV("if没在函数体内使用");
    }
    // 创建三个basic block，先添加IfTrue至函数体里面
    BasicBlock *bbIfTrue = BasicBlock::Create(TheContext, "neuq_jintao_if_true", theFunction);
    BasicBlock *bbElse = BasicBlock::Create(TheContext, "neuq_jintao_else");
    BasicBlock *bbIfEnd = BasicBlock::Create(TheContext, "neuq_jintao_if_end");
    // 创建条件分支,createCondBr(条件结果，为true的basic block，为false的basic block)
    Builder.CreateCondBr(ifCondV, bbIfTrue, bbElse);
    // 开启往Basic Block中塞入代码
    Builder.SetInsertPoint(bbIfTrue);
    Value *ifTrueV = if_stmt->codegen();
    if (!ifTrueV) {
        return LogErrorV("if里面的内容有误");
    }
    // if 中为true的代码写完了，将if跳转到else后面，也就是IfEnd结点。这符合逻辑，同时也是LLVM的强制要求，每个Basic Block必须以跳转结尾
    Builder.CreateBr(bbIfEnd);
    // 将插入if内容时，可能会有循环嵌套，需要更新嵌套最底层的insertBlock
    bbIfTrue = Builder.GetInsertBlock();
    // 塞入else
    theFunction->getBasicBlockList().push_back(bbElse);
    Builder.SetInsertPoint(bbElse);
    Value *elseV = else_stmt->codegen();
    if (!elseV) {
        return LogErrorV("else里面内容有误");
    }
    Builder.CreateBr(bbIfEnd);
    bbElse = Builder.GetInsertBlock();
    // 创建末尾结点
    theFunction->getBasicBlockList().push_back(bbIfEnd);
    Builder.SetInsertPoint(bbIfEnd);
    PHINode *pn = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, "neuq_jintao_iftmp");
    pn->addIncoming(ifTrueV, bbIfTrue);
    pn->addIncoming(elseV, bbElse);
    return pn;
}

Value *ForExprAST::codegen() {
    // 先执行for(xxx;;)
    Value *StartV = Start->codegen();
    if (!StartV) {
        return LogErrorV("for的初始语句有误");
    }
    Function *theFuntion = Builder.GetInsertBlock()->getParent();
    // 拿到当前Start生成的那个BasicBlock
    BasicBlock *bbStart = Builder.GetInsertBlock();
    // 在当前的函数末尾创建个循环体
    BasicBlock *bbLoop = BasicBlock::Create(TheContext, "kingtous_loop", theFuntion);
    Builder.CreateBr(bbLoop);
    // insert code into bbLoop
    Builder.SetInsertPoint(bbLoop);
    PHINode *phi = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, VarName.c_str());
    phi->addIncoming(StartV, bbStart);
    // SSA特性+防止shadow
    Value *oldV = NamedValues[VarName];
    NamedValues[VarName] = phi;
    if (!Body->codegen()) {
        return nullptr;
    }

    // 开始解析for(;;xxx)
    Value *StepV = nullptr;
    if (Step) {
        StepV = Step->codegen();
        if (!StepV) {
            return LogErrorV("for(xx;xx;here) 解析错误");
        }
    }

    Value *endV = End->codegen();
    if (!endV) {
        return LogErrorV("解析for终止条件失败");
    }
    endV = Builder.CreateFCmpONE(endV, ConstantFP::get(TheContext, APFloat(0.0)), "jintao_compare");
    BasicBlock *bbEnd = Builder.GetInsertBlock();

    BasicBlock *bbAfterFor = BasicBlock::Create(TheContext, "after_for", theFuntion);
    Builder.CreateCondBr(endV, bbLoop, bbAfterFor);
    Builder.SetInsertPoint(bbEnd);

    phi->addIncoming(StepV, bbEnd);
    if (oldV) {
        NamedValues[VarName] = oldV;
    } else {
        NamedValues.erase(VarName);
    }
    // 对于循环，永远都返回0.0
    return Constant::getNullValue(Type::getDoublePtrTy(TheContext));
}


const std::string &PrototypeAST::getName() const {
    return name;
}

PrototypeAST::PrototypeAST(const string &returnType, const string &name, const vector<VariableDeclarationAST *> &args)
        : returnType(returnType), name(name), args(args) {}

llvm::Function *FunctionAST::codegen() {
    llvm::Function *function = TheModule->getFunction(Proto->getName());

    if (!function)
        function = Proto->codegen();

    if (!function)
        return nullptr;
    // 如果函数不为空，则表示已定义过了
    if (!function->empty())
        return (llvm::Function *) LogErrorV("Function cannot be redefined.");

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "jintao_entry", function);
    Builder.SetInsertPoint(BB);
    TheCodeGenContext.push_block(BB);
    TheCodeGenContext.setFunction(function);

    // Record the function arguments in the NamedValues map.
//    auto funcVars = TheCodeGenContext.get_current_locals()->localVars;
    for (auto &Arg : function->args()){
        TheCodeGenContext.get_current_locals()->localVars.insert(make_pair(Arg.getName().data(),&Arg));
    }

    if (Body->codegen()) {
        // Finish off the function.
        // TODO 后期自动匹配return，自动追加
        // Validate the generated code, checking for consistency.
        auto isNotValid = verifyFunction(*function, &errs());
        if (isNotValid) {
            LogError(("function '" + Proto->getName() + "' not valid\n").c_str());
            return nullptr;
        }
        TheCodeGenContext.pop_block();
        return function;
    }
    TheCodeGenContext.pop_block();
    TheCodeGenContext.removeFunction();
    // Error reading body, remove function.
    function->eraseFromParent();

    return nullptr;
}

FunctionAST::FunctionAST(PrototypeAST *proto, BlockAST *body) : Proto(proto), Body(body) {}

Value *BlockAST::codegen() {
    auto iterator = statements.begin();
    Value *lastStatementValue;
    for (; iterator != statements.end(); iterator++) {
        lastStatementValue = (*iterator)->codegen();
    }
    return lastStatementValue;
}

Value *ExpressionStatementAST::codegen() {
    return expr->codegen();
}

ExpressionStatementAST::ExpressionStatementAST(ExpressionAST *expr) : expr(expr) {}

VariableAssignmentAST::VariableAssignmentAST(const string &identifier, ExpressionAST *expr) : identifier(identifier),
                                                                                              expr(expr) {}

llvm::Value *VariableAssignmentAST::codegen() {
    Value *result = nullptr;
    auto vars = LOCALS;
    auto idx = LOCALS.find(identifier);
    if (idx != vars.end()) {
        idx->second = expr == nullptr ? nullptr : result = expr->codegen();
    } else {
        auto gv = TheModule->getNamedGlobal(identifier);
        if (gv) {
            result = Builder.CreateStore(expr->codegen(), gv);
        } else {
            return LogErrorV((identifier + " redefined!").c_str());
        }
    }
    return result;
}

VariableDeclarationAST::VariableDeclarationAST(const std::string &type, const std::string &identifier) {
    this->type = type;
    this->identifier = identifier;
    this->expr = nullptr;
}

VariableDeclarationAST::VariableDeclarationAST(const string &type, const string &identifier, ExpressionAST *expr) {
    this->type = type;
    this->identifier = identifier;
    this->expr = expr;
}

llvm::Value *VariableDeclarationAST::codegen() {
    Value *ret = nullptr;
    auto vp = TheCodeGenContext.get_current_locals();
    if (vp == nullptr) {
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier.c_str());
            return nullptr;
        } else {
            auto *gv = new GlobalVariable(*TheModule, getTypeFromStr(type), isConst,
                                          GlobalVariable::LinkageTypes::ExternalLinkage,
                                          Constant::getNullValue(getTypeFromStr(type)), identifier);
            gv->setInitializer(dyn_cast<Constant>(expr->codegen()));
            ret = gv;
        }
    } else {
        if (vp->localVars.find(identifier) == vp->localVars.end()) {
            (vp->localVars)[identifier] = expr == nullptr ? nullptr : ret = expr->codegen();
        }
    }
    return ret;
}

llvm::Value *IntegerExprAST::codegen() {
    return ConstantInt::get(Type::getInt32Ty(TheContext), Val, true);
}

llvm::Value *IdentifierExprAST::codegen() {
    auto locals = TheCodeGenContext.get_current_locals();
    if (locals) {
        auto idx = locals->localVars.find(identifier);
        if (idx != locals->localVars.end()) {
            return idx->second;
        }
    }
    // 可能是全局变量
    auto gv = TheModule->getNamedGlobal(identifier);
    if (gv) {
        return Builder.CreateLoad(gv);
    } else {
        return LogErrorV((identifier + " is not defined!!!").c_str());
    }
}

ReturnStmtAST::ReturnStmtAST(ExpressionAST *expr) : expr(expr) {}

ReturnStmtAST::ReturnStmtAST() {
    expr = nullptr;
}

llvm::Value *ReturnStmtAST::codegen() {
    if (expr == nullptr) {
        return Builder.CreateRetVoid();
    } else {
        return Builder.CreateRet(expr->codegen());
    }
}

Type *getTypeFromStr(const std::string &type) {
    if (type == "int") {
        return Type::getInt32Ty(TheContext);
    } else if (type == "long") {
        return Type::getInt64Ty(TheContext);
    } else if (type == "short"){
        return Type::getInt8Ty(TheContext);
    } else if (type == "bool"){
        return Type::getInt1Ty(TheContext); // 本质上就是1字节的int
    } else if (type == "void") {
        return Type::getVoidTy(TheContext);
    } else if (type == "double") {
        return Type::getDoubleTy(TheContext);
    } else if (type == "float") {
        return Type::getFloatTy(TheContext);
    } else {
        return nullptr;
    }
}
