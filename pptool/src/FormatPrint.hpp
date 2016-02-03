#pragma once

#include <cstdio>
#include <sstream>
#include "KeywordArgument.hpp"
#include "TupleAlgorithm.hpp"

namespace THI {

template <typename... Args>
auto format(const Args&... args)
{
    ARGUMENT_PARSER
    {
        ADD_ARGUMENT_CONSTRUCTOR(sep, " ");
        ADD_ARGUMENT_CONSTRUCTOR(end, "");
        ADD_ARGUMENT_CONSTRUCTOR(boolalpha, true);
    };
    ADD_ARGUMENT(sep, " ");
    ADD_ARGUMENT(end, "");
    ADD_ARGUMENT(boolalpha, true);
    ARGUMENT_PARSER_END;

    ArgumentParser parser;
    auto parsed = parseArgs(parser, args...);
    auto values = std::get<0>(parsed);

    std::stringstream ss;
    if (parser.boolalpha) {
        ss << std::boolalpha;
    }
    forEachWithIndex([&ss, &parser, &values](std::size_t index, const auto& v) {
        ss << v;
        if (index != std::tuple_size<decltype(values)>::value - 1) {
            ss << parser.sep;
        }
    }, values);
    ss << parser.end;

    return ss.str();
}

template <typename... Args>
void print(const Args&... args)
{
    ARGUMENT_PARSER
    {
        ADD_ARGUMENT_CONSTRUCTOR(end, "\n");
        ADD_ARGUMENT_CONSTRUCTOR(file, stdout);
        ADD_ARGUMENT_CONSTRUCTOR(flush, false);
    };
    ADD_ARGUMENT(end, "\n");
    ADD_ARGUMENT(file, stdout);
    ADD_ARGUMENT(flush, false);
    ARGUMENT_PARSER_END;

    ArgumentParser parser;
    auto parsed = parseArgs(parser, args...);
    auto kw = std::tuple_cat(std::get<1>(parsed),
                             std::make_tuple("end"_kw = parser.end));
    auto str = format(std::get<0>(parsed), kw);
    std::fprintf(parser.file, "%s", str.c_str());
    if (parser.flush) {
        std::fflush(parser.file);
    }
}

} // namespace THI
