//
// Created by moony on 3/11/21.
//

#include "servers.h"
#include <sstream>

static std::string_view serverIndex(const std::string_view &chan)
{
    return chan.substr(5);
}
std::string hostnameFromChannel(const std::string_view &chan) {
    std::stringstream sstr;
    sstr << "proxy" << serverIndex(chan);
    return sstr.str();
}

std::string serverKeyFromChannel(const std::string_view &chan) {
    std::stringstream sstr;
    sstr << "server:" << serverIndex(chan);
    return sstr.str();
}
