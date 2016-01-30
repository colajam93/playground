#pragma once

#include "Any.hpp"
#include <string>
#include <unordered_map>
#include <functional>
#include "TupleAlgorithm.hpp"

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

namespace Detail {

template <typename... Values, typename... KW>
auto updateTuple(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
                 const KeywordArgument& kw)
{
    return std::make_tuple(v, std::tuple_cat(kws, std::make_tuple(kw)));
}

template <typename... Values, typename... KW, typename T>
auto updateTuple(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
                 const T& t)
{
    return std::make_tuple(std::tuple_cat(v, std::make_tuple(t)), kws);
}

template <typename... Args>
auto separateArgs(const std::tuple<Args...>& args)
{
    auto separated = foldR([](const auto& value, const auto& tuple) {
        return updateTuple(std::get<0>(tuple), std::get<1>(tuple), value);
    }, std::make_tuple(std::make_tuple(), std::make_tuple()), args);

    return std::make_tuple(reverse(std::get<0>(separated)),
                           std::get<1>(separated));
}

template <typename... Args>
auto separateArgs(const Args&... args)
{
    return separateArgs(std::make_tuple(args...));
}

} // namespace Detail

template <typename Parser, typename... Args>
auto parseArgs(Parser& parser, const Args&... args)
{
    auto separated = Detail::separateArgs(args...);
    forEach([&parser](auto&& kw) {
        auto&& parseFunction = parser.parsers[kw.key()];
        if (parseFunction) {
            parseFunction(kw.value());
        }
    }, std::get<1>(separated));
    return separated;
}

#define ARGUMENT_PARSER                                                        \
    struct ArgumentParser {                                                    \
        std::unordered_map<std::string, std::function<void(const THI::Any&)>>  \
            parsers;                                                           \
    ArgumentParser()

#define ARGUMENT_PARSER_END }

#define ADD_ARGUMENT(name, defaultValue)                                       \
    std::decay_t<decltype(defaultValue)> name = defaultValue

#define ADD_ARGUMENT_CONSTRUCTOR(name, defaultValue)                           \
    parsers.emplace(std::make_pair(#name, [this](const THI::Any& value) {      \
        auto result =                                                          \
            THI::anyCast<std::decay_t<decltype(defaultValue)>>(value);         \
        if (result) {                                                          \
            this->name = *result;                                              \
        }                                                                      \
    }))

} // namespace THI
