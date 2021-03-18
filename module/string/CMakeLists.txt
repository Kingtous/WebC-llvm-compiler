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

add_library(kstring STATIC ${SRC})


install(TARGETS kstring
        ARCHIVE DESTINATION ${LOCATION}
        LIBRARY DESTINATION ${LOCATION}
        )