//
// Created by moony on 3/11/21.
//

#include <doctest/doctest.h>
#include "../src/utils/urls.h"

TEST_CASE("urls") {

    SUBCASE("http 헤더 형태의 값일 때 url을 추출해낼 수 있어야 한다.") {
        auto input = "GET http://goddaehee.tistory.com/168 HTTP/1.1\n"
                     "\n"
                     "Host: goddaehee.tistory.com\n"
                     "Connection: keep-alive\n"
                     "Upgrade-Insecure-Requests: 1\n"
                     "\n"
                     "\n";

        auto actual = createUrlFromConnectReq(input);
        auto expected = skyr::url{"http://goddaehee.tistory.com/168"};

        REQUIRE(std::string(actual.c_str()).compare(expected.c_str()) == 0);
    }

    SUBCASE("http 헤더 형태의 값일 때 경로값을 추출한다.") {
        auto input = "GET /192.168.15.107:554/onvif/profile3/media.smp HTTP/1.0\r\n"
                     "\r\n"
                     "CSeq: 1\r\n"
                     "\r\n"
                     "User-Agent: LIVE555 Streaming Media v2020.12.23\r\n"
                     "\r\n"
                     "Host: 172.29.131.87\r\n"
                     "\r\n"
                     "x-sessioncookie: 6fe99d2d576445b7980c463\r\n"
                     "\r\n"
                     "Accept: application/x-rtsp-tunnelled\r\n"
                     "\r\n"
                     "Pragma: no-cache\r\n"
                     "\r\n"
                     "Cache-Control: no-cache\r\n"
                     "\r\n"
                     "\r\n";

        auto actual = pathnameFromHttpRequest(input);
        auto expected = "/192.168.15.107:554/onvif/profile3/media.smp";

        REQUIRE(actual.compare(expected) == 0);
    }

    SUBCASE("rtsp 헤더 형태의 값일 때 경로값을 추출한다.") {
        auto input = "OPTIONS rtsp://172.29.131.87:12554/192.168.15.107:554/onvif/profile2/media.smp RTSP/1.0\r\n"
                     "\r\n"
                     "CSeq: 1\r\n"
                     "\r\n"
                     "User-Agent: Lavf58.45.100\r\n"
                     "\r\n"
                     "\r\n";

        auto actual = pathnameFromRtspRequest(input);
        auto expected = "/192.168.15.107:554/onvif/profile2/media.smp";

        REQUIRE(actual.compare(expected) == 0);
    }

}