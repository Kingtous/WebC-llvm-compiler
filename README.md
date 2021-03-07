# sysy

使用LLVM框架的Sysy2020语言编译器
---

当前编译状态：![build status](https://github.com/Kingtous/Sysy2020-llvm-compiler/actions/workflows/cmake.yml/badge.svg)

LLVM框架语言：LLVM 12.0.0

编译环境：cmake 3.13 + Deepin 20.1 Beta
> 编译时若出错请注意LLVM版本号

#### 目前支持程度

- 函数定义
  - 参数
- 二元表达式
  - &&、||
  - +、-、*、/、%、!=
- 函数调用
  - 传参
    - 基础类型、数组、多维数组传参
- 条件分支关键字
  - if
- 循环关键字(c++ -> sysyplus)
  - for -> lp
  - While -> whl
  - break -> out
    - continue -> cont
- 函数返回 return -> ret
  - 保持单入单出特性
- 全局、局部变量
- 数组
  - 全局、局部数组/多维数组
  - 数组定义时初始化
- 字符串
  - 局部、全局声明、传值
- 语言特色
  - sleep(s) 休眠
  - echo(str) 输出
  - now(null) 获取当前秒数
- 生成AST语法树
- 生成对应系统架构的目标代码
- ...(比较懒，不想写)

