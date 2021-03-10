cmake_minimum_required(VERSION 3.17)

project(testDynamicProxying)

set(CMAKE_CXX_STANDARD 17)

find_package(doctest CONFIG REQUIRED)

add_executable(testDynamicProxying
        testMain.cpp
        testAppArgParser.cpp
        )

target_link_libraries(testDynamicProxying PRIVATE
        dynamicProxyingLib
        doctest::doctest
        )