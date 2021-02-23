cmake_minimum_required(VERSION 3.13)
project(sysyplus_compiler)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
# 处理语法分析器
#find_package(BISON)
#BISON_TARGET(Parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.cpp
#        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser/Parser.h)

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

# import gtkmm
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

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
aux_source_directory(ui UI)
message(STATUS "窗口实现：${UI}")

add_executable(sysyplus_compiler
        ${ROOT} ${AST} ${PARSER}
        #        ${BISON_Parser_OUTPUTS}
        ${CodeGen} ${UI} ${ARGS}
        args/all.hpp args/api.hpp args/arg.hpp args/arg_iface.hpp
        args/cmd_line.hpp args/command.hpp args/context.hpp args/enums.hpp
        args/exceptions.hpp args/group_iface.hpp args/groups.hpp args/help_printer.hpp
        args/help_printer.hpp args/help_printer_iface.hpp args/multi_arg.hpp args/types.hpp
        args/utils.hpp args/value_utils.hpp
        )

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs all)

#message(STATUS "Found ${llvm_libs}")

# Link against LLVM libraries
target_link_libraries(sysyplus_compiler LLVM ${GTKMM_LIBRARIES})

#message(STATUS "UI逻辑：复制UI:${CMAKE_SOURCE_DIR}/ui/ui -> ${CMAKE_CURRENT_BINARY_DIR}/ui")
#add_custom_command(TARGET sysyplus_compiler PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/ui/ui ${CMAKE_CURRENT_BINARY_DIR}/ui)