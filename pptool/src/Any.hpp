#pragma once

#include <memory>
#include <typeinfo>
#include <type_traits>
#include <experimental/optional>

namespace THI {

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

    std::shared_ptr<HolderBase> content_ = nullptr;

public:
    Any() = default;
    template <typename T>
    Any(const T& value)
        : content_{std::make_shared<Holder<std::decay_t<const T>>>(value)}
    {
    }

    template <typename T>
    Any& operator=(const T& value)
    {
        content_ = std::make_shared<Holder<std::decay_t<const T>>>(value);
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

} // namespace THI
