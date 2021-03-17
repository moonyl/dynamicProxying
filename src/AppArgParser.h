//
// Created by moony on 3/9/21.
//

#pragma once

#include <argagg/argagg.hpp>

class AppArgParser
{
    argagg::parser _argParser;
    argagg::parser_results _args;
public:
    AppArgParser(int argc, const char *argv[]);

    unsigned short port() const;
    bool helpRequested() const;
    std::string helpMessage() const;
};


