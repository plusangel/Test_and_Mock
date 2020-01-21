cmake_minimum_required(VERSION 3.15)
project(autoBrake)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        include
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR})

add_subdirectory(lib)
enable_testing()

add_library(autoBrake include/autoBrake.h)
add_executable(main main.cpp)

# Catch2 tests
add_executable(catch2Tests catch2_tests.cpp)

# gtest tests
add_executable(gtestTests gTest_tests.cpp)
target_link_libraries(gtestTests gtest gtest_main)

# boost tests
add_executable(boostTests boost_tests.cpp)

# gmock tests
add_executable(hippoTests hippo_tests.cpp)