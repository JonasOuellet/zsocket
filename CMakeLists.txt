cmake_minimum_required(VERSION 3.5.0)
project(zsocket VERSION 0.1.0 LANGUAGES C CXX)

add_compile_definitions(ZSOCKET_EXPORTS)

set(SOURCE_FILES
src/dllmain.cpp
src/zsocket.cpp
)

set(HEADERS_FILES
include/zsocket.h
)

add_library(zsocket SHARED ${HEADERS_FILES} ${SOURCE_FILES})

target_include_directories(zsocket PRIVATE ${CMAKE_SOURCE_DIR}/include)
