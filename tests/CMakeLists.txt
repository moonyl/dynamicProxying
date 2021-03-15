cmake_minimum_required(VERSION 3.17)

project(testDynamicProxying)

set(CMAKE_CXX_STANDARD 17)

find_package(doctest CONFIG REQUIRED)
find_package(trompeloeil CONFIG REQUIRED)

add_executable(testDynamicProxying
        testMain.cpp
        testUtils.cpp testRedis.cpp)

target_link_libraries(testDynamicProxying PRIVATE
        dynamicProxyingLib
        trompeloeil
        doctest::doctest
        )