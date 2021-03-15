//
// Created by moony on 3/11/21.
//

#include <doctest/doctest.h>
#include "../src/redis/keys.h"
#include "../src/redis/servers.h"

TEST_CASE("keys") {
    SUBCASE("url path 이름을 이용하여 proxy key값을 만든다.") {
        auto input = "/172.25.223.203:10554/192.168.15.107:554/onvif/profile3/media.smp";
        auto const actual = createProxyKeyFromPathname(input);
        std::string const expected = "proxy:192.168.15.107:554/onvif/profile3/media.smp";
        REQUIRE(actual == expected);
    }
}

TEST_CASE("servers") {
    SUBCASE("get hostname from channel name") {
        auto input = "chan-1";
        auto const actual = hostnameFromChannel(input);
        std::string const expected = "proxy1";
        REQUIRE(actual == expected);
    }

    SUBCASE("get server key from channel name") {
        auto input = "chan-2";
        auto const actual = serverKeyFromChannel(input);
        std::string const expected = "server:2";
        REQUIRE(actual == expected);
    }
}