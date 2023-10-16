cmake_minimum_required(VERSION 3.26)

project(From-regular-to-NFA)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GoogleTest)

enable_testing()

add_executable(
        tests
        tests/run_tests.cpp
        tests/MatchRegularTest.cpp
        tests/ParseRegularTest.cpp
        Functions/MatchRegular.cpp
        Functions/ParseRegular.cpp
        NFA/NFA.cpp
)

target_link_libraries(
        tests
        GTest::gtest_main
)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

gtest_discover_tests(tests)
