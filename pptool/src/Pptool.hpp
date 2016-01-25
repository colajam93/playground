#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include <experimental/optional>

namespace Pptool {

class Any {
    class HolderBase {
    public:
        virtual ~HolderBase() = default;
        virtual const std::type_info& type() const = 0;
    };

    template <typename T>
    class Holder final : public HolderBase {
        T value_;

    public:
        Holder(const T& value)
            : value_{value}
        {
        }
        ~Holder() = default;
        const std::type_info& type() const override { return typeid(T); }
        const T& value() const { return value_; }
    };

    std::unique_ptr<HolderBase> content_ = nullptr;

public:
    Any() = default;
    template <typename T>
    Any(const T& value)
        : content_{std::make_unique<Holder<std::decay_t<const T>>>(value)}
    {
    }

    template <typename T>
    Any& operator=(const T& value)
    {
        content_ = std::make_unique<Holder<std::decay_t<const T>>>(value);
        return *this;
    }

    decltype(auto) type() const
    {
        return content_ ? content_->type() : typeid(void);
    }

    template <typename T>
    auto value() const
    {
        auto p = dynamic_cast<Holder<T>*>(content_.get());
        return p ? std::experimental::optional<T>{p->value()}
                 : std::experimental::nullopt;
    }
};

template <typename T>
auto anyCast(const Any& any)
{
    return any.value<T>();
}

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
};

auto operator""_kw(const char* key, std::size_t)
{
    return KeywordArgument{key};
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
