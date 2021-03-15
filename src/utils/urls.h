//
// Created by moony on 3/11/21.
//

#pragma once

#include <skyr/url.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <httpparser/request.h>
#include <httpparser/httprequestparser.h>

inline std::string pathnameFromHttpRequest(const std::string_view& line)
{
    using namespace httpparser;
    Request request;
    HttpRequestParser parser;

    HttpRequestParser::ParseResult res = parser.parse(request, line.cbegin(), line.cend());
    if( res == HttpRequestParser::ParsingCompleted )
    {
        std::cout << request.uri << std::endl;

        return request.uri;
    }
    return "";
}

inline skyr::url createUrlFromConnectReq(const std::string_view& line)  {

    std::string_view const protocols[] = {"rtsp", "http"};
    for (auto const& protocol : protocols)  {
        std::stringstream sstrScheme;
        sstrScheme << protocol << "://";
        auto pos = line.find(sstrScheme.str());
        if (pos == std::string_view::npos)   {
            continue;
        }

        std::string temp;
        std::transform(std::cbegin(protocol), std::cend(protocol), std::back_inserter(temp), ::toupper);
        std::stringstream sstr;
        sstr << " " << temp << "/";
//        std::cout << sstr.str() << std::endl;
        auto end = line.find(sstr.str());
        if (end == std::string_view::npos)   {
            continue;
        }
        auto urlStr = line.substr(pos, end - pos);
        return skyr::url{urlStr};
    }
    return skyr::url{};
}

inline std::string pathnameFromRtspRequest(const std::string_view& line)    {
    auto url = createUrlFromConnectReq(line);
    return url.pathname();
}