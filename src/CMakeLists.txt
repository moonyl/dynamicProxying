project(dynamicProxyingLib)

find_package(Boost REQUIRED)

#include(FetchContent)
#FetchContent_Declare(
#    httpparser
#    GIT_REPOSITORY https://github.com/nekipelov/httpparser.git
#)
#
#FetchContent_GetProperties(httpparser)
#if(NOT httpparser_POPULATED)
#    FetchContent_Populate(httpparser)
#    add_subdirectory(${httpparser_SOURCE_DIR} ${httpparser_BINARY_DIR})
#endif()
#
#FetchContent_MakeAvailable(httpparser)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    find_library(skyrUrl skyr-urld REQUIRED)
else()
    find_library(skyrUrl skyr-url REQUIRED)
endif()

find_library(hiredisLib hiredis REQUIRED)
find_library(redisPlusPlus redis++ REQUIRED)

find_package(spdlog CONFIG REQUIRED)

add_library(dynamicProxyingLib STATIC
        AppArgParser.cpp
        ConnectionListener.cpp
        ConnectionAcceptor.cpp ConnectionAcceptor.h ProxySession.cpp ProxySession.h ProxyHostFinder.cpp ProxyHostFinder.h utils/urls.h redis/RedisClient.cpp redis/RedisClient.h redis/keys.h redis/servers.h redis/servers.cpp redis/IRedisClient.h)

target_include_directories(dynamicProxyingLib
        PUBLIC
            ${CMAKE_SOURCE_DIR}/third-party/includes
)
target_link_libraries(dynamicProxyingLib
        PUBLIC
        ${skyrUrl}
        ${redisPlusPlus}
        ${hiredisLib}
        spdlog::spdlog
        spdlog::spdlog_header_only
        Boost::boost
        pthread
        )