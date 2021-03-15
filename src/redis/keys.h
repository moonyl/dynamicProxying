//
// Created by moony on 3/11/21.
//

#pragma once

#include <skyr/url.hpp>
#include <sstream>

inline std::string createProxyKeyFromPathname(const std::string_view &pathname)
{
    std::stringstream sstr;
    auto proxying = pathname.substr(1);
    sstr << "proxy:" << proxying;
    auto key = sstr.str();
    return key;
}