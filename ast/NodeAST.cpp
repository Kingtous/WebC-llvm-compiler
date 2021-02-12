//
// Created by 金韬 on 2020/9/21.
//

#include "NodeAST.hpp"

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
    // 逻辑值bool
    if (type == BinaryType::AND) {
        // 与
        return this->codeGenAnd(LEA, REA);
    } else if (type == BinaryType::OR) {
        // 或
        return this->codeGenOr(LEA, REA);
    } else {
        llvm::Value *L = LEA->codegen();
        llvm::Value *R = REA->codegen();
        if (!L || !R)
            return nullptr;
        // 数值计算
        if (L->getType() == getTypeFromStr("int") && R->getType() == getTypeFromStr("int")) {
            switch (type) {
                case BinaryType::add:
                    return Builder.CreateAdd(L, R, "add");
                case BinaryType::sub:
                    return Builder.CreateSub(L, R, "sub");
                case BinaryType::mul:
                    return Builder.CreateMul(L, R, "mul");
                case BinaryType::divi:
                    return Builder.CreateSDiv(L, R, "divi");
                case BinaryType::mod:
                    return Builder.CreateSRem(L, R,
                                              "mod");
                case BinaryType::less:
                    return Builder.CreateICmpSLT(L, R, "less_than");
                case BinaryType::greater:
                    return Builder.CreateICmpSGT(L, R, "greater_than");
                case BinaryType::equ:
                    return Builder.CreateICmpEQ(L, R, "equ");
                case BinaryType::greater_equ:
                    return Builder.CreateICmpSGE(L, R, "greater_equ");
                case BinaryType::less_equ:
                    return Builder.CreateICmpSLE(L, R, "less_equ");
//            case BinaryType::AND:
//                auto boolL = Builder.CreateICmpNE(L,ConstantInt::get(getTypeFromStr("bool"),0));
//                auto boolR = Builder.CreateICmpNE(R,ConstantInt::get(getTypeFromStr("bool"),0));
//                return Builder.CreateICmpSLE(boolL, boolR, "less_equ");
//            case BinaryType::AND:
//                auto boolL = Builder.CreateICmpNE(L,ConstantInt::get(getTypeFromStr("bool"),0));
//                auto boolR = Builder.CreateICmpNE(R,ConstantInt::get(getTypeFromStr("bool"),0));
//                Builder.
//                return Builder.CreateICmpSLE(boolL, boolR, "less_equ");
//            default:
//                return LogErrorV(("invalid binary operator"));
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
                case BinaryType::divi:
                    return Builder.CreateFDiv(L, R, "divi");
                case BinaryType::mod:
                    return Builder.CreateFRem(L, R, "mod");
                case BinaryType::less:
                    return Builder.CreateFCmpOLT(L, R, "less_than");
                case BinaryType::equ:
                    return Builder.CreateFCmpOEQ(L, R, "ordered_fequ");
                case BinaryType::greater:
                    return Builder.CreateFCmpOGT(L, R, "greater_than");
                case BinaryType::greater_equ:
                    return Builder.CreateFCmpOGE(L, R, "greater_equ");
                case BinaryType::less_equ:
                    return Builder.CreateFCmpOLE(L, R, "less_equ");
                default:
                    return LogErrorV(("invalid binary operator"));
            }
        }
    }
}

BinaryExprAST::BinaryExprAST(BinaryType type, ExpressionAST *lea, ExpressionAST *rea) : type(type), LEA(lea),
                                                                                        REA(rea) {}

Value *BinaryExprAST::codeGenAnd(NodeAST *l, NodeAST *r) {
    if (Builder.GetInsertBlock() == NIL) {
        return LogErrorV("当前需要一个Basic Block来开始and环境");
    }
    auto func = Builder.GetInsertBlock()->getParent();
    if (func == NIL) {
        return LogErrorV("&&需要在函数环境下使用，建议在函数中使用");
    }
//    BasicBlock* condTrue = NIL;
//    BasicBlock* condEnd = NIL;
//    if (LOCALS->contextType == CodeGenBlockContextType::IF){
//        condTrue = LOCALS->context.ifCodeGenBlockContext->bbTrue;
//        condEnd = LOCALS->context.ifCodeGenBlockContext->bbElse == NIL ? LOCALS->context.ifCodeGenBlockContext->bbEndIf : LOCALS->context.ifCodeGenBlockContext->bbElse;
//    } else if (LOCALS->contextType == CodeGenBlockContextType::FOR){
//        condTrue = LOCALS->context.forCodeGenBlockContext->bbBody;
//        condEnd = LOCALS->context.forCodeGenBlockContext->bbEndFor;
//    } else {
//        return LogErrorV("不能将二元操作符应用于");
//    }
    auto bbCond1 = Builder.GetInsertBlock();
    auto bbCond2 = BasicBlock::Create(TheContext, "and_right");
    auto bbCondEnd = BasicBlock::Create(TheContext, "and_end");
    auto cond1V = l->codegen();
    cond1V = Builder.CreateICmpSGT(cond1V, ConstantInt::get(cond1V->getType(), 0));
    bbCond1 = Builder.GetInsertBlock();
    Builder.CreateCondBr(cond1V, bbCond2, bbCondEnd); // 如果为true的话接着判断cond2，
    func->getBasicBlockList().push_back(bbCond2);
    Builder.SetInsertPoint(bbCond2);
    auto cond2V = r->codegen();
    cond2V = Builder.CreateICmpSGT(cond2V, ConstantInt::get(cond2V->getType(), 0));
    // 要考虑嵌套
    bbCond2 = Builder.GetInsertBlock();
    Builder.CreateBr(bbCondEnd);
    func->getBasicBlockList().push_back(bbCondEnd);
    Builder.SetInsertPoint(bbCondEnd);
    auto phi = Builder.CreatePHI(getTypeFromStr("bool"), 2);
    phi->addIncoming(cond1V, bbCond1);
    phi->addIncoming(cond2V, bbCond2);
    return phi;
}

Value *BinaryExprAST::codeGenOr(NodeAST *l, NodeAST *r) {
    if (Builder.GetInsertBlock() == NIL) {
        return LogErrorV("当前需要一个Basic Block来开始or环境");
    }
    auto func = Builder.GetInsertBlock()->getParent();
    if (func == NIL) {
        return LogErrorV("||需要在函数环境下使用");
    }
//    BasicBlock* condTrue = NIL;
//    BasicBlock* condEnd = NIL;
//    if (LOCALS->contextType == CodeGenBlockContextType::IF){
//        condTrue = LOCALS->context.ifCodeGenBlockContext->bbTrue;
//        condEnd = LOCALS->context.ifCodeGenBlockContext->bbElse == NIL ? LOCALS->context.ifCodeGenBlockContext->bbEndIf : LOCALS->context.ifCodeGenBlockContext->bbElse;
//    } else if (LOCALS->contextType == CodeGenBlockContextType::FOR){
//        condTrue = LOCALS->context.forCodeGenBlockContext->bbBody;
//        condEnd = LOCALS->context.forCodeGenBlockContext->bbEndFor;
//    } else {
//        return LogErrorV("不能将二元操作符应用于");
//    }
    auto bbCond1 = Builder.GetInsertBlock();
    auto bbCond2 = BasicBlock::Create(TheContext, "or_right");
    auto bbCondEnd = BasicBlock::Create(TheContext, "or_end");
    // TODO
    Builder.SetInsertPoint(bbCond1);
    auto cond1V = l->codegen();
    cond1V = Builder.CreateICmpNE(cond1V, ConstantInt::get(cond1V->getType(), 0));
    bbCond1 = Builder.GetInsertBlock();
    Builder.CreateCondBr(cond1V, bbCondEnd, bbCond2);

    func->getBasicBlockList().push_back(bbCond2);
    Builder.SetInsertPoint(bbCond2);
    auto cond2V = r->codegen();
    cond2V = Builder.CreateICmpNE(cond2V, ConstantInt::get(cond2V->getType(), 0));
    // 要考虑嵌套，更新当前bb
    bbCond2 = Builder.GetInsertBlock();
    Builder.CreateBr(bbCondEnd);
    func->getBasicBlockList().push_back(bbCondEnd);
    Builder.SetInsertPoint(bbCondEnd);
    auto phi = Builder.CreatePHI(getTypeFromStr("bool"), 2);
    phi->addIncoming(cond1V, bbCond1);
    phi->addIncoming(cond2V, bbCond2);
    return phi;
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
        Arg.setName(args[Idx++]->identifier->identifier);
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
    ifCondV = Builder.CreateICmpNE(ifCondV, ConstantInt::get(getTypeFromStr("bool"), 0), "neuq_jintao_ifcond");
    // 获取if里面的函数体，准备创建基础块
    auto theFunction = Builder.GetInsertBlock()->getParent();
    if (!theFunction) {
        return LogErrorV("if没在函数体内使用");
    }
    // 创建三个basic block，先添加IfTrue至函数体里面
    BasicBlock *bbIfTrue = BasicBlock::Create(TheContext, "neuq_jintao_if_true", theFunction);
    BasicBlock *bbElse = else_stmt == NIL ? NIL : BasicBlock::Create(TheContext, "neuq_jintao_else");
    BasicBlock *bbIfEnd = BasicBlock::Create(TheContext, "neuq_jintao_if_end");
    LOCALS->setContextType(CodeGenBlockContextType::IF);
    LOCALS->context.ifCodeGenBlockContext = new IfCodeGenBlockContext(Builder.GetInsertBlock(), bbIfEnd, bbIfTrue,
                                                                      bbElse);
    // 创建条件分支,createCondBr(条件结果，为true的basic block，为false的basic block)
    if (bbElse == NIL) {
        Builder.CreateCondBr(ifCondV, bbIfTrue, bbIfEnd);
    } else {
        Builder.CreateCondBr(ifCondV, bbIfTrue, bbElse);
    }
    // 开启往Basic Block中塞入代码
    Builder.SetInsertPoint(bbIfTrue);
    Value *ifTrueV = if_stmt->codegen();
    if (!ifTrueV) {
        return LogErrorV("if里面的内容有误");
    }
    // if 中为true的代码写完了，将if跳转到else后面，也就是IfEnd结点。这符合逻辑，同时也是LLVM的强制要求，每个Basic Block必须以跳转结尾
    if (Builder.GetInsertBlock()->empty() || !(*Builder.GetInsertBlock()->rbegin()).isTerminator()) {
        Builder.CreateBr(bbIfEnd);
    }
    // 将插入if内容时，可能会有循环嵌套，需要更新嵌套最底层的insertBlock
    bbIfTrue = Builder.GetInsertBlock();
    // 塞入else
    if (bbElse != NIL) {
        theFunction->getBasicBlockList().push_back(bbElse);
        Builder.SetInsertPoint(bbElse);
        Value *elseV = nullptr;
        if (else_stmt) {
            elseV = else_stmt->codegen();
            if (!elseV) {
                return LogErrorV("else里面内容有误");
            }
        }
        if (Builder.GetInsertBlock()->empty() || !(*Builder.GetInsertBlock()->rbegin()).isTerminator()) {
            Builder.CreateBr(bbIfEnd);
        }
        bbElse = Builder.GetInsertBlock();
    }
    // 创建末尾结点
    theFunction->getBasicBlockList().push_back(bbIfEnd);
    Builder.SetInsertPoint(bbIfEnd);
//    PHINode *pn;
//    if (elseV){
//        pn = Builder.CreatePHI(ifTrueV->getType(), 2, "neuq_jintao_iftmp");
//    } else {
//        pn = Builder.CreatePHI(ifTrueV->getType(), 1, "neuq_jintao_iftmp");
//    }
//    pn->addIncoming(ifTrueV, bbIfTrue);
//    if (elseV){
//        pn->addIncoming(elseV, bbElse);
//    }
    return bbIfEnd;
}

ConditionAST::ConditionAST(ExpressionAST *ifCond, BlockAST *ifStmt, BlockAST *elseStmt) : if_cond(ifCond),
                                                                                          if_stmt(ifStmt),
                                                                                          else_stmt(elseStmt) {}

Value *ForExprAST::codegen() {
    Function *theFuntion = Builder.GetInsertBlock()->getParent();
    if (theFuntion == nullptr) {
        return nullptr;
    }
    // 先执行for(xxx;;)
    auto tmpForBlock = BasicBlock::Create(TheContext, "bb_for_start", theFuntion);
    TheCodeGenContext.push_block(tmpForBlock);

    auto bbStart = tmpForBlock;
    auto bbCond = BasicBlock::Create(TheContext, "bb_for_cond", theFuntion);
    auto bbStep = BasicBlock::Create(TheContext, "bb_for_step", theFuntion);
    auto bbBody = BasicBlock::Create(TheContext, "bb_for_body", theFuntion);
    auto bbEndFor = BasicBlock::Create(TheContext, "bb_for_end");
    LOCALS->setContextType(CodeGenBlockContextType::FOR);
    LOCALS->context.forCodeGenBlockContext = new ForCodeGenBlockContext(bbStart, bbCond, bbStep, bbBody, bbEndFor);
    Builder.CreateBr(bbStart);
    Builder.SetInsertPoint(bbStart);
    auto startV = Start->codegen();
    if (startV == nullptr) {
        return LogErrorV("for(x;;){}中x有误");
    }
    Builder.CreateBr(bbCond);

    Builder.SetInsertPoint(bbCond);
    auto condV = Cond->codegen();
    if (condV == nullptr){
        return LogErrorV("for(;x;){}中x有误");
    }

    condV = Builder.CreateICmpNE(condV, ConstantInt::get(getTypeFromStr("bool"), 0), "neuq_jintao_ifcond");
    Builder.CreateCondBr(condV, bbBody, bbEndFor);

    Builder.SetInsertPoint(bbBody);
    auto body = Body->codegen();
    if (body == nullptr) {
        return LogErrorV("for执行内容有误");
    }
    auto ins = Builder.GetInsertBlock()->rbegin();
    if (ins == Builder.GetInsertBlock()->rend() || !(*ins).isTerminator()) {
        Builder.CreateBr(bbStep);
    }
    Builder.SetInsertPoint(bbStep);
    auto stepV = Step->codegen();
    if (stepV == nullptr) {
        return LogErrorV("for(;;x){}中x有误");
    }
    Builder.CreateBr(bbCond);
    theFuntion->getBasicBlockList().push_back(bbEndFor);
    Builder.SetInsertPoint(bbEndFor);
    LOCALS->setContextType(CodeGenBlockContextType::NONE);
    return bbEndFor;
}

ForExprAST::ForExprAST(NodeAST *start, NodeAST *end, NodeAST *step, BlockAST *body) : Start(start),
                                                                                                        Cond(end),
                                                                                                        Step(step),
                                                                                                        Body(body) {}


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
    for (auto &Arg : function->args()) {
        // 分配一片内存
        auto mem = Builder.CreateAlloca(Arg.getType());
        Builder.CreateStore(&Arg, mem);
        LOCALSVARS.insert(make_pair(Arg.getName().data(), mem));
    }

    if (Body->codegen()) {
        // Finish off the function.
        // TODO 后期自动匹配return，自动追加
        // Validate the generated code, checking for consistency.
        auto isNotValid = verifyFunction(*function, &errs());
        if (isNotValid) {
            LogError(("function '" + Proto->getName() + "' not valid\n").c_str());
            errs()<<"function ll codes:\n";
            function->print(errs());
            return nullptr;
        } else {
            function->print(outs());
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
    auto func = TheCodeGenContext.getFunc();
    if (func == NIL) {
        auto iterator = statements.begin();
        Value *lastStatementValue;
        for (; iterator != statements.end(); iterator++) {
            auto it = (*iterator);
            if (typeid(*it) == typeid(BreakStmtAST)) {
                if (iterator + 1 != statements.end()) {
                    LogWarn("警告：作用域内break后方的语句无法到达");
                }
                break;
            } else if (typeid(*it) == typeid(ContinueStmtAST)) {
                if (iterator + 1 != statements.end()) {
                    LogWarn("警告：作用域内continue后方的语句无法到达");
                }
                break;
            }
            lastStatementValue = it->codegen();
        }
        return lastStatementValue;
    } else {
        auto bb = BasicBlock::Create(TheContext, "blk");
        Builder.CreateBr(bb);
        func->getBasicBlockList().push_back(bb);
        Builder.SetInsertPoint(bb);
        auto iterator = statements.begin();
        Value *lastStatementValue;
        for (; iterator != statements.end(); iterator++) {
            auto it = (*iterator);
            lastStatementValue = (*iterator)->codegen();
            if (typeid(*it) == typeid(BreakStmtAST)) {
                if (iterator + 1 != statements.end()) {
                    LogWarn("警告：作用域内break后方的语句无法到达");
                }
                break;
            } else if (typeid(*it) == typeid(ContinueStmtAST)) {
                if (iterator + 1 != statements.end()) {
                    LogWarn("警告：作用域内continue后方的语句无法到达");
                }
                break;
            }
        }
        return lastStatementValue;
    }
}

Value *ExpressionStatementAST::codegen() {
    return expr->codegen();
}

ExpressionStatementAST::ExpressionStatementAST(ExpressionAST *expr) : expr(expr) {}

VariableAssignmentAST::VariableAssignmentAST(const string &identifier, ExpressionAST *expr) : identifier(identifier),
                                                                                              expr(expr) {}

llvm::Value *VariableAssignmentAST::codegen() {
    Value *result = nullptr;
    auto ori_v = FINDLOCAL(identifier);
    if (ori_v != nullptr) {
        auto v = expr == nullptr ? nullptr :expr->codegen();
        result = Builder.CreateStore(v,ori_v);
    } else {
        auto gv = TheModule->getNamedGlobal(identifier);
        if (gv) {
            result = Builder.CreateStore(expr->codegen(), gv);
        } else {
            return LogErrorV((identifier + " is not defined!").c_str());
        }
    }
    return result;
}

llvm::Value *VariableDeclarationAST::codegen() {
    Value *ret = nullptr;
    if (!HASLOCALS) {
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier->identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier->identifier.c_str());
            return nullptr;
        } else {
            auto *gv = new GlobalVariable(*TheModule, getTypeFromStr(type), isConst,
                                          GlobalVariable::LinkageTypes::ExternalLinkage,
                                          Constant::getNullValue(getTypeFromStr(type)), identifier->identifier);
            if (expr != nullptr) {
                gv->setInitializer(dyn_cast<Constant>(expr->codegen()));
            }
            ret = gv;
        }
    } else {
        if (LOCALSVARS.find(identifier->identifier) == LOCALSVARS.end()) {
            ret = Builder.CreateAlloca(getTypeFromStr(type));
//            identifier.
//            auto mem = Builder.CreateAlloca(getTypeFromStr(type),)
            if (expr != nullptr) {
                auto v = expr->codegen();
                if (v->getType()->isIntegerTy() && v->getType() != getTypeFromStr(type)) {
                    // 不同，尝试进行修剪
                    v = Builder.CreateZExtOrTrunc(v, getTypeFromStr(type));
                }
                Builder.CreateStore(v, ret);
            }
            INSERTLOCAL(identifier->identifier, ret);
        }
    }
    return ret;
}

VariableDeclarationAST::VariableDeclarationAST(const string &type, IdentifierExprAST *identifier, ExpressionAST *expr,
                                               bool isConst) : type(type), identifier(identifier), expr(expr),
                                                               isConst(isConst) {}

void VariableDeclarationAST::setIsConst(bool isConst) {
    VariableDeclarationAST::isConst = isConst;
}

llvm::Value *IntegerExprAST::codegen() {
    return ConstantInt::get(Type::getInt32Ty(TheContext), Val, true);
}

llvm::Value *IdentifierExprAST::codegen() {
    if (HASLOCALS) {
        auto local_mem = FINDLOCAL(identifier);
        if (local_mem != nullptr) {
            return Builder.CreateLoad(local_mem);
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

IdentifierExprAST::IdentifierExprAST() {

}

ReturnStmtAST::ReturnStmtAST(ExpressionAST *expr) : expr(expr) {}

ReturnStmtAST::ReturnStmtAST() {
    expr = nullptr;
}

llvm::Value *ReturnStmtAST::codegen() {
    if (expr == nullptr) {
        return Builder.CreateRetVoid();
    } else {
        auto v = expr->codegen();
        return Builder.CreateRet(v);
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

// int a[5][6][7] -> vector<Expr>(7,6,5)
Type *buildArrayType(vector<ExpressionAST *> *vec, Type *arrayElemType) {
    Type *arr_type = nullptr;
    auto it = vec->rbegin();
    for (; it < vec->rend(); it++) {
        auto v = (*it)->codegen();
        if (isa<ConstantInt>(v)) {
            auto constant_int = dyn_cast<ConstantInt>(v);
            uint64_t size = 0;
            if (constant_int->getBitWidth() <= 32) {
                size = constant_int->getZExtValue();
            } else {
                size = constant_int->getSExtValue();
            }
            arr_type = arr_type == nullptr ? ArrayType::get(arrayElemType, size) : ArrayType::get(arr_type, size);
        } else {
            LogError("数组声明[]内必须为常量，请检查");
        }
    }
    return arr_type;
}

Type *getArrayElemType(Value *value) {
    if (value == nullptr ||
        (!value->getType()->isPointerTy() && !value->getType()->isArrayTy())) {
        return nullptr;
    }
    Type *tmp = value->getType();
    while (tmp->isPointerTy()) {
        tmp = tmp->getContainedType(0);
    }
    while (tmp->isArrayTy()) {
        tmp = tmp->getArrayElementType();
    }
    return tmp;
}

Type *getArrayType(Value *value) {
    if (value == nullptr ||
        (!value->getType()->isPointerTy() && !value->getType()->isArrayTy())) {
        return nullptr;
    }
    Type *tmp = value->getType();
    while (tmp->isPointerTy()) {
        tmp = tmp->getContainedType(0);
    }
    if (tmp->isArrayTy()) {
        return tmp;
    } else {
        return nullptr;
    }
}


llvm::Value *IdentifierArrExprAST::codegen() {
    auto local = FINDLOCAL(identifier);
    if (local == nullptr) {
        return LogErrorV(("未找到" + identifier + "\n").c_str());
    }
    auto arr_elem_type = getArrayType(local);
    auto it = arrIndex->rbegin();
    Value *ret = local;
    auto v_vec = vector<Value *>();
    // 添加0取指针地址
    v_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    for (; it < arrIndex->rend(); it++) {
        auto v = (*it)->codegen();
        if (v == nullptr) {
            return LogErrorV("数组解析失败");
        }
        v_vec.push_back(v);
    }
    ret = Builder.CreateInBoundsGEP(ret, ArrayRef<Value *>(v_vec));
    return Builder.CreateLoad(ret);
}

IdentifierArrExprAST::IdentifierArrExprAST(const string &identifier, vector<ExpressionAST *> *arrIndex)
        : IdentifierExprAST(identifier), arrIndex(arrIndex) {}

VariableArrDeclarationAST::VariableArrDeclarationAST(const string &type, IdentifierArrExprAST *identifier,
                                                     ExpressionAST *expr, bool isConst) : type(type),
                                                                                          identifier(identifier),
                                                                                          expr(expr),
                                                                                          isConst(isConst) {}

llvm::Value *VariableArrDeclarationAST::codegen() {
    Value *ret = nullptr;
    Type *arr_type = buildArrayType(identifier->arrIndex, getTypeFromStr(type));
    if (!HASLOCALS) {
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier->identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier->identifier.c_str());
            return nullptr;
        } else {
            auto gv = new GlobalVariable(*TheModule, arr_type, isConst,
                                         GlobalVariable::LinkageTypes::ExternalLinkage,
                                         Constant::getNullValue(arr_type), identifier->identifier);
            if (expr != nullptr) {
                gv->setInitializer(dyn_cast<Constant>(expr->codegen()));
            }
            gv->print(outs(), true);
            ret = gv;
        }
    } else {
        auto at = identifier->arrIndex->rbegin();
        if (LOCALSVARS.find(identifier->identifier) == LOCALSVARS.end()) {
            auto mem = Builder.CreateAlloca(arr_type);
            // TODO
//            if (expr != nullptr){
//                auto v = expr->codegen();
//                Builder.CreateStore(v,mem);
//            }
            INSERTLOCAL(identifier->identifier, mem);
        }
    }
    return ret;
}

VariableArrAssignmentAST::VariableArrAssignmentAST(IdentifierArrExprAST *identifier, ExpressionAST *expr) : identifier(
        identifier), expr(expr) {}

llvm::Value *VariableArrAssignmentAST::codegen() {
    auto arr_addr = FINDLOCAL(identifier->identifier);
    if (arr_addr == nullptr) {
        return LogErrorV((identifier->identifier + "is not defined").c_str());
    }
    auto st = identifier->arrIndex->rbegin();
    Value *ret = arr_addr;
    vector<Value *> vec;
    // 0取地址
    vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    for (; st != identifier->arrIndex->rend(); st++) {
        auto v = (*st)->codegen();
        vec.push_back(v);
    }
    ret = Builder.CreateInBoundsGEP(arr_addr, ArrayRef(vec));
    auto newV = expr->codegen();
    ret = Builder.CreateStore(newV, ret);
    return ret;
}

BreakStmtAST::BreakStmtAST() = default;

Value *BreakStmtAST::codegen() {
    auto blk = TheCodeGenContext.findTopLoopCodeGenBlockTypeBlock();
    if (blk == NIL) {
        return LogErrorV("break 不能用作于当前的代码上下文中，请检查");
    }
    switch (blk->contextType) {
        case NONE:
            return LogErrorV("程序匹配的break环境与当前域不符(NONE)，请联系作者");
        case FOR:
            if (blk->context.forCodeGenBlockContext == NIL
                || blk->context.forCodeGenBlockContext->bbEndFor == NIL) {
                return LogErrorV("程序匹配的上下文context为NULL，请联系作者");
            }
            return Builder.CreateBr(blk->context.forCodeGenBlockContext->bbEndFor);
        case IF:
            return LogErrorV("程序匹配的break环境与当前域不符(IF)，请联系作者");
        case WHILE:
            if (blk->context.whileCodeGenBlockContext == NIL
                || blk->context.whileCodeGenBlockContext->bbEndWhile == NIL) {
                return LogErrorV("程序匹配的上下文context为NULL(while continue;)，请联系作者");
            }
            return Builder.CreateBr(blk->context.whileCodeGenBlockContext->bbEndWhile);
        default:
            return LogErrorV("Break在未知的代码域中，或编译器目前无法处理的关键字");
    }
    return NIL;
}

ContinueStmtAST::ContinueStmtAST() = default;

Value *ContinueStmtAST::codegen() {
    auto blk = TheCodeGenContext.findTopLoopCodeGenBlockTypeBlock();
    if (blk == NIL) {
        return LogErrorV("break 不能用作于当前的代码上下文中，请检查");
    }
    switch (blk->contextType) {
        case NONE:
            return LogErrorV("程序匹配的break环境与当前域不符(NONE)，请联系作者");
        case FOR:
            if (blk->context.forCodeGenBlockContext == NIL
                || blk->context.forCodeGenBlockContext->bbEndFor == NIL) {
                return LogErrorV("程序匹配的上下文context为NULL，请联系作者");
            }
            return Builder.CreateBr(blk->context.forCodeGenBlockContext->bbStep);
        case IF:
            return LogErrorV("程序匹配的break环境与当前域不符(IF)，这可能是个bug，请联系作者");
        case WHILE:
            if (blk->context.whileCodeGenBlockContext == NIL
                || blk->context.whileCodeGenBlockContext->bbCond == NIL) {
                return LogErrorV("程序匹配的上下文context为NULL(while continue;)，请联系作者");
            }
            return Builder.CreateBr(blk->context.whileCodeGenBlockContext->bbCond);
        default:
            return LogErrorV("Break在未知的代码域中，或编译器目前无法处理的关键字");
    }
    return NIL;
}

WhileStmtAST::WhileStmtAST(NodeAST *cond, BlockAST *body) : Cond(cond), Body(body) {}

llvm::Value *WhileStmtAST::codegen() {
    auto function = Builder.GetInsertBlock()->getParent();
    if (function == NIL) {
        return LogErrorV("while 不能用在非函数体中");
    }
    BasicBlock *bbCond = BasicBlock::Create(TheContext, "while_cond", function);
    BasicBlock *bbBody = BasicBlock::Create(TheContext, "while_body");
    BasicBlock *bbEndWhile = BasicBlock::Create(TheContext, "while_end");
    LOCALS->setContextType(CodeGenBlockContextType::WHILE);
    LOCALS->context.whileCodeGenBlockContext = new WhileCodeGenBlockContext(bbCond, bbBody, bbEndWhile);
    // 创建条件分支
    Builder.CreateBr(bbCond);
    Builder.SetInsertPoint(bbCond);
    auto condV = Cond->codegen();
    if (condV == NIL) {
        return LogErrorV("while中的判断语句解析失败");
    }
    condV = Builder.CreateICmpNE(condV, ConstantInt::get(getTypeFromStr("bool"), 0));
    Builder.CreateCondBr(condV, bbBody, bbEndWhile);
    function->getBasicBlockList().push_back(bbBody);
    Builder.SetInsertPoint(bbBody);
    auto bodyV = Body->codegen();
    if (bodyV == NIL) {
        return LogErrorV("while中的判断语句解析失败");
    }
    if (Builder.GetInsertBlock()->empty() || !Builder.GetInsertBlock()->rbegin()->isTerminator()) {
        Builder.CreateBr(bbCond);
    }
    function->getBasicBlockList().push_back(bbEndWhile);
    Builder.SetInsertPoint(bbEndWhile);
    return bbEndWhile;
}
