//
// Created by moony on 3/9/21.
//

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