//
// Created by 金韬 on 2021/2/26.
//

#include "RecursiveToLoopPass.h"

char RecursiveToLoopPass::ID = 0;

// example:
//int loop_echo(int a,int b,int times){
//    if (times <= 0){
//        ret b;
//    }
//    ret loop_echo(b,a+b,times-1);
//}

/// 满足条件：最后一条是递归、标记为了tail
bool RecursiveToLoopPass::runOnFunction(Function &F) {
    auto rit = F.getBasicBlockList().rbegin();
    rit++;
    if (rit->empty()) {
        return false;
    }
    return false;
}

RecursiveToLoopPass::RecursiveToLoopPass() : FunctionPass(ID) {
}
