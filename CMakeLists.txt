cmake_minimum_required(VERSION 3.17)
project(dynamicProxying)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(dynamicProxying main.cpp)

target_link_libraries(dynamicProxying PRIVATE Boost::boost pthread)

add_subdirectory(tests)