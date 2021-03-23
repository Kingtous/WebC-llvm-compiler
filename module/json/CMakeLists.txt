cmake_minimum_required(VERSION 3.13)
project(KingtousProject)
set(CMAKE_CXX_STANDARD 17)

message(STATUS ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/unknown)
set(LOCATION ".")

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(JSONLib jsoncpp)
#message(STATUS "json decl:${JSONLib_INCLUDE_DIRS}")
#message(STATUS "json lib:${JSONLib_LIBRARY_DIRS}")
#include_directories(${JSONLib_INCLUDE_DIRS})
#link_directories(${JSONLib_LIBRARY_DIRS})

message(STATUS "查找Boost框架 For PropertyTree")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


if (APPLE)
    message(STATUS "macOS detected")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/mac)
    #    set(CMAKE_C_FLAGS  "-fPIC")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-fPIC")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/linux)
    message(STATUS "unix detected")
endif ()

aux_source_directory(src SRC)

add_library(kjson STATIC ${SRC})

add_executable(kjsondemo ${SRC})

install(TARGETS kjson
        ARCHIVE DESTINATION ${LOCATION}
        LIBRARY DESTINATION ${LOCATION}
        )