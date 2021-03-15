//
// Created by moony on 3/10/21.
//

#include "ProxyHostFinder.h"
#include <iostream>
#include "redis/RedisClient.h"
#include "redis/keys.h"
#include "redis/servers.h"
using namespace std;

ProxyHostFinder::ProxyHostFinder(const string_view &pathname, IRedisClient& redis) {

    auto key = createProxyKeyFromPathname(pathname);
    auto chan = redis.hget(key, "chan");

    _hostname = hostnameFromChannel(chan);
    auto const serverKey = serverKeyFromChannel(chan);

    _port = "8554";

//      _hostname = "127.0.0.1";
//      _port = "8080";
}

string ProxyHostFinder::hostname() const {
    return _hostname;
}

string ProxyHostFinder::port() const {
    return _port;
}