#pragma once

#include "Any.hpp"
#include <string>
#include <unordered_map>
#include <functional>
#include "TupleAlgorithm.hpp"
#include <boost/type_index.hpp>
#include <iostream>

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
auto parseArgsImpl(const std::tuple<Values...>& values,
                   const std::tuple<KW...>& keywords)
{
    return std::make_tuple(values, keywords);
}

template <typename... Values, typename... KW, typename... Rest>
auto parseArgsImpl(const std::tuple<Values...>& values,
                   const std::tuple<KW...>& keywords, const KeywordArgument& kw,
                   const Rest&... args)
{
    return parseArgsImpl(
        values, std::tuple_cat(keywords, std::forward_as_tuple(kw)), args...);
}

template <typename... Values, typename... KW, typename T, typename... Rest>
auto parseArgsImpl(const std::tuple<Values...>& values,
                   const std::tuple<KW...>& keywords, const T& t,
                   const Rest&... args)
{
    return parseArgsImpl(std::tuple_cat(values, std::forward_as_tuple(t)),
                         keywords, args...);
}

template <typename... Args>
auto parseArgs(const Args&... args)
{
    return parseArgsImpl(std::make_tuple(), std::make_tuple(), args...);
}

template <typename... Values, typename... KW>
auto update(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
            const KeywordArgument& kw)
{
    std::cout << "update KW" << std::endl;
    auto x = std::make_tuple(v, std::tuple_cat(kws, std::forward_as_tuple(kw)));
    std::cout << "u"
              << boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name()
              << std::endl;

    return x;
}

template <typename... Values, typename... KW, typename T>
auto update(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
            const T& t)
{
    std::cout << "update T" << std::endl;
    return std::make_tuple(std::tuple_cat(v, std::forward_as_tuple(t)), kws);
}

template <std::size_t N = 0, typename... Values, typename... KW, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
auto ap(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
        const std::tuple<Tp...>&)
{
    return std::make_tuple(v, kws);
}

template <std::size_t N = 0, typename... Values, typename... KW, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
auto ap(const std::tuple<Values...>& v, const std::tuple<KW...>& kws,
        const std::tuple<Tp...>& tuple)
{
    auto next = update(v, kws, std::get<N>(tuple));
    auto nv = std::get<0>(next);
    auto nkw = std::get<1>(next);
    std::cout
        << "val "
        << boost::typeindex::type_id_with_cvr<decltype(nv)>().pretty_name()
        << std::endl;
    std::cout
        << "kw "
        << boost::typeindex::type_id_with_cvr<decltype(nkw)>().pretty_name()
        << std::endl;

    // return ap<N + 1, decltype(nv), decltype(nkw), Tp...>(nv, nkw, tuple);
}

} // namespace Detail

template <typename Parser, typename... Args>
auto parseArgs(Parser& parser, const Args&... args)
{
    auto separated = Detail::parseArgs(args...);
    forEach([&parser](auto&& kw) {
        auto&& parseFunction = parser.parsers[kw.key()];
        if (parseFunction) {
            parseFunction(kw.value());
        }
    }, std::get<1>(separated));
    return std::get<0>(separated);
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
