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
            case BinaryType::divi:
                return Builder.CreateSDiv(L, R, "divi");
            case BinaryType::mod:
                return Builder.CreateSRem(L, R,
                                          "mod");
            case BinaryType::less:
                return Builder.CreateICmpSLT(L, R, "less_than");
            case BinaryType::equ:
                return Builder.CreateICmpEQ(L, R, "equ");
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
            case BinaryType::divi:
                return Builder.CreateFDiv(L, R, "divi");
            case BinaryType::mod:
                return Builder.CreateFRem(L, R, "mod");
            case BinaryType::less:
                return Builder.CreateFCmpOLT(L, R, "less_than");
            case BinaryType::equ:
                return Builder.CreateFCmpOEQ(L, R, "ordered_fequ");
            default:
                return LogErrorV(("invalid binary operator"));
        }
    }

}

BinaryExprAST::BinaryExprAST(BinaryType type, ExpressionAST *lea, ExpressionAST *rea) : type(type), LEA(lea),
                                                                                        REA(rea) {}

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
    ifCondV = Builder.CreateICmpNE(ifCondV, ConstantInt::get(getTypeFromStr("bool"),0), "neuq_jintao_ifcond");
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
    Value *elseV = nullptr;
    if (else_stmt){
        elseV = else_stmt->codegen();
        if (!elseV) {
            return LogErrorV("else里面内容有误");
        }
    }
    Builder.CreateBr(bbIfEnd);
    bbElse = Builder.GetInsertBlock();
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
    auto bbCond = BasicBlock::Create(TheContext, "bb_cond", theFuntion);
    auto bbStep = BasicBlock::Create(TheContext, "bb_step", theFuntion);
    auto bbBody = BasicBlock::Create(TheContext, "bb_body", theFuntion);
    auto bbEndFor = BasicBlock::Create(TheContext, "bb_end", theFuntion);

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

    condV = Builder.CreateICmpNE(condV, ConstantInt::get(getTypeFromStr("bool"),0), "neuq_jintao_ifcond");
    Builder.CreateCondBr(condV,bbBody,bbEndFor);

    Builder.SetInsertPoint(bbBody);
    auto body = Body->codegen();
    if (body == nullptr){
        return LogErrorV("for执行内容有误");
    }

    Builder.CreateBr(bbStep);
    Builder.SetInsertPoint(bbStep);
    auto stepV = Step->codegen();
    if (stepV == nullptr){
        return LogErrorV("for(;;x){}中x有误");
    }
    Builder.CreateBr(bbCond);
    Builder.SetInsertPoint(bbEndFor);
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
    for (auto &Arg : function->args()){
        // 分配一片内存
        auto mem = Builder.CreateAlloca(Arg.getType());
        Builder.CreateStore(&Arg,mem);
        LOCALS.insert(make_pair(Arg.getName().data(),mem));
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
        if (LOCALS.find(identifier->identifier) == LOCALS.end()) {
            ret = Builder.CreateAlloca(getTypeFromStr(type));
//            identifier.
//            auto mem = Builder.CreateAlloca(getTypeFromStr(type),)
            if (expr != nullptr) {
                auto v = expr->codegen();
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
        if (LOCALS.find(identifier->identifier) == LOCALS.end()) {
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
    Type *elem_type = getArrayElemType(ret);
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
