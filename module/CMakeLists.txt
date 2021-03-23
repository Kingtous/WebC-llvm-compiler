cmake_minimum_required(VERSION 3.13)
project(KingtousCompilerModule)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

add_subdirectory(time)
add_subdirectory(web)
add_subdirectory(string)
add_subdirectory(sql)
add_subdirectory(json)