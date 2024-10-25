cmake_minimum_required(VERSION 3.29)
project(damnCoffee)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_SOURCE_ENCODING GB2312)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
add_executable(damnCoffee main.cpp resources.rc)