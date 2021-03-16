cmake_minimum_required(VERSION 3.13)
project(sysyplus_compiler)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#find_package(LLD REQUIRED CONFIG)
#message(STATUS "find lld ${LLD_INCLUDE_DIRS}")
#include_directories(${LLD_INCLUDE_DIRS})
#add_definitions(${LLD_DEFINITIONS})

message(STATUS "查找Boost框架")
find_package(Boost COMPONENTS system filesystem thread chrono REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})
#add_definitions(${GTK3_CFLAGS_OTHER})
# 处理语法分析器
#find_package(BISON)
#BISON_TARGET(Parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.cpp
#        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.h)

# C++静态链接
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
#message(STATUS "GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
#message(STATUS "GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using defs in: ${LLVM_DEFINITIONS}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

# import LLVM
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# import gtk
#include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})


# import gtkmm, for linux use 3.0, for macos use 4.0 for test
FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 gtksourceviewmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
message(STATUS "GTKMM Lib ${GTKMM_LIBRARY_DIRS}")
message(STATUS "GTKMM Inc ${GTKMM_INCLUDE_DIRS}")
aux_source_directory(ui UI)
message(STATUS "窗口实现：${UI}")

# 导入头文件
include_directories(
        .
        parser
        codegen
        args
)

aux_source_directory(. ROOT)
message(STATUS "根目录含有：${ROOT}")
aux_source_directory(parser PARSER)
message(STATUS "解释器文件含有：${PARSER}")
aux_source_directory(ast AST)
message(STATUS "语法树定义：${AST}")
aux_source_directory(codegen CodeGen)
message(STATUS "代码生成：${CodeGen}")
aux_source_directory(args ARGS)
message(STATUS "Console Parser：${ARGS}")
aux_source_directory(pass PASS)
message(STATUS "外部声明：${EXT}")
aux_source_directory(extern EXT)
message(STATUS "Pass：${PASS}")
message(STATUS "编译器：${COMPILER}")
aux_source_directory(compiler COMPILER)
message(STATUS "Pass：${COMPILER}")

add_executable(sysyplus_compiler
        ${ROOT} ${AST} ${PARSER}
        #        ${BISON_Parser_OUTPUTS}
        ${CodeGen} ${UI} ${ARGS} ${PASS} ${EXT} ${COMPILER}
        args/all.hpp args/api.hpp args/arg.hpp args/arg_iface.hpp
        args/cmd_line.hpp args/command.hpp args/context.hpp args/enums.hpp
        args/exceptions.hpp args/group_iface.hpp args/groups.hpp args/help_printer.hpp
        args/help_printer.hpp args/help_printer_iface.hpp args/multi_arg.hpp args/types.hpp
        args/utils.hpp args/value_utils.hpp
        )

# CGUI存在则生成可视化界面 DEBUG_FLAG开启调试模式
add_compile_definitions(sysyplus_compiler PUBLIC CGUI=1 DEBUG_FLAG=1)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader
        x86info x86asmparser x86desc x86codegen x86disassembler #X86
        )

message(STATUS "Found ${llvm_libs}")

# Link against LLVM libraries
#target_link_libraries(sysyplus_compiler ${llvm_libs})
target_link_libraries(sysyplus_compiler ${llvm_libs} ${GTKMM_LIBRARIES} Boost::thread Boost::chrono)

#message(STATUS "UI逻辑：复制UI:${CMAKE_SOURCE_DIR}/ui/ui -> ${CMAKE_CURRENT_BINARY_DIR}/ui")
#add_custom_command(TARGET sysyplus_compiler PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/ui/ui ${CMAKE_CURRENT_BINARY_DIR}/ui)

#message(STATUS "配置目标库")
#add_subdirectory(module)

# time module
#aux_source_directory(module/time/src TIME_MODULE)
#message(STATUS "${TIME_MODULE}")
#add_library(time "${TIME_MODULE}")
add_subdirectory(module)

# Web
#aux_source_directory(module/web/src WEB_MODULE)
#message(STATUS "${WEB_MODULE}")
#add_library(web "${WEB_MODULE}")

# test.cpp
#add_executable(test test/test1.c)