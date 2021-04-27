cmake_minimum_required(VERSION 3.13)
project(KingtousProject)
set(CMAKE_CXX_STANDARD 17)
# 始终选择最高版本依赖
SET(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
SET(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)

message(STATUS ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/unknown)
set(LOCATION "lib")
include_directories()
link_directories()

if (APPLE)
    message(STATUS "macOS detected")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/mac)
    include_directories("/usr/local/Cellar/mysql-connector-c++/include")
    link_directories(/usr/local/Cellar/mysql-connector-c++/lib)
    #    set(CMAKE_C_FLAGS  "-fPIC")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-fPIC")
endif ()

if (UNIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/x86_64/linux)
    message(STATUS "unix detected")
endif ()

aux_source_directory(src SRC)

add_library(ksql STATIC ${SRC})
add_executable(ksqldemo ${SRC})

target_link_libraries(ksqldemo mysqlcppconn kjson)

install(TARGETS ksql
        ARCHIVE DESTINATION ${LOCATION}
        LIBRARY DESTINATION ${LOCATION}
        )