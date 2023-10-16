cmake_minimum_required(VERSION 3.10)

project(From-regular-to-NFA)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_executable(
        MatchRegularTest
        tests/MatchRegularTest.cpp
        MatchRegular.cpp
        ParseRegular.cpp
        functions.h
        NFA.h
)

target_link_libraries(
        MatchRegularTest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MatchRegularTest)
