cmake_minimum_required(VERSION 3.8)
project(test1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c linker.h linker.c)
add_executable(test1 ${SOURCE_FILES})