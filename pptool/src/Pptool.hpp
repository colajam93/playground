#pragma once

#include "Any.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>

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
};

auto operator""_kw(const char* key, std::size_t)
{
    return KeywordArgument{key};
}

class KeywordArgumentParser {
    std::unordered_map<std::string, KeywordArgument> keywordArguments_;
};

#define BEGIN_KEYWORD_ARGUMENT                                                 \
    class {                                                                    \
        std::unordered_map<std::string, Any> keywordArguments;                 \
                                                                               \
    public:

#define END_KEYWORD_ARGUMENT                                                   \
    }                                                                          \
    arguments;

#define ADD_KEYWORD_ARGUMENT(key, type)                                        \
    auto key() const                                                           \
    {                                                                          \
        return anyCast<std::decay_t<decltype(type)>>(                          \
            keywordArguments.at(#key));                                        \
    }

BEGIN_KEYWORD_ARGUMENT
ADD_KEYWORD_ARGUMENT(hello, "aaa")
ADD_KEYWORD_ARGUMENT(tom, 10)
ADD_KEYWORD_ARGUMENT(op, 10.0)
END_KEYWORD_ARGUMENT

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

template <typename T, typename... Args>
auto format(const T& t, const Args&... args)
{
    std::stringstream ss;
    ss << std::boolalpha;
    return formatImpl(ss, t, args...).str();
}

template <typename T, typename... Args>
void print(const T& t, const Args&... args)
{
    auto s = format(t, args...);
    std::cout << s << std::endl;
}

} // namespace THI
