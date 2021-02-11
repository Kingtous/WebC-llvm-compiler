//
// Created by 金韬 on 2020/9/22.
//

#include <codegen/CodeGen.h>

#include "Global.h"

/// 全局变量
// TheContext 是一个不透明的对象，拥有许多核心LLVM数据结构，例如类型和常量值表。
llvm::LLVMContext TheContext;
/// 该Builder对象是一个辅助对象，可以轻松生成LLVM指令。IRBuilder 类模板的实例 跟踪插入指令的当前位置，并具有创建新指令的方法。
llvm::IRBuilder<> Builder = llvm::IRBuilder<>(TheContext);
/// TheModule是一个包含函数和全局变量的LLVM构造。在许多方面，它是LLVM IR用于包含代码的顶级结构。它将拥有我们生成的所有IR的内存，这就是codegen（）方法返回原始值*而不是unique_ptr 的原因。
std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("Kingtous JIT", TheContext);
/// 跟踪哪些值在当前范围以及他们LLVM表示被定义。（换句话说，它是代码的符号表）。
std::map<std::string, llvm::Value *> NamedValues = std::map<std::string, llvm::Value *>();
/// 遍优化器
std::unique_ptr<llvm::FunctionPassManager> TheFPM = std::make_unique<llvm::FunctionPassManager>(TheModule.get());
/// 代码生成上下文
CodeGenContext TheCodeGenContext;