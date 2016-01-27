#pragma once

#include <cstdio>
#include <sstream>
#include "KeywordArgument.hpp"
#include "TupleAlgorithm.hpp"

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
        ADD_ARGUMENT_CONSTRUCTER(file, stdout);
        ADD_ARGUMENT_CONSTRUCTER(flush, false);
    };
    ADD_ARGUMENT(sep, " ");
    ADD_ARGUMENT(end, "\n");
    ADD_ARGUMENT(file, stdout);
    ADD_ARGUMENT(flush, false);
    ARGUMENT_PARSER_END;

    ArgumentParser parser;
    auto values = THI::parseArgs(parser, args...);

    std::stringstream ss;
    tupleForEach([&ss, &parser, &values](std::size_t index, const auto& v) {
        ss << v;
        if (index != std::tuple_size<decltype(values)>::value - 1) {
            ss << parser.sep;
        }
    }, values);

    std::fprintf(parser.file, "%s%s", ss.str().c_str(), parser.end);
    if (parser.flush) {
        std::fflush(parser.file);
    }
}

} // namespace THI
