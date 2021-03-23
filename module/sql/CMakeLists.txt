cmake_minimum_required(VERSION 3.13)
project(KingtousProject)
set(CMAKE_CXX_STANDARD 17)

message(STATUS ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/obj/unknown)
set(LOCATION ".")

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

add_library(ksql STATIC ${SRC})

add_executable(ksqldemo ${SRC})
target_link_libraries(ksqldemo mysqlcppconn)

install(TARGETS ksql
        ARCHIVE DESTINATION ${LOCATION}
        LIBRARY DESTINATION ${LOCATION}
        )