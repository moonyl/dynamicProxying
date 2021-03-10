//
// Created by moony on 3/9/21.
//

#include "AppArgParser.h"

AppArgParser::AppArgParser(int argc, const char *argv[]) {
    argagg::parser argParser{{
                                     {"help", {"-h", "--help"}, "shows this help message", 0},
                                     {"port", {"-p", "--port"}, "port number (default: 10554)", 1},
                             }};
    _args = argParser.parse(argc, argv);
}

#include <sstream>
unsigned short AppArgParser::port() const {
    using namespace std;
    if (_args["port"])   {
        unsigned short port = _args["port"];
        if (port <= 0 || port > std::numeric_limits<unsigned short>::max())    {
            stringstream sstr;
            sstr << "port can not be the value: " << port;
            throw range_error(sstr.str());
        }
        return port;
    }
    return 10554;
}