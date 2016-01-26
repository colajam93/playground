#pragma once

#include <cstdio>
#include <sstream>
#include "KeywordArgument.hpp"

namespace THI {

decltype(auto) formatImpl(std::stringstream& ss)
{
    return ss;
}

template <typename T, typename... Args>
decltype(auto) formatImpl(std::stringstream& ss, const T& t,
                          const Args&... args)
{
    ss << t;
    if (sizeof...(args) != 0) {
        ss << ' ';
    }
    return formatImpl(ss, args...);
}

template <typename... Args>
auto format(const Args&... args)
{
    std::stringstream ss;
    ss << std::boolalpha;
    return formatImpl(ss, args...).str();
}

template <typename... Args>
void print(const Args&... args)
{
    ARGUMENT_PARSER
    {
        ADD_ARGUMENT_CONSTRUCTER(sep, " ");
        ADD_ARGUMENT_CONSTRUCTER(end, "\n");
    };
    ADD_ARGUMENT(sep, " ");
    ADD_ARGUMENT(end, "\n");
    ARGUMENT_PARSER_END;

    ArgumentParser parser;
    auto values = THI::parseArgs(parser, args...);

    // TODO
    // change format use std::tuple
    // auto s = format(values);
    // std::fprintf(stdout, "%s%s", s.c_str(), parser.end);
}

} // namespace THI
