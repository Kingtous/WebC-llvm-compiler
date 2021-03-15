//
// Created by 金韬 on 2020/9/22.
//

#include "codegen/CodeGen.h"

/// 全局变量
// TheContext 是一个不透明的对象，拥有许多核心LLVM数据结构，例如类型和常量值表。
std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
/// 该Builder对象是一个辅助对象，可以轻松生成LLVM指令。IRBuilder 类模板的实例 跟踪插入指令的当前位置，并具有创建新指令的方法。
std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
/// TheModule是一个包含函数和全局变量的LLVM构造。在许多方面，它是LLVM IR用于包含代码的顶级结构。它将拥有我们生成的所有IR的内存，这就是codegen（）方法返回原始值*而不是unique_ptr 的原因。
std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("Kingtous JIT", *TheContext);
/// 代码生成上下文
std::unique_ptr<CodeGenContext> TheCodeGenContext = std::make_unique<CodeGenContext>();