#pragma once

#include <iostream>
#include <sstream>
#include <unordered_map>

namespace Pptool {

class Any {
    class AnyBase {
    public:
        virtual ~AnyBase() = default;
    };

    template <typename T>
    class AnyDerived final : public AnyBase {
        T value_;

    public:
        AnyDerived(const T& value)
            : value_(value)
        {
        }
        virtual ~AnyDerived() = default;
        using type = T;
    };

    AnyBase* p_ = nullptr;

public:
    Any() = default;
    template <typename T>
    Any(const T& value)
        : p_{new AnyDerived<T>(value)}
    {
    }

    template <typename T>
    Any& operator=(const T& value)
    {
        p_ = new AnyDerived<T>(value);
        return *this;
    }
    ~Any() { delete p_; }

    decltype(auto) type() const { return typeid(p_); }
};

class KeyWord {
    std::string key_;
    Any value_;

public:
    KeyWord(const std::string& key)
        : key_{key}
    {
    }

    template <typename T>
    KeyWord& operator=(const T& value)
    {
        value_ = value;
        return *this;
    }
};

auto operator""_kw(const char* key, std::size_t)
{
    return KeyWord{key};
}

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

} // namespace Pptool
