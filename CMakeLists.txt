cmake_minimum_required(VERSION 3.3)

project(batchren)

set(CMAKE_CXX_STANDARD 20)
if (CMAKE_HOST_UNIX)
    add_executable(batchren batchren.cpp)
elseif (CMAKE_HOST_APPLE)
    message(FATAL_ERROR "batchren doesn't supports apple, currently")
else ()
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    add_executable(batchren batchren.cpp)
endif ()