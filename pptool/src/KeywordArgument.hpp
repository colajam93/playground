#pragma once

#include "Any.hpp"
#include <string>
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
    auto key() const { return key_; }
};

auto operator""_kw(const char* key, std::size_t)
{
    return KeywordArgument{key};
}

class KeywordArgumentParser {
    std::unordered_map<std::string, Any> keywordArguments_;

public:
    template <typename T>
    void add(const std::string& key, const T& value)
    {
        keywordArguments_[key] = value;
    }
    const auto& value(const std::string& key) const
    {
        return keywordArguments_.at(key);
    }
};

#define ADD_ARGUMENT(name, defaultValue)                                       \
    template <typename... Args>                                                \
    auto parseArgs(KeywordArgumentParser& current, const KeywordArgument& kw,  \
                   const Args&... args)                                        \
    {                                                                          \
        if (kw.key() == #name) {                                               \
            current.add(#name, kw.value());                                    \
        }                                                                      \
        else {                                                                 \
            current.add(#name, defaultValue);                                  \
        }                                                                      \
        return parseArgs(current, args...);                                    \
    }                                                                          \
    struct name {                                                              \
    };                                                                         \
    auto getValue(const KeywordArgumentParser& parser, name)                   \
    {                                                                          \
        return anyCast<std::decay_t<decltype(defaultValue)>>(                  \
            parser.value(#name));                                              \
    }

} // namespace THI
