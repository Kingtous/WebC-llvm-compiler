# SysyPlus Language For Kingtous

使用LLVM框架的SysyPlus语言编译器
---

仓库地址：[Github Repo](https://github.com/Kingtous/Sysy2020-llvm-compiler)

当前编译状态：![build status](https://github.com/Kingtous/Sysy2020-llvm-compiler/actions/workflows/cmake.yml/badge.svg)

LLVM框架语言：LLVM 12.0.0

工具库依赖：libBoost、LLVM、glib

界面依赖：gtkmm3

编译环境：cmake 3.13 + Deepin 20.1 Beta
> 编译时若出错请注意LLVM版本号

#### 背景
SysyPlus编译器偏向于生成网络方面的执行代码，让小白开发者也可以做一个简单、易用、高性能、安全的http服务器。后期根据开发进度开发其他功能。

众所周知：c的http晦涩难学，导包过于繁琐。但性能最好，python的http简单，但是是解释型语言，源码直接暴露，java的http也有学习成本，
且针对简单的http服务使用java这种复杂通用性语言有剩余空间，杀鸡焉用牛刀。

SysyPlus编译器致力于打造一个简单语法的http服务器，上手即用语法简单，不用导任何包，不用管依赖问题，
生成的产物为编译好的可执行文件，文件小，安全，且能利用python不能使用的多核cpu线程优势。

#### 效果图

以用程序自带访问一次HTTP为例

![](/imgs/ide.png)

- IDE GUI可直接进行代码静态分析（变量不存在，函数重复定义等）
- IDE GUI拥有丰富的代码提示功能
- 编译或执行SysyPlus代码
- 当然你也可以当一个文本编辑器

#### 编译选项

- CGUI
  - 开启图形界面编译，生成带GUI的编译器
- DEBUG_FLAG
  - 开启词法、语法的调试模式

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
- web功能
  - 创建socket
  - 连接url
  - 发送GET请求
  - 发送POST请求（正在开发中）
- 代码插桩/优化功能
  - 函数执行时间分析
  - 递归函数转非递归（正在开发中）
- 生成AST语法树
- 生成对应系统架构的目标代码
- ...(比较懒，不想写)

#### 一段SySyPlus源文件

- 无需导包

```c
int main(){
    sleep(1);
    echo('init web framework', '123', '345');
    int socketId = getSocket();
    echo('socket id is',socketId);
    str url = 'file.kingtous.cn';
    str port = '443';
    echo('url is:',url,' port is:',port);
    int state = connectSocket(socketId,url,port);
    state = isSocketConnected(socketId);
    if (state == 0){
        echo('socket connected');
        echo('sending get request');
        str response = getRequest(socketId,url,'/');
        echo('response is:');
        echo(response);
    }
    closeSocket(socketId);
    ret 0;
}
```

- 编译输出
  - 可直接运行
```shell
➜  Desktop g++ 2.sy.o libtime.a libweb.a -o test
➜  Desktop ./test
init web framework 123 345
socket id is 0
url is: file.kingtous.cn  port is: 443
socket connected
sending get request
response is:
<html>
<head><title>302 Found</title></head>
<body>
<center><h1>302 Found</h1></center>
<hr><center>nginx</center>
</body>
</html>

➜  Desktop
```
  - 可选择生成LLVM IR语法
```lldb
 ModuleID = 'cn.kingtous.sysycompiler'
source_filename = "cn.kingtous.sysycompiler"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin19.6.0"

@_str_49 = private unnamed_addr constant [19 x i8] c"init web framework\00", align 1
@_str_50 = private unnamed_addr constant [4 x i8] c"123\00", align 1
@_str_51 = private unnamed_addr constant [4 x i8] c"345\00", align 1
@_str_52 = private unnamed_addr constant [13 x i8] c"socket id is\00", align 1
@_str_53 = private unnamed_addr constant [17 x i8] c"file.kingtous.cn\00", align 1
@_str_54 = private unnamed_addr constant [4 x i8] c"443\00", align 1
@_str_55 = private unnamed_addr constant [8 x i8] c"url is:\00", align 1
@_str_56 = private unnamed_addr constant [10 x i8] c" port is:\00", align 1
@_str_57 = private unnamed_addr constant [17 x i8] c"socket connected\00", align 1
@_str_58 = private unnamed_addr constant [20 x i8] c"sending get request\00", align 1
@_str_59 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@_str_60 = private unnamed_addr constant [13 x i8] c"response is:\00", align 1

define i32 @main() {
jintao_entry:
  %0 = alloca i32, align 4
  store i32 0, i32* %0, align 4
  %1 = tail call i32 @sleep(i32 1)
  %2 = alloca [11 x i8], align 1
  store [11 x i8] c"%s %s %s \0A\00", [11 x i8]* %2, align 1
  %3 = bitcast [11 x i8]* %2 to i8*
  %echo = call i32 (i8*, ...) @printf(i8* %3, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @_str_49, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_str_50, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_str_51, i32 0, i32 0))
  %4 = alloca i32, align 4
  %5 = call i32 @_web_getSocket()
  store i32 %5, i32* %4, align 4
  %6 = alloca [8 x i8], align 1
  store [8 x i8] c"%s %d \0A\00", [8 x i8]* %6, align 1
  %7 = bitcast [8 x i8]* %6 to i8*
  %echo1 = call i32 (i8*, ...) @printf(i8* %7, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @_str_52, i32 0, i32 0), i32 %5)
  %8 = alloca i8*, align 8
  store i8* getelementptr inbounds ([17 x i8], [17 x i8]* @_str_53, i32 0, i32 0), i8** %8, align 8
  %9 = alloca i8*, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_str_54, i32 0, i32 0), i8** %9, align 8
  %10 = alloca [14 x i8], align 1
  store [14 x i8] c"%s %s %s %s \0A\00", [14 x i8]* %10, align 1
  %11 = bitcast [14 x i8]* %10 to i8*
  %echo2 = call i32 (i8*, ...) @printf(i8* %11, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @_str_55, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @_str_53, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @_str_56, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_str_54, i32 0, i32 0))
  %12 = alloca i32, align 4
  %13 = call i32 @_web_connectSocket(i32 %5, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @_str_53, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_str_54, i32 0, i32 0))
  store i32 %13, i32* %12, align 4
  %14 = call i32 @_web_isSocketConnected(i32 %5)
  store i32 %14, i32* %12, align 4
  %equ = icmp eq i32 %14, 0
  br i1 %equ, label %neuq_jintao_if_true, label %neuq_jintao_if_end

neuq_jintao_if_true:                              ; preds = %jintao_entry
  %15 = alloca [5 x i8], align 1
  store [5 x i8] c"%s \0A\00", [5 x i8]* %15, align 1
  %16 = bitcast [5 x i8]* %15 to i8*
  %echo4 = call i32 (i8*, ...) @printf(i8* %16, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @_str_57, i32 0, i32 0))
  %17 = alloca [5 x i8], align 1
  store [5 x i8] c"%s \0A\00", [5 x i8]* %17, align 1
  %18 = bitcast [5 x i8]* %17 to i8*
  %echo5 = call i32 (i8*, ...) @printf(i8* %18, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @_str_58, i32 0, i32 0))
  %19 = alloca i8*, align 8
  %20 = call i8* @_web_callGetRequest(i32 %5, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @_str_53, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @_str_59, i32 0, i32 0))
  store i8* %20, i8** %19, align 8
  %21 = alloca [5 x i8], align 1
  store [5 x i8] c"%s \0A\00", [5 x i8]* %21, align 1
  %22 = bitcast [5 x i8]* %21 to i8*
  %echo6 = call i32 (i8*, ...) @printf(i8* %22, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @_str_60, i32 0, i32 0))
  %23 = alloca [5 x i8], align 1
  store [5 x i8] c"%s \0A\00", [5 x i8]* %23, align 1
  %24 = bitcast [5 x i8]* %23 to i8*
  %echo7 = call i32 (i8*, ...) @printf(i8* %24, i8* %20)
  br label %neuq_jintao_if_end

neuq_jintao_if_end:                               ; preds = %neuq_jintao_if_true, %jintao_entry
  %25 = call i32 @_web_closeSocket(i32 %5)
  store i32 0, i32* %0, align 4
  ret i32 0
}

declare i32 @sleep(i32)

declare i32 @printf(i8*, ...)

declare i32 @_web_getSocket()

declare i32 @_web_connectSocket(i32, i8*, i8*)

declare i32 @_web_isSocketConnected(i32)

declare i8* @_web_callGetRequest(i32, i8*, i8*)

declare i32 @_web_closeSocket(i32)

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
```
  - 可选择生成可读汇编代码
```asm
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15
	.globl	_main
	.p2align	4, 0x90
_main:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$80, %rsp
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	$0, -48(%rbp)
	movl	$169898789, -37(%rbp)
	movb	$0, -33(%rbp)
	leaq	L__str_39(%rip), %rsi
	leaq	-37(%rbp), %rdi
	xorl	%eax, %eax
	callq	_printf
	movl	$1, %edi
	callq	_sleep
	movabsq	$8297073567416218405, %r12
	movq	%r12, -59(%rbp)
	movw	$2592, -51(%rbp)
	movb	$0, -49(%rbp)
	leaq	L__str_40(%rip), %rsi
	leaq	L__str_41(%rip), %rdx
	leaq	L__str_42(%rip), %rcx
	leaq	-59(%rbp), %rdi
	xorl	%eax, %eax
	callq	_printf
	callq	__web_getSocket
	movl	%eax, %r14d
	movl	%eax, -80(%rbp)
	movabsq	$2850364258808613, %rax
	movq	%rax, -88(%rbp)
	leaq	L__str_43(%rip), %rsi
	leaq	-88(%rbp), %rdi
	movl	%r14d, %edx
	xorl	%eax, %eax
	callq	_printf
	leaq	L__str_44(%rip), %r15
	movq	%r15, -104(%rbp)
	leaq	L__str_45(%rip), %rbx
	movq	%rbx, -96(%rbp)
	movq	%r12, -73(%rbp)
	movl	$544417056, -65(%rbp)
	movw	$10, -61(%rbp)
	leaq	L__str_46(%rip), %rsi
	leaq	L__str_47(%rip), %rcx
	leaq	-73(%rbp), %rdi
	movq	%r15, %rdx
	movq	%rbx, %r8
	xorl	%eax, %eax
	callq	_printf
	movl	%r14d, %edi
	movq	%r15, %rsi
	movq	%rbx, %rdx
	callq	__web_connectSocket
	movl	%eax, -44(%rbp)
	movl	%r14d, %edi
	callq	__web_isSocketConnected
	movl	%eax, -44(%rbp)
	testl	%eax, %eax
	jne	LBB0_2
	movq	%rsp, %rax
	leaq	-16(%rax), %rdi
	movq	%rdi, %rsp
	movb	$0, -12(%rax)
	movl	$169898789, -16(%rax)
	leaq	L__str_48(%rip), %rsi
	xorl	%eax, %eax
	callq	_printf
	movq	%rsp, %rax
	leaq	-16(%rax), %rdi
	movq	%rdi, %rsp
	movb	$0, -12(%rax)
	movl	$169898789, -16(%rax)
	leaq	L__str_49(%rip), %rsi
	xorl	%eax, %eax
	callq	_printf
	movq	%rsp, %rbx
	leaq	-16(%rbx), %rsp
	leaq	L__str_44(%rip), %rsi
	leaq	L__str_50(%rip), %rdx
	movl	%r14d, %edi
	callq	__web_callGetRequest
	movq	%rax, %r15
	movq	%rax, -16(%rbx)
	movq	%rsp, %rax
	leaq	-16(%rax), %rdi
	movq	%rdi, %rsp
	movb	$0, -12(%rax)
	movl	$169898789, -16(%rax)
	leaq	L__str_51(%rip), %rsi
	xorl	%eax, %eax
	callq	_printf
	movq	%rsp, %rax
	leaq	-16(%rax), %rdi
	movq	%rdi, %rsp
	movb	$0, -12(%rax)
	movl	$169898789, -16(%rax)
	movq	%r15, %rsi
	xorl	%eax, %eax
	callq	_printf
LBB0_2:
	movl	%r14d, %edi
	callq	__web_closeSocket
	movl	$0, -48(%rbp)
	xorl	%eax, %eax
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L__str_39:
	.asciz	"\347\235\241\347\234\2401s..."

L__str_40:
	.asciz	"init web framework"

L__str_41:
	.asciz	"123"

L__str_42:
	.asciz	"345"

L__str_43:
	.asciz	"socket id is"

L__str_44:
	.asciz	"file.kingtous.cn"

L__str_45:
	.asciz	"443"

L__str_46:
	.asciz	"url is:"

L__str_47:
	.asciz	" port is:"

L__str_48:
	.asciz	"socket connected"

L__str_49:
	.asciz	"sending get request"

L__str_50:
	.asciz	"/"

L__str_51:
	.asciz	"response is:"

.subsections_via_symbols

```

### TODO list

- 支持编译器内的函数指针
- 支持HTTPS TLS加密
- json数据的创建、修改
- 支持连接mysql，执行sql语句返回json