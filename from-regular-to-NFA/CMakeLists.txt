cmake_minimum_required(VERSION 3.10)

project(From-regular-to-NFA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(solution main.cpp functions.h NFA.h parse-regular.cpp)
