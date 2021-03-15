//
// Created by moony on 3/9/21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/AppArgParser.h"

TEST_CASE("AppArgParser") {

    SUBCASE("명령어 옵션을 -p 8554로 넘겼을 때 port()는 8554이어야 한다.") {
        const char *argv[] = {"dynamicProxying", "-p", "8554"};
        AppArgParser parser{3, argv};
        REQUIRE(parser.port() == 8554);
    }

    SUBCASE("옵션이 없을 경우는 10554로 넘어와야 한다.") {
        const char *argv[] = {"dynamicProxying"};
        AppArgParser parser{1, argv};
        REQUIRE(parser.port() == 10554);
    }
}

#include "../src/redis/IRedisClient.h"
#include <trompeloeil.hpp>
#include "../src/ProxyHostFinder.h"
#include <skyr/url.hpp>
class MockRedisClient : public IRedisClient
{
public:
    MAKE_MOCK2(hget, std::string(const std::string_view&, const std::string_view&), override);
};

TEST_CASE("ProxyHostFinder") {
    MockRedisClient redis;
    skyr::url url{"rtsp://172.25.223.203:8554/192.168.15.107:554/onvif/profile3/media.smp"};
    auto const key = url.pathname().substr(1);

    ALLOW_CALL(redis, hget("proxy:192.168.15.107:554/onvif/profile3/media.smp", "chan")).RETURN("chan-1");
    //ALLOW_CALL(redis, hget(key, "chan")).RETURN("chan-1");

    SUBCASE("hostname for chan-1 is proxy1") {
        ProxyHostFinder finder{ url.pathname(), redis};
        REQUIRE(finder.hostname().compare("proxy1") == 0);
    }

    SUBCASE("port for chan-1 is fixed at 8554") {
        ProxyHostFinder finder{ url.pathname(), redis};
        REQUIRE(finder.port().compare("8554") == 0);
    }
}