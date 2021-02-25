//
// Created by bytedance on 2021/2/24.
//

#include "TimeAnalysisPass.h"

#include "ErrHelper.h"

char TimeAnalysisPass::ID = 0;

bool TimeAnalysisPass::runOnFunction(Function &func) {
    // 确保可以使用printf
    auto printfFunc = ExternFunctionLinker::getOrAddPrintfFunc(TheContext, *TheModule);
    if (printfFunc == NIL) {
        LogError("函数printf未找到，设置的时间分析插值失效");
        return false;
    }
    auto bb_it = func.getBasicBlockList().begin();
    Builder.SetInsertPoint(&bb_it->front());

//    auto st_mem = Builder.CreateAlloca(getTypeFromStr("long"),NIL,LabelPrefix+string("st"));
    auto call_v = Builder.CreateCall(ExternFunctionLinker::getOrAddTimeFunc(TheContext, *TheModule),
                                     ConstantExpr::getNullValue(getTypeFromStr("long")->getPointerTo()));
    auto call_v_mem = Builder.CreateAlloca(getTypeFromStr("long"));
    Builder.CreateStore(call_v, call_v_mem);

    auto bb_it_end = func.getBasicBlockList().rbegin();
    Builder.SetInsertPoint(&(*bb_it_end->rbegin()));
    auto call_v_2 = Builder.CreateCall(ExternFunctionLinker::getOrAddTimeFunc(TheContext, *TheModule),
                                       ConstantExpr::getNullValue(getTypeFromStr("long")->getPointerTo()));
    auto call_v_2_mem = Builder.CreateAlloca(getTypeFromStr("long"));
    Builder.CreateStore(call_v_2, call_v_2_mem);

    auto time_v = Builder.CreateSub(Builder.CreateLoad(call_v_2_mem), Builder.CreateLoad(call_v_mem));
    auto time_mem = Builder.CreateAlloca(getTypeFromStr("long"));
    Builder.CreateStore(time_v, time_mem);

    auto stmt = TheModule->getGlobalVariable(LabelPrefix + string("time_cost"));
    if (stmt == NIL) {
        stmt = Builder.CreateGlobalString("function cost time(s): %ld", LabelPrefix + string("time_cost"));
    }
    std::vector<Constant *> args_vec;
    args_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    args_vec.push_back(ConstantInt::get(getTypeFromStr("int"), 0));
    auto stmt_ptr = Builder.CreateInBoundsGEP(stmt,{ConstantInt::get(getTypeFromStr("int"),0),ConstantInt::get(getTypeFromStr("int"),0)});
    Builder.CreateCall(printfFunc,
                       {stmt_ptr, Builder.CreateLoad(time_mem)});
    return false;
}

TimeAnalysisPass::TimeAnalysisPass() : FunctionPass(ID) {
}
