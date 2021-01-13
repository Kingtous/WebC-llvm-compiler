cmake_minimum_required(VERSION 3.13)
project(llvm_kaleidoscope)

set(CMAKE_CXX_STANDARD 11)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using defs in: ${LLVM_DEFINITIONS}")

# import LLVM
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(llvm_kaleidoscope main.cpp Lexer.cpp Lexer.h ExprAST.cpp ExprAST.h Parser.cpp Parser.h Global.h IFileReader.h ErrHelper.h ErrHelper.cpp Global.cpp FileReader.cpp FileReader.h)

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs all)

#message(STATUS "Found ${llvm_libs}")

# Link against LLVM libraries
target_link_libraries(llvm_kaleidoscope LLVM)