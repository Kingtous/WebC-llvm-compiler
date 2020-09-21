cmake_minimum_required(VERSION 3.15)
project(llvm_kaleidoscope)

set(CMAKE_CXX_STANDARD 20)

# import LLVM
link_directories(/usr/local/opt/llvm/lib)
include_directories(/usr/local/opt/llvm/include)

add_executable(llvm_kaleidoscope main.cpp Lexer.cpp Lexer.h ExprAST.cpp ExprAST.h parser.cpp parser.h global.h file_reader_wrapper.h)