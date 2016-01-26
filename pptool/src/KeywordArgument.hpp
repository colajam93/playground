#pragma once

#include "Any.hpp"
#include <string>
#include <unordered_map>
#include <functional>

namespace THI {

class KeywordArgument {
    std::string key_;
    Any value_;

public:
    KeywordArgument(const std::string& key)
        : key_{key}
    {
    }

    template <typename T>
    KeywordArgument& operator=(const T& value)
    {
        value_ = value;
        return *this;
    }

    const auto& value() const { return value_; }
    auto key() const { return key_; }
};

auto operator""_kw(const char* key, std::size_t)
{
    return KeywordArgument{key};
}

struct ArgumentParserBase {
};

void parseArgs(ArgumentParserBase&)
{
}

template <typename Parser, typename T, typename... Args>
void parseArgs(Parser& parser, const T&, const Args&... args)
{
    parseArgs(parser, args...);
}

template <typename Parser, typename... Args>
void parseArgs(Parser& parser, const KeywordArgument& kw, const Args&... args)
{
    auto&& parseFunction = parser.parsers[kw.key()];
    if (parseFunction) {
        parseFunction(kw.value());
    }
    parseArgs(parser, args...);
}

#define ARGUMENT_PARSER                                                        \
    struct ArgumentParser : public THI::ArgumentParserBase {                   \
        std::unordered_map<std::string, std::function<void(const THI::Any&)>>  \
            parsers;                                                           \
    ArgumentParser()

#define ARGUMENT_PARSER_END }

#define ADD_ARGUMENT(name, defaultValue)                                       \
    std::decay_t<decltype(defaultValue)> name = defaultValue

#define ADD_ARGUMENT_CONSTRUCTER(name, defaultValue)                           \
    parsers.emplace(std::make_pair(#name, [this](const THI::Any& value) {      \
        auto result =                                                          \
            THI::anyCast<std::decay_t<decltype(defaultValue)>>(value);         \
        if (result) {                                                          \
            this->name = *result;                                              \
        }                                                                      \
    }))

} // namespace THI
