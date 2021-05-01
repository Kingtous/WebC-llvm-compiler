//
// Created by 金韬 on 2020/9/21.
//

#include "NodeAST.hpp"

#include <utility>
#include <iostream>
#include "ErrHelper.h"
#include "codegen/CodeGen.h"

long StringExprAST::id = 0;

llvm::Value *DoubleExprAST::codegen() {
    // 在LLVM IR中，数字常量用ConstantFP类表示 ，它在APFloat 内部保存数值（APFloat能够保持任意精度的浮点常量）
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}

string DoubleExprAST::toString() {
    return to_string(Val);
}

llvm::Value *VariableExprAST::codegen() {
    // 从符号表中取出
    llvm::Value *v = FINDLOCAL(Name);
    if (!v) {
        ABORT_COMPILE;
        LogErrorV((std::string("未知变量名:") + Name).c_str());
    }
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
        if (!L || !R) {
            ABORT_COMPILE;
            return LogErrorV("二元表达式解析失败");
        }
        // 数值计算
        if (L->getType()->isIntegerTy() && R->getType()->isIntegerTy()) {
            auto l_width = dyn_cast<IntegerType>(L->getType())->getBitWidth();
            auto r_width = dyn_cast<IntegerType>(R->getType())->getBitWidth();
            if (l_width > r_width) {
                R = Builder->CreateZExtOrTrunc(R, L->getType());
            } else if (l_width < r_width) {
                L = Builder->CreateZExtOrTrunc(L, R->getType());
            }
            switch (type) {
                case BinaryType::add:
                    return Builder->CreateAdd(L, R, "add");
                case BinaryType::sub:
                    return Builder->CreateSub(L, R, "sub");
                case BinaryType::mul:
                    return Builder->CreateMul(L, R, "mul");
                case BinaryType::divi:
                    return Builder->CreateSDiv(L, R, "divi");
                case BinaryType::mod:
                    return Builder->CreateSRem(L, R,
                                               "mod");
                case BinaryType::less:
                    return Builder->CreateICmpSLT(L, R, "less_than");
                case BinaryType::greater:
                    return Builder->CreateICmpSGT(L, R, "greater_than");
                case BinaryType::equ:
                    return Builder->CreateICmpEQ(L, R, "equ");
                case BinaryType::greater_equ:
                    return Builder->CreateICmpSGE(L, R, "greater_equ");
                case BinaryType::less_equ:
                    return Builder->CreateICmpSLE(L, R, "less_equ");
                case BinaryType::n_equ:
                    return Builder->CreateICmpNE(L, R, "not_equ");
//            case BinaryType::AND:
//                auto boolL = Builder->CreateICmpNE(L,ConstantInt::get(getTypeFromStr("bool"),0));
//                auto boolR = Builder->CreateICmpNE(R,ConstantInt::get(getTypeFromStr("bool"),0));
//                return Builder->CreateICmpSLE(boolL, boolR, "less_equ");
//            case BinaryType::AND:
//                auto boolL = Builder->CreateICmpNE(L,ConstantInt::get(getTypeFromStr("bool"),0));
//                auto boolR = Builder->CreateICmpNE(R,ConstantInt::get(getTypeFromStr("bool"),0));
//                Builder->
//                return Builder->CreateICmpSLE(boolL, boolR, "less_equ");
                default:
                    ABORT_COMPILE;
                    return LogErrorV(("invalid binary operator"));
            }
        } else {
            // 类型不相同，做类型转换
            if (L->getType()->isIntegerTy()) {
                L = Builder->CreateSIToFP(L, R->getType());
            }
            if (R->getType()->isIntegerTy()) {
                R = Builder->CreateSIToFP(R, L->getType());
            }
            switch (type) {
                case BinaryType::add:
                    return Builder->CreateFAdd(L, R, "add");
                case BinaryType::sub:
                    return Builder->CreateFSub(L, R, "sub");
                case BinaryType::mul:
                    return Builder->CreateFMul(L, R, "mul");
                case BinaryType::divi:
                    return Builder->CreateFDiv(L, R, "divi");
                case BinaryType::mod:
                    return Builder->CreateFRem(L, R, "mod");
                case BinaryType::less:
                    return Builder->CreateFCmpOLT(L, R, "less_than");
                case BinaryType::equ:
                    return Builder->CreateFCmpOEQ(L, R, "ordered_fequ");
                case BinaryType::greater:
                    return Builder->CreateFCmpOGT(L, R, "greater_than");
                case BinaryType::greater_equ:
                    return Builder->CreateFCmpOGE(L, R, "greater_equ");
                case BinaryType::less_equ:
                    return Builder->CreateFCmpOLE(L, R, "less_equ");
                case BinaryType::n_equ:
                    return Builder->CreateFCmpONE(L, R, "not_equ");
                default:
                    ABORT_COMPILE;
                    return LogErrorV(("invalid binary operator"));
            }
        }
    }
    ABORT_COMPILE;
    return NIL;
}

BinaryExprAST::BinaryExprAST(BinaryType type, ExpressionAST *lea, ExpressionAST *rea) : type(type), LEA(lea),
                                                                                        REA(rea) {}

Value *BinaryExprAST::codeGenAnd(NodeAST *l, NodeAST *r) {
    if (Builder->GetInsertBlock() == NIL) {
        ABORT_COMPILE;
        return LogErrorV("当前需要一个Basic Block来开始and环境");
    }
    auto func = Builder->GetInsertBlock()->getParent();
    if (func == NIL) {
        ABORT_COMPILE;
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
    auto bbCond1 = Builder->GetInsertBlock();
    auto bbCond2 = BasicBlock::Create(*TheContext, "and_right");
    auto bbCondEnd = BasicBlock::Create(*TheContext, "and_end");
    auto cond1V = l->codegen();
    cond1V = Builder->CreateICmpSGT(cond1V, ConstantInt::get(cond1V->getType(), 0));
    bbCond1 = Builder->GetInsertBlock();
    Builder->CreateCondBr(cond1V, bbCond2, bbCondEnd); // 如果为true的话接着判断cond2，
    func->getBasicBlockList().push_back(bbCond2);
    Builder->SetInsertPoint(bbCond2);
    auto cond2V = r->codegen();
    cond2V = Builder->CreateICmpSGT(cond2V, ConstantInt::get(cond2V->getType(), 0));
    // 要考虑嵌套
    bbCond2 = Builder->GetInsertBlock();
    Builder->CreateBr(bbCondEnd);
    func->getBasicBlockList().push_back(bbCondEnd);
    Builder->SetInsertPoint(bbCondEnd);
    auto phi = Builder->CreatePHI(getTypeFromStr("bool"), 2);
    phi->addIncoming(cond1V, bbCond1);
    phi->addIncoming(cond2V, bbCond2);
    return phi;
}

Value *BinaryExprAST::codeGenOr(NodeAST *l, NodeAST *r) {
    if (Builder->GetInsertBlock() == NIL) {
        ABORT_COMPILE;
        return LogErrorV("当前需要一个Basic Block来开始or环境");
    }
    auto func = Builder->GetInsertBlock()->getParent();
    if (func == NIL) {
        ABORT_COMPILE;
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
    auto bbCond1 = Builder->GetInsertBlock();
    auto bbCond2 = BasicBlock::Create(*TheContext, "or_right");
    auto bbCondEnd = BasicBlock::Create(*TheContext, "or_end");
    // TODO
    Builder->SetInsertPoint(bbCond1);
    auto cond1V = l->codegen();
    cond1V = Builder->CreateICmpNE(cond1V, ConstantInt::get(cond1V->getType(), 0));
    bbCond1 = Builder->GetInsertBlock();
    Builder->CreateCondBr(cond1V, bbCondEnd, bbCond2);

    func->getBasicBlockList().push_back(bbCond2);
    Builder->SetInsertPoint(bbCond2);
    auto cond2V = r->codegen();
    cond2V = Builder->CreateICmpNE(cond2V, ConstantInt::get(cond2V->getType(), 0));
    // 要考虑嵌套，更新当前bb
    bbCond2 = Builder->GetInsertBlock();
    Builder->CreateBr(bbCondEnd);
    func->getBasicBlockList().push_back(bbCondEnd);
    Builder->SetInsertPoint(bbCondEnd);
    auto phi = Builder->CreatePHI(getTypeFromStr("bool"), 2);
    phi->addIncoming(cond1V, bbCond1);
    phi->addIncoming(cond2V, bbCond2);
    return phi;
}

string BinaryExprAST::toString() {
    return string("二元操作结点：") + LEA->toString() + "," + REA->toString();
}

llvm::Value *CallExprAST::codegen() {
    // Look up the name in the global module table.
    llvm::Function *func = TheModule->getFunction(callName);
    std::vector<llvm::Value *> argsV;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        Value *av = args[i]->codegen();
        if (av == NIL) {
            ABORT_COMPILE;
            return LogErrorV(("函数形参解析失败：" + args[i]->toString()).c_str());
        }
        argsV.push_back(av);
    }

    if (func == NIL) {
        //if find in the local value ,it is the function point
        auto call_func_mem = FINDLOCAL(callName);
        if (call_func_mem != NIL) {
            auto call_mem = Builder->CreateLoad(call_func_mem);
            if (call_mem->getType()->isPointerTy() &&
                call_mem->getType()->getPointerElementType()->getTypeID() ==
                llvm::Type::FunctionTyID) {
                // 是函数
                auto function_type = dyn_cast<FunctionType>(call_mem->getType()->getPointerElementType());
                return Builder->CreateCall(function_type, call_mem, argsV);;
            }
        }
    }

    if (func == NIL) {
        auto ev = ExternFunctionLinker::tryHandleFuncCall(*TheContext, *TheModule, callName, &argsV);
        // Extern直接处理，就直接返回
        if (ev) {
            return ev;
        }
    }

    if (!func && LOCALSVARS.find(callName) == LOCALSVARS.end()) {
        ABORT_COMPILE;
        return LogErrorV(("使用了未知的函数：" + callName).c_str());
    }
    // If argument mismatch error.
    if (func->arg_size() != args.size()) {
        ABORT_COMPILE;
        return LogErrorV("Incorrect # arguments passed");
    }
    return Builder->CreateCall(func, argsV);
}

string CallExprAST::toString() {
    return "函数调用：" + callName;
}

const string &CallExprAST::getCallName() const {
    return callName;
}

llvm::Function *PrototypeAST::codegen() {
    // 形参的类型 int*()
    auto type = FunctionType::get(getTypeFromStr("int"), false);

    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type *> Args;
    for (auto arg : args) {
        if (typeid(*arg) == typeid(VariableArrDeclarationAST)) {
            auto arr_arg = dynamic_cast<VariableArrDeclarationAST *>(arg);
            Args.push_back(arr_arg->buildPointerTy());
        } else if (typeid(*arg) == typeid(FuncPtrAST)) {//如果这种类型是FunctrAST的话
            auto ptr_arg = dynamic_cast<FuncPtrAST *>(arg);
            std::vector<llvm::Type *> putArgs;
            for (int i = 1; i < ptr_arg->args.size(); i++) {
                putArgs.push_back(getTypeFromStr(ptr_arg->args[i]->identifier));
            }
            llvm::ArrayRef<llvm::Type *> argsRef(putArgs);
            llvm::FunctionType *funcPtrType = llvm::FunctionType::get(getTypeFromStr(ptr_arg->args[0]->identifier),
                                                                      argsRef,
                                                                      false);
            Args.push_back(funcPtrType->getPointerTo());
        } else {
            Args.push_back(getTypeFromStr(arg->type));
        }
    }
    llvm::FunctionType *FT = llvm::FunctionType::get(getTypeFromStr(returnType), Args, false);;
    llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, getName(), TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        Arg.setName(args[Idx++]->getName());
    }
    return F;
}

Value *ConditionAST::codegen() {
    // 解析if表达式
    Value *ifCondV = if_cond->codegen();
    if (!ifCondV) {
        ABORT_COMPILE;
        return LogErrorV("if parse error");
    }
    // 创建一条 icnp ne指令，用于if的比较，由于是bool比较，此处直接与0比较。
    ifCondV = Builder->CreateICmpNE(ifCondV, ConstantInt::get(getTypeFromStr("bool"), 0), "neuq_jintao_ifcond");
    // 获取if里面的函数体，准备创建基础块
    auto theFunction = Builder->GetInsertBlock()->getParent();
    if (!theFunction) {
        ABORT_COMPILE;
        return LogErrorV("if没在函数体内使用");
    }
    TheCodeGenContext->push_block(Builder->GetInsertBlock());
    // 创建三个basic block，先添加IfTrue至函数体里面
    BasicBlock *bbIfTrue = BasicBlock::Create(*TheContext, "neuq_jintao_if_true", theFunction);
    BasicBlock *bbElse = else_stmt == NIL ? NIL : BasicBlock::Create(*TheContext, "neuq_jintao_else");
    BasicBlock *bbIfEnd = BasicBlock::Create(*TheContext, "neuq_jintao_if_end");
    LOCALS->setContextType(CodeGenBlockContextType::IF);
    LOCALS->context.ifCodeGenBlockContext = new IfCodeGenBlockContext(Builder->GetInsertBlock(), bbIfEnd, bbIfTrue,
                                                                      bbElse);
    // 创建条件分支,createCondBr(条件结果，为true的basic block，为false的basic block)
    if (bbElse == NIL) {
        Builder->CreateCondBr(ifCondV, bbIfTrue, bbIfEnd);
    } else {
        Builder->CreateCondBr(ifCondV, bbIfTrue, bbElse);
    }
    // 开启往Basic Block中塞入代码
    Builder->SetInsertPoint(bbIfTrue);
    Value *ifTrueV = if_stmt->codegen();
    if (!ifTrueV) {
        ABORT_COMPILE;
        return LogErrorV("if里面的内容有误");
    }
    // if 中为true的代码写完了，将if跳转到else后面，也就是IfEnd结点。这符合逻辑，同时也是LLVM的强制要求，每个Basic Block必须以跳转结尾
    if (Builder->GetInsertBlock()->empty() || !(*Builder->GetInsertBlock()->rbegin()).isTerminator()) {
        Builder->CreateBr(bbIfEnd);
    }
    // 将插入if内容时，可能会有循环嵌套，需要更新嵌套最底层的insertBlock
    bbIfTrue = Builder->GetInsertBlock();
    // 塞入else
    if (bbElse != NIL) {
        theFunction->getBasicBlockList().push_back(bbElse);
        Builder->SetInsertPoint(bbElse);
        Value *elseV = nullptr;
        if (else_stmt) {
            elseV = else_stmt->codegen();
            if (!elseV) {
                ABORT_COMPILE;
                return LogErrorV("else里面内容有误");
            }
        }
        if (Builder->GetInsertBlock()->empty() || !(*Builder->GetInsertBlock()->rbegin()).isTerminator()) {
            Builder->CreateBr(bbIfEnd);
        }
        bbElse = Builder->GetInsertBlock();
    }
    // 创建末尾结点
    theFunction->getBasicBlockList().push_back(bbIfEnd);
    Builder->SetInsertPoint(bbIfEnd);
//    PHINode *pn;
//    if (elseV){
//        pn = Builder->CreatePHI(ifTrueV->getType(), 2, "neuq_jintao_iftmp");
//    } else {
//        pn = Builder->CreatePHI(ifTrueV->getType(), 1, "neuq_jintao_iftmp");
//    }
//    pn->addIncoming(ifTrueV, bbIfTrue);
//    if (elseV){
//        pn->addIncoming(elseV, bbElse);
//    }
    TheCodeGenContext->pop_block();
    return bbIfEnd;
}

ConditionAST::ConditionAST(ExpressionAST *ifCond, BlockAST *ifStmt, BlockAST *elseStmt) : if_cond(ifCond),
                                                                                          if_stmt(ifStmt),
                                                                                          else_stmt(elseStmt) {}

string ConditionAST::toString() {
    return "条件结点";
}

Value *ForExprAST::codegen() {
    Function *theFuntion = Builder->GetInsertBlock()->getParent();
    if (theFuntion == nullptr) {
        ABORT_COMPILE;
        return LogErrorV("for 需要在函数语境下使用");
    }
    // 先执行for(xxx;;)
    auto tmpForBlock = BasicBlock::Create(*TheContext, "bb_for_start", theFuntion);
    TheCodeGenContext->push_block(tmpForBlock);

    auto bbStart = tmpForBlock;
    auto bbCond = BasicBlock::Create(*TheContext, "bb_for_cond", theFuntion);
    auto bbStep = BasicBlock::Create(*TheContext, "bb_for_step", theFuntion);
    auto bbBody = BasicBlock::Create(*TheContext, "bb_for_body", theFuntion);
    auto bbEndFor = BasicBlock::Create(*TheContext, "bb_for_end");
    LOCALS->setContextType(CodeGenBlockContextType::FOR);
    LOCALS->context.forCodeGenBlockContext = new ForCodeGenBlockContext(bbStart, bbCond, bbStep, bbBody, bbEndFor);
    Builder->CreateBr(bbStart);
    Builder->SetInsertPoint(bbStart);
    auto startV = Start->codegen();
    if (startV == nullptr) {
        ABORT_COMPILE;
        return LogErrorV("for(x;;){}中x有误");
    }
    Builder->CreateBr(bbCond);

    Builder->SetInsertPoint(bbCond);
    auto condV = Cond->codegen();
    if (condV == nullptr) {
        ABORT_COMPILE;
        return LogErrorV("for(;x;){}中x有误");
    }

    condV = Builder->CreateICmpNE(condV, ConstantInt::get(getTypeFromStr("bool"), 0), "neuq_jintao_ifcond");
    Builder->CreateCondBr(condV, bbBody, bbEndFor);

    Builder->SetInsertPoint(bbBody);
    auto body = Body->codegen();
    if (body == nullptr) {
        ABORT_COMPILE;
        return LogErrorV("for执行内容有误");
    }
    auto ins = Builder->GetInsertBlock()->rbegin();
    if (ins == Builder->GetInsertBlock()->rend() || !(*ins).isTerminator()) {
        Builder->CreateBr(bbStep);
    }
    Builder->SetInsertPoint(bbStep);
    auto stepV = Step->codegen();
    if (stepV == nullptr) {
        ABORT_COMPILE;
        return LogErrorV("for(;;x){}中x有误");
    }
    Builder->CreateBr(bbCond);
    theFuntion->getBasicBlockList().push_back(bbEndFor);
    Builder->SetInsertPoint(bbEndFor);
    TheCodeGenContext->pop_block();
    return bbEndFor;
}

ForExprAST::ForExprAST(NodeAST *start, NodeAST *end, NodeAST *step, BlockAST *body) : Start(start),
                                                                                      Cond(end),
                                                                                      Step(step),
                                                                                      Body(body) {}

string ForExprAST::toString() {
    return "循环结点";
}


const std::string &PrototypeAST::getName() const {
    return name;
}

PrototypeAST::PrototypeAST(const string &returnType, const string &name, const vector<VariableDeclarationAST *> &args)
        : returnType(returnType), name(name), args(args) {}

string PrototypeAST::toString() {
    return "函数描述结点：" + this->getName() + " ret:" + returnType + " params:" + to_string(args.size());
}

void FunctionAST::cleanCodeGenContext() {
    // 清除
    TheCodeGenContext->setRetBb(NIL);
    TheCodeGenContext->pop_block();
    TheCodeGenContext->removeFunction();
}

llvm::Function *FunctionAST::codegen() {
    auto name = Proto->getName();
    llvm::Function *function = TheModule->getFunction(name);
    // 设置返回值block
    auto retBB = BasicBlock::Create(*TheContext, "function_ret");
    TheCodeGenContext->setRetBb(retBB);

    if (!function)
        function = Proto->codegen();

    if (!function) {
        ABORT_COMPILE;
        return nullptr;
    }
    // 如果函数不为空，则表示已定义过了
    if (!function->empty())
        return (llvm::Function *) LogErrorV("Function cannot be redefined.");

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "alloca", function);
    Builder->SetInsertPoint(BB);
    TheCodeGenContext->push_block(BB);
    TheCodeGenContext->setFunction(function);

    // Record the function argnts in the NamedValues map.
//    auto funcVars = TheCodeGenContext->get_current_locals()->localVars;
    for (auto &Arg : function->args()) {
        // 分配一片内存
        auto mem = Builder->CreateAlloca(Arg.getType());
        Builder->CreateStore(&Arg, mem);
        LOCALSVARS.insert(make_pair(Arg.getName().data(), mem));
    }
    auto bb_entry = BasicBlock::Create(*TheContext, "neuq_entry", function);
    Builder->CreateBr(bb_entry);
    Builder->SetInsertPoint(bb_entry);
    // 把return值alloc一下，如果不是void
    if (!function->getReturnType()->isVoidTy()) {
        Value *ret = Builder->CreateAlloca(function->getReturnType());
        Builder->CreateStore(Constant::getNullValue(function->getReturnType()), ret);
        TheCodeGenContext->setRetV(ret);
    }
    if (Body->codegen()) {
        // 结束函数，创建return结点
        if (Builder->GetInsertBlock()->empty() || !Builder->GetInsertBlock()->rbegin()->isTerminator()) {
            Builder->CreateBr(retBB);
        }
        function->getBasicBlockList().push_back(retBB);
        Builder->SetInsertPoint(retBB);
        if (function->getReturnType()->isVoidTy()) {
            Builder->CreateRetVoid();
        } else {
            Builder->CreateRet(Builder->CreateLoad(TheCodeGenContext->getRetV()));
        }
        auto isNotValid = verifyFunction(*function, &errs());
        if (isNotValid) {
            LogError(("function '" + Proto->getName() + "' not valid\n").c_str());
            errs() << "function ll codes:\n";
            function->print(errs());
            ABORT_COMPILE;
            goto clean;
        } else {
            cleanCodeGenContext();
        }
        return function;
    }
    clean:
    cleanCodeGenContext();
    // Error reading body, remove function.
    function->eraseFromParent();
    return nullptr;
}

FunctionAST::FunctionAST(PrototypeAST *proto, BlockAST *body) : Proto(proto), Body(body) {}

string FunctionAST::toString() {
    return "函数结点：\n" + Proto->toString() + "\n" + Body->toString();
}

Value *BlockAST::codegen() {
    auto func = TheCodeGenContext->getFunc();
    if (func == NIL) {
        auto iterator = statements.begin();
        Value *lastStatementValue;
        for (; iterator != statements.end(); iterator++) {
            if (IS_COMPILE_ABORTED) {
                break;
            }
            auto it = (*iterator);
            if (typeid(*it) == typeid(OutStmtAST)) {
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
#ifdef DEBUG_FLAG
            if (lastStatementValue != NIL) {
                lastStatementValue->print(outs());
            }
        }
#endif
        return lastStatementValue;
    } else {
        auto bb = BasicBlock::Create(*TheContext, "blk");
        Builder->CreateBr(bb);
        func->getBasicBlockList().push_back(bb);
        Builder->SetInsertPoint(bb);
        auto iterator = statements.begin();
        Value *lastStatementValue;
        for (; iterator != statements.end(); iterator++) {
            if (IS_COMPILE_ABORTED) {
                break;
            }
            auto it = (*iterator);
            lastStatementValue = (*iterator)->codegen();
#ifdef DEBUG_FLAG
            if (lastStatementValue != NIL) {
                lastStatementValue->print(outs());
            }
#endif
            if (typeid(*it) == typeid(OutStmtAST)) {
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
        auto v = expr == nullptr ? nullptr : expr->codegen();
        result = Builder->CreateStore(v, ori_v);
    } else {
        auto gv = TheModule->getNamedGlobal(identifier);
        if (gv) {
            result = Builder->CreateStore(expr->codegen(), gv);
        } else {
            ABORT_COMPILE;
            return LogErrorV((identifier + " is not defined!").c_str());
        }
    }
    return result;
}

string VariableAssignmentAST::toString() {
    return StatementAST::toString();
}

llvm::Value *VariableDeclarationAST::codegen() {
    Value *ret = nullptr;
    if (this->getName() == "ctotal") {
        ret = NIL;
    }
    if (!HASLOCALS) {
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier->identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier->identifier.c_str());
            ABORT_COMPILE;
            return nullptr;
        } else {
            auto *gv = new GlobalVariable(*TheModule, getTypeFromStr(type), isConst,
                                          GlobalVariable::LinkageTypes::ExternalLinkage,
                                          Constant::getNullValue(getTypeFromStr(type)), identifier->identifier);
            if (expr != nullptr) {
                auto v = expr->codegen();
                if (IS_COMPILE_ABORTED) {
                    return NIL;
                }
                auto val = dyn_cast<Constant>(expr->codegen());
                gv->setInitializer(val);
            }
            ret = gv;
        }
    } else {
        if (LOCALSVARS.find(identifier->identifier) == LOCALSVARS.end()) {
            ret = Builder->CreateAlloca(getTypeFromStr(type));
//            identifier.
//            auto mem = Builder->CreateAlloca(getTypeFromStr(type),)
            if (expr != nullptr) {
                auto v = expr->codegen();
                if (IS_COMPILE_ABORTED) {
                    return NIL;
                }
                if (v->getType()->isIntegerTy() && v->getType() != getTypeFromStr(type)) {
                    // 不同，尝试进行修剪
                    v = Builder->CreateZExtOrTrunc(v, getTypeFromStr(type));
                }
                Builder->CreateStore(v, ret);
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

std::string VariableDeclarationAST::getName() {
    return identifier->identifier;
}

string VariableDeclarationAST::toString() {
    string s = "变量声明：";
    return s + this->getName();
}

llvm::Value *FuncPtrAST::codegen() {
    Value *ret = nullptr;
    if (this->getName() == "ctotal") {
        ret = NIL;
    }
    if (!HASLOCALS) {
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier->identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier->identifier.c_str());
            ABORT_COMPILE;
            return nullptr;
        } else {
            auto *gv = new GlobalVariable(*TheModule, getTypeFromStr(type), isConst,
                                          GlobalVariable::LinkageTypes::ExternalLinkage,
                                          Constant::getNullValue(getTypeFromStr(type)), identifier->identifier);
            if (expr != nullptr) {
                auto v = expr->codegen();
                if (IS_COMPILE_ABORTED) {
                    return NIL;
                }
                auto val = dyn_cast<Constant>(expr->codegen());
                gv->setInitializer(val);
            }
            ret = gv;
        }
    } else {
        if (LOCALSVARS.find(identifier->identifier) == LOCALSVARS.end()) {
            std::vector<llvm::Type *> putArgs;
            for (int i = 1; i < args.size(); i++) {
                putArgs.push_back(getTypeFromStr(args[i]->identifier));
            }
            llvm::ArrayRef<llvm::Type *> argsRef(putArgs);
            llvm::FunctionType *funcPtrType = llvm::FunctionType::get(getTypeFromStr(args[0]->identifier),
                                                                      argsRef,
                                                                      false);

            ret = Builder->CreateAlloca(funcPtrType->getPointerTo());
//            identifier.
//           auto mem = Builder->CreateAlloca(getTypeFromStr(type),)
            INSERTLOCAL(identifier->identifier, ret);
        }
    }
    return ret;
}

llvm::Value *IntegerExprAST::codegen() {
    return ConstantInt::get(Type::getInt32Ty(*TheContext), Val, true);
}

string IntegerExprAST::toString() {
    return string("整型：" + to_string(Val));
}

llvm::Value *IdentifierExprAST::codegen() {
    if (HASLOCALS) {
        auto local_mem = FINDLOCAL(identifier);
        if (local_mem != nullptr) {
            auto v = Builder->CreateLoad(local_mem);
            if (v->getType()->isArrayTy()) {
                // 数组兜底
                v->eraseFromParent();
                // 0取数组，后0为取第一个元素所在地址
                return Builder->CreateInBoundsGEP(local_mem,
                                                  {ConstantInt::get(getTypeFromStr("int"), 0),
                                                   ConstantInt::get(getTypeFromStr("int"), 0)});
            }
            return v;
        }
    }
    // 可能是全局变量
    auto gv = TheModule->getNamedGlobal(identifier);
    if (gv) {
        return Builder->CreateLoad(gv);
    }
    // 可能是函数指针
    auto func = TheModule->getFunction(identifier);
    if (func != NIL) {
        return func;
    }
    ABORT_COMPILE;
    return LogErrorV((identifier + " is not defined!!!").c_str());
}

IdentifierExprAST::IdentifierExprAST() {

}

ReturnStmtAST::ReturnStmtAST(ExpressionAST *expr) : expr(expr) {}

ReturnStmtAST::ReturnStmtAST() {
    expr = nullptr;
}

llvm::Value *ReturnStmtAST::codegen() {
    auto ret_bb = TheCodeGenContext->getRetBb();
    if (ret_bb == NIL) {
        ABORT_COMPILE;
        return LogErrorV("return是否用在了非函数环境下？");
    }
    if (expr == nullptr) {
        return Builder->CreateBr(ret_bb);
    } else {
        auto ret_addr = TheCodeGenContext->getRetV();
        if (ret_addr == NIL) {
            ABORT_COMPILE;
            return LogErrorV("无返回值分配域？请联系作者");
        }
        auto v = expr->codegen();
        if (v == NIL) {
            ABORT_COMPILE;
            return LogErrorV("函数返回的值为空，可能是值解析失败，或变量不存在");
        } else {
            // 当返回递归函数
            if (typeid(*expr) == typeid(CallExprAST)) {
                auto call_expr = dynamic_cast<CallExprAST *>(expr);
                if (call_expr->getCallName() == TheCodeGenContext->getFunc()->getName()) {
                    dyn_cast<CallInst>(v)->setTailCall(true);
                }
            }
            if (!v->getType()->isVoidTy()) {
                Builder->CreateStore(v, ret_addr);
            }
            Builder->CreateBr(ret_bb);
            return v;
        }
    }
}

string ReturnStmtAST::toString() {
    return string("ret：" + string(expr == NIL ? "void" : expr->toString()));
}

Type *getTypeFromStr(const std::string &type) {
    if (type == "int") {
        return Type::getInt32Ty(*TheContext);
    } else if (type == "long") {
        return Type::getInt64Ty(*TheContext);
    } else if (type == "short") {
        return Type::getInt8Ty(*TheContext);
    } else if (type == "bool") {
        return Type::getInt1Ty(*TheContext); // 本质上就是1字节的int
    } else if (type == "void") {
        return Type::getVoidTy(*TheContext);
    } else if (type == "double") {
        return Type::getDoubleTy(*TheContext);
    } else if (type == "float") {
        return Type::getFloatTy(*TheContext);
    } else if (type == "char") {
        // 8b
        return Type::getInt8Ty(*TheContext);
    } else if (type == "str") {
        // 8b的指针
        return Type::getInt8Ty(*TheContext)->getPointerTo();
    }
    return NIL;
}

// int a[5][6][7] -> vector<Expr>(7,6,5)
ArrayType *buildArrayType(vector<ExpressionAST *> *vec, Type *arrayElemType) {
    ArrayType *arr_type = NIL;
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
            ABORT_COMPILE;
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
    if (local == NIL) {
        return LogErrorV(("未找到" + identifier + "\n").c_str());
    }
    auto it = arrIndex->begin();
    auto v_vec = vector<Value *>();
    // 添加0取指针地址
    // FIXME 此处特判了如果是连续的两个pointerType，则判断可能传入的是一个CreateStore后的值，此时进行Load（该方案拓展性差，后期改良方案）
    if (local->getType()->isPointerTy() && local->getType()->getContainedType(0)->isPointerTy()) {
        local = Builder->CreateLoad(local);
    } else {
        v_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    }
    Value *ret = local;
//    if (!(local->getType()->isPointerTy())) {
//        v_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
//    }
    // 兜底，目前放入local内的数组
    for (; it < arrIndex->end(); it++) {
        auto v = (*it)->codegen();
        if (v == nullptr) {
            ABORT_COMPILE;
            return LogErrorV("数组解析失败");
        }
        v_vec.push_back(v);
    }
    ret = Builder->CreateInBoundsGEP(ret, ArrayRef<Value *>(v_vec));
    return Builder->CreateLoad(ret);
}

IdentifierArrExprAST::IdentifierArrExprAST(const string &identifier, vector<ExpressionAST *> *arrIndex)
        : IdentifierExprAST(identifier), arrIndex(arrIndex) {}

string IdentifierArrExprAST::toString() {
    return string("数组标识符：") + identifier;
}


llvm::Value *VariableArrDeclarationAST::codegen() {
    Value *ret = nullptr;
    ArrayType *arr_type = buildArrayType(identifier->arrIndex, getTypeFromStr(type));
    if (!HASLOCALS) {
        vector<Constant *> *ca = genGlobalExprs();
        auto constant_arr_value = ConstantArray::get(arr_type, ArrayRef(*ca));
        // 全局变量
        auto global_variable = TheModule->getNamedGlobal(identifier->identifier);
        if (global_variable) {
            fprintf(stderr, "variable %s redefined", identifier->identifier.c_str());
            return nullptr;
        } else {
            auto gv = new GlobalVariable(*TheModule, arr_type, true,
                                         GlobalVariable::LinkageTypes::ExternalLinkage,
                                         Constant::getNullValue(arr_type), identifier->identifier);
            auto vs = ArrayRef(*ca);
            gv->setInitializer(constant_arr_value);
//            gv->print(outs(), true);
            ret = gv;
        }
    } else {
        if (LOCALSVARS.find(identifier->identifier) == LOCALSVARS.end()) {
            auto mem = Builder->CreateAlloca(arr_type);
            genLocalStoreExprs(mem);
            INSERTLOCAL(identifier->identifier, mem);
            ret = mem;
        }
    }
    return ret;
}

VariableArrDeclarationAST::VariableArrDeclarationAST(const string &type, IdentifierArrExprAST *identifier,
                                                     vector<NodeAST *> *exprs, bool isConst) : type(type),
                                                                                               identifier(identifier),
                                                                                               exprs(exprs),
                                                                                               isConst(isConst) {}

vector<Constant *> *VariableArrDeclarationAST::genGlobalExprs() {
//    vector<uint64_t> vmaxindex = getIndexVal();
//    vector<uint64_t> vindex(vmaxindex.size(),0);
    if (exprs == NIL && exprs->empty()) {
        return NIL;
    }
    auto arr_index_value_vector = new vector<Constant *>();
//    for (auto exp = exprs->begin(); exp != exprs->end() ; exp++) {
//
//    }
    for_each(exprs->begin(), exprs->end(), [&](NodeAST *e) -> void {
        auto v = e->codegen();
        if (!isa<Constant>(v)) {
            if (v->getType()->isIntegerTy()) {
//                auto id = typeid(v);
                auto ty = dyn_cast<IntegerType>(v->getType());
                if (ty == NIL) {
                    ABORT_COMPILE;
                    LogErrorV("全局数组初始化必须为常量声明");
                    return;
                } else {
                    // TODO FixME!
                    auto info = typeid(v).name();
                    v = ConstantInt::get(getTypeFromStr("int"), 0);
                }
            } else {
                ABORT_COMPILE;
                LogErrorV("全局数组初始化必须为常量声明");
                return;
            }
        }
        arr_index_value_vector->push_back(dyn_cast<Constant>(v));
    });
    return arr_index_value_vector;
}

void VariableArrDeclarationAST::genLocalStoreExprs(Value *mem) {
    if (exprs == NIL && exprs->empty()) {
        return;
    }
    vector<uint64_t> vmaxindex = getIndexVal();
    vector<uint64_t> vindex(vmaxindex.size(), 0);
    for (auto e : *exprs) {
        if (e != NIL) {
            vector<Value *> gepindex_vec;
            gepindex_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
            for (auto index : vindex) {
                auto intv = ConstantInt::get(Type::getInt32Ty(*TheContext), index);
                gepindex_vec.push_back(intv);
            }
            auto ret = Builder->CreateInBoundsGEP(mem, ArrayRef(gepindex_vec));
            Builder->CreateStore(e->codegen(), ret);
            if (incrementVectorIndex(vindex, vmaxindex)) {
//                LogWarn("数组声明超过定义，超过定义的值将被忽略");
                break;
            }
        } else {
            // TODO Fixme
        }
    }
}

int VariableArrDeclarationAST::incrementVectorIndex(vector<uint64_t> &indexVec, vector<uint64_t> &maxVec) {
    assert(indexVec.size() == maxVec.size());
    int sz = indexVec.size() - 1;
    bool is_max = true;
    for (int tsz = 0; tsz <= sz; ++tsz) {
        if (indexVec.at(tsz) != maxVec.at(tsz) - 1) {
            is_max = false;
            break;
        }
    }
    int carry = 1;
    if (is_max) {
        return carry;
    }
    while (carry > 0 && sz >= 0) {
        if (indexVec.at(sz) + 1 < maxVec.at(sz)) {
            indexVec[sz]++;
            carry = 0;
            break;
        } else {
            indexVec[sz] = 0;
            sz--;
        }
    }
    return carry;
}

vector<uint64_t> VariableArrDeclarationAST::getIndexVal() {
    vector<uint64_t> vmaxindex;
    // 获取index的vector
    for_each(identifier->arrIndex->begin(), identifier->arrIndex->end(), [&](ExpressionAST *e) {
        if (e == NIL) {
            if (vmaxindex.empty()) {
                vmaxindex.push_back(INT_MAX);
            } else {
                ABORT_COMPILE;
                LogError("只允许数组index首部为空值，要不然无法编译，我就罢工了");
                return;
            }
        } else {
            auto value_index = dyn_cast<ConstantInt>(e->codegen());
            if (value_index == NIL) {
                ABORT_COMPILE;
                LogError("声明一个数组长度不能使用非常量");
                return;
            } else {
                if (value_index->getBitWidth() <= 32) {
                    vmaxindex.push_back(value_index->getSExtValue());
                } else {
                    vmaxindex.push_back(value_index->getZExtValue());
                }
            }
        }
    });
    return vmaxindex;
}

Type *VariableArrDeclarationAST::buildPointerTy() {
    auto idx_nums = getIndexVal();
    if (idx_nums.empty()) {
        return NIL;
    }
    Type *type = getTypeFromStr(this->type);
    int length = idx_nums.size();
    for (int i = length - 1; i > 0; i--) {
        if (idx_nums[i] == INT_MAX) {
            ABORT_COMPILE;
            return LogErrorTy("数组索引值只有第一个可以为空!(第一个索引我不管，你随便)");
        }
        type = ArrayType::get(type, idx_nums[i]);
    }
    return type->getPointerTo();
}

string VariableArrDeclarationAST::getName() {
    return identifier->identifier;
}

string VariableArrDeclarationAST::toString() {
    return identifier->identifier;
}

string FuncPtrAST::getName() {
    return identifier->identifier;
}

VariableArrAssignmentAST::VariableArrAssignmentAST(IdentifierArrExprAST *identifier, ExpressionAST *expr) : identifier(
        identifier), expr(expr) {}

llvm::Value *VariableArrAssignmentAST::codegen() {
    auto arr_addr = FINDLOCAL(identifier->identifier);
    arr_addr = Builder->CreateLoad(arr_addr);
    if (arr_addr == nullptr) {
        ABORT_COMPILE;
        return LogErrorV((identifier->identifier + "is not defined").c_str());
    }
    auto st = identifier->arrIndex->begin();
    Value *ret = arr_addr;
    vector<Value *> vec;
    // 0取地址，如果是i8*这种就不用，如果是[i8 x n]*就得加0
    // FIXME: 不优雅的特判
    if (arr_addr->getType()->isArrayTy()) {
        vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    }
    for (; st != identifier->arrIndex->end(); st++) {
        auto v = (*st)->codegen();
        vec.push_back(v);
    }
    ret = Builder->CreateInBoundsGEP(arr_addr, ArrayRef(vec));
    auto newV = expr->codegen();
    ret = Builder->CreateStore(newV, ret);
    return ret;
}

string VariableArrAssignmentAST::toString() {
    return string("数组变量赋值") + identifier->toString();
}

OutStmtAST::OutStmtAST() = default;

Value *OutStmtAST::codegen() {
    auto blk = TheCodeGenContext->findTopLoopCodeGenBlockTypeBlock();
    if (blk == NIL) {
        ABORT_COMPILE;
        return LogErrorV("break 不能用作于当前的代码上下文中，请检查");
    }
    switch (blk->contextType) {
        case NONE:
            return LogErrorV("程序匹配的break环境与当前域不符(NONE)，请联系作者");
        case FOR:
            if (blk->context.forCodeGenBlockContext == NIL
                || blk->context.forCodeGenBlockContext->bbEndFor == NIL) {
                ABORT_COMPILE;
                return LogErrorV("程序匹配的上下文context为NULL，请联系作者");
            }
            return Builder->CreateBr(blk->context.forCodeGenBlockContext->bbEndFor);
        case IF:
            return LogErrorV("程序匹配的break环境与当前域不符(IF)，请联系作者");
        case WHILE:
            if (blk->context.whileCodeGenBlockContext == NIL
                || blk->context.whileCodeGenBlockContext->bbEndWhile == NIL) {
                ABORT_COMPILE;
                return LogErrorV("程序匹配的上下文context为NULL(while continue;)，请联系作者");
            }
            return Builder->CreateBr(blk->context.whileCodeGenBlockContext->bbEndWhile);
        default:
            ABORT_COMPILE;
            return LogErrorV("Break在未知的代码域中，或编译器目前无法处理的关键字");
    }
    return NIL;
}

string OutStmtAST::toString() {
    return "跳出语句";
}

ContinueStmtAST::ContinueStmtAST() = default;

Value *ContinueStmtAST::codegen() {
    auto blk = TheCodeGenContext->findTopLoopCodeGenBlockTypeBlock();
    if (blk == NIL) {
        ABORT_COMPILE;
        return LogErrorV("break 不能用作于当前的代码上下文中，请检查");
    }
    switch (blk->contextType) {
        case NONE:
            ABORT_COMPILE;
            return LogErrorV("程序匹配的break环境与当前域不符(NONE)，请联系作者");
        case FOR:
            if (blk->context.forCodeGenBlockContext == NIL
                || blk->context.forCodeGenBlockContext->bbEndFor == NIL) {
                ABORT_COMPILE;
                return LogErrorV("程序匹配的上下文context为NULL，请联系作者");
            }
            return Builder->CreateBr(blk->context.forCodeGenBlockContext->bbStep);
        case IF:
            ABORT_COMPILE;
            return LogErrorV("程序匹配的break环境与当前域不符(IF)，这可能是个bug，请联系作者");
        case WHILE:
            if (blk->context.whileCodeGenBlockContext == NIL
                || blk->context.whileCodeGenBlockContext->bbCond == NIL) {
                ABORT_COMPILE;
                return LogErrorV("程序匹配的上下文context为NULL(while continue;)，请联系作者");
            }
            return Builder->CreateBr(blk->context.whileCodeGenBlockContext->bbCond);
        default:
            ABORT_COMPILE;
            return LogErrorV("Break在未知的代码域中，或编译器目前无法处理的关键字");
    }
    return NIL;
}

string ContinueStmtAST::toString() {
    return "继续";
}

WhileStmtAST::WhileStmtAST(NodeAST *cond, BlockAST *body) : Cond(cond), Body(body) {}

llvm::Value *WhileStmtAST::codegen() {
    auto function = Builder->GetInsertBlock()->getParent();
    if (function == NIL) {
        ABORT_COMPILE;
        return LogErrorV("while 不能用在非函数体中");
    }
    BasicBlock *bbCond = BasicBlock::Create(*TheContext, "while_cond", function);
    TheCodeGenContext->push_block(bbCond);
    BasicBlock *bbBody = BasicBlock::Create(*TheContext, "while_body");
    BasicBlock *bbEndWhile = BasicBlock::Create(*TheContext, "while_end");
    LOCALS->setContextType(CodeGenBlockContextType::WHILE);
    LOCALS->context.whileCodeGenBlockContext = new WhileCodeGenBlockContext(bbCond, bbBody, bbEndWhile);
    // 创建条件分支
    Builder->CreateBr(bbCond);
    Builder->SetInsertPoint(bbCond);
    auto condV = Cond->codegen();
    if (condV == NIL) {
        ABORT_COMPILE;
        return LogErrorV("while中的判断语句解析失败");
    }
    condV = Builder->CreateICmpNE(condV, ConstantInt::get(getTypeFromStr("bool"), 0));
    Builder->CreateCondBr(condV, bbBody, bbEndWhile);
    function->getBasicBlockList().push_back(bbBody);
    Builder->SetInsertPoint(bbBody);
    auto bodyV = Body->codegen();
    if (bodyV == NIL) {
        ABORT_COMPILE;
        return LogErrorV("while中的判断语句解析失败");
    }
    if (Builder->GetInsertBlock()->empty() || !Builder->GetInsertBlock()->rbegin()->isTerminator()) {
        Builder->CreateBr(bbCond);
    }
    function->getBasicBlockList().push_back(bbEndWhile);
    Builder->SetInsertPoint(bbEndWhile);
    TheCodeGenContext->pop_block();
    return bbEndWhile;
}

string WhileStmtAST::toString() {
    string s = "循环：\n条件:\n";
    s.append(this->Cond->toString());
    s.append("\n执行域：" + this->Body->toString());
    return s;
}

string ExpressionAST::toString() {
    return std::string();
}

string StatementAST::toString() {
    return std::string();
}

string ExpressionStatementAST::toString() {
    return std::string("变量声明：" + expr->toString());
}

string BlockAST::toString() {
    string s = "block域内容：\n";
    for (auto st : statements) {
        s.append(st->toString() + "\n");
    }
    return s;
}

string IdentifierExprAST::toString() {
    return string("标识符：") + identifier;
}

llvm::Value *StringExprAST::codegen() {
    GlobalVariable *gv;
    if (TheCodeGenContext->getFunc() == NIL) {
        auto sz = str->size();
        auto arr_type = ArrayType::get(getTypeFromStr("char"), sz + 1);
        gv = new GlobalVariable(*TheModule, arr_type,
                                true, GlobalValue::PrivateLinkage, ConstantDataArray::getString(*TheContext, *str),
                                getUniqueId());
//        for (int i = 0; i < sz; ++i) {
//            uint8_t ch = str->at(i);
//            v.push_back(ConstantInt::get(getTypeFromStr("char"), ch));
//        }
        // 反斜杠0
//        v.push_back(ConstantInt::get(getTypeFromStr("char"), '\0'));
        gv->setUnnamedAddr(GlobalVariable::UnnamedAddr::Global);
        return ConstantExpr::getBitCast(gv, getTypeFromStr("str"));
    } else {
        gv = Builder->CreateGlobalString(*str, StringExprAST::getUniqueId());
    }
    return Builder->CreateInBoundsGEP(gv, {ConstantInt::get(getTypeFromStr("int"), 0),
                                           ConstantInt::get(getTypeFromStr("int"), 0)});
}

string StringExprAST::getUniqueId() {
    return std::string("_str_") + to_string(StringExprAST::id++);
}

void StringExprAST::setUniqueId(long id) {
    StringExprAST::id = id;
}

StringExprAST::StringExprAST(string *str) : str(str) {}

string StringExprAST::toString() {
    return str == NIL ? "" : *str;
}

string NullExprAST::toString() {
    return "空值";
}

llvm::Value *NullExprAST::codegen() {
    return ConstantExpr::getNullValue(getTypeFromStr("char")->getPointerTo());
}

llvm::Value *BoolExprAST::codegen() {
    return is_true ? ConstantInt::get(getTypeFromStr("bool"),1) : ConstantInt::get(getTypeFromStr("bool"),0);
}

string BoolExprAST::toString() {
    return std::to_string(is_true);
}
