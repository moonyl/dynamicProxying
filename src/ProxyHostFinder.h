//
// Created by moony on 3/10/21.
//

#pragma once

#include <skyr/url.hpp>
class IRedisClient;

class ProxyHostFinder
{
    std::string _hostname;
    std::string _port;

public:
    explicit ProxyHostFinder(const std::string_view &pathname, IRedisClient& redis);

    std::string hostname() const;
    std::string port() const;
};


