//
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_GLOBAL_H
#define LLVM_KALEIDOSCOPE_GLOBAL_H

#include "CodeGen.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

/// 状态字
#define OK 0
#define ERR -1
#define DEBUG_FLAG 1

/// 初始化数字
#define INIT_NUM -1

/// 特性前缀
#define LabelPrefix "neuq_kingtous_"
/// 全局变量
#define HASLOCALS (TheCodeGenContext.get_current_locals() != nullptr)
#define LOCALS TheCodeGenContext.get_current_locals()->localVars
#define INSERTLOCAL(identifier, llvmValue) TheCodeGenContext.get_current_locals()->localVars.insert(std::make_pair(identifier,llvmValue))
#define FINDLOCAL(x) TheCodeGenContext.findValue(x)
extern void yyerror(const char *s);

/// 全局变量
/// TheContext 是一个不透明的对象，拥有许多核心LLVM数据结构，例如类型和常量值表。
extern llvm::LLVMContext TheContext;
/// 该Builder对象是一个辅助对象，可以轻松生成LLVM指令。IRBuilder 类模板的实例 跟踪插入指令的当前位置，并具有创建新指令的方法。
extern llvm::IRBuilder<> Builder;
/// TheModule是一个包含函数和全局变量的LLVM构造。在许多方面，它是LLVM IR用于包含代码的顶级结构。它将拥有我们生成的所有IR的内存，这就是codegen（）方法返回原始值*而不是unique_ptr 的原因。
extern std::unique_ptr<llvm::Module> TheModule;
/// 跟踪哪些值在当前范围以及他们LLVM表示被定义。（换句话说，它是代码的符号表）。
extern std::map<std::string, llvm::Value *> NamedValues;
/// 遍优化器
extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
/// 代码生成上下文
extern CodeGenContext TheCodeGenContext;
#endif //LLVM_KALEIDOSCOPE_GLOBAL_H
