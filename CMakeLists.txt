cmake_minimum_required(VERSION 3.5)
project(LL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES LL/main.c LL/LinkList.c)
add_executable(LL ${SOURCE_FILES})