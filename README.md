# sysy
使用LLVM框架的kaleidoscope语言编译器
---

LLVM框架语言：LLVM 11.1.0
编译环境：cmake 3.13 + clion 2020.2 + macOS 10.15.7/Deepin 20.1 Beta
> 编译时若出错请注意LLVM版本号

#### 目前支持程度

目前还不完善，支持简单的：

- 函数定义
    - 参数
- 二元表达式
- 函数调用
- 条件分支关键字
    - if
- 循环关键字
    - For
    - ~~While~~
    - ~~break~~
    - ~~continue~~
- 函数返回 return
- 全局、局部变量
- 生成AST语法树
- 生成对应系统架构的目标代码
- ...(比较懒，不想写)

