cmake_minimum_required(VERSION 3.17)
project(dynamicProxying)

set(CMAKE_CXX_STANDARD 17)




add_executable(dynamicProxying main.cpp)

target_link_libraries(dynamicProxying
        PRIVATE
            dynamicProxyingLib)

add_subdirectory(src)
add_subdirectory(tests)

install(TARGETS dynamicProxying
        DESTINATION ${CMAKE_SOURCE_DIR}/docker)
install(CODE "execute_process(COMMAND bash -c ${CMAKE_SOURCE_DIR}/scripts/build_docker.sh)")