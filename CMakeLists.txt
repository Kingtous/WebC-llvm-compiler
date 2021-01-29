cmake_minimum_required(VERSION 3.13)
project(llvm_kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
# 处理语法分析器
#find_package(BISON)
#BISON_TARGET(Parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.cpp
#        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.h)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using defs in: ${LLVM_DEFINITIONS}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

# import LLVM
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 导入头文件
include_directories(
        .
        parser
)

aux_source_directory(. ROOT)
message(STATUS "根目录含有：${ROOT}")
aux_source_directory(parser PARSER)
message(STATUS "解释器文件含有：${PARSER}")
aux_source_directory(ast AST)
message(STATUS "语法树定义：${AST}")

add_executable(llvm_kaleidoscope
        ${ROOT} ${AST} ${PARSER}
#        ${BISON_Parser_OUTPUTS}
        )

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs all)

#message(STATUS "Found ${llvm_libs}")

# Link against LLVM libraries
target_link_libraries(llvm_kaleidoscope LLVM)