cmake_minimum_required(VERSION 3.2)
project(Alignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(Alignment ${SOURCE_FILES})