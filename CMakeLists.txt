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

# Catch tests
add_executable(catchTests src/catch_tests.cpp)

# gtest tests
add_executable(gtestTests src/gTest_tests.cpp)
target_link_libraries(gtestTests gtest gtest_main)

# gtest tests
add_executable(gmockTests src/gMock_tests.cpp)
target_link_libraries(gmockTests gmock gtest gtest_main)

# boost tests
add_executable(boostTests src/boost_tests.cpp)

# HippoMock tests
add_executable(hippoTests src/hippo_tests.cpp)