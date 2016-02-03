#pragma once
#include <type_traits>
#include <tuple>

namespace THI {

namespace Detail {

template <template <typename> class Pred, typename List>
struct allOf;

template <template <typename> class Pred, typename Head, typename... Tail>
struct allOf<Pred, std::tuple<Head, Tail...>> {
    static const bool value =
        Pred<Head>::value ? allOf<Pred, std::tuple<Tail...>>::value : false;
};

template <template <typename> class Pred>
struct allOf<Pred, std::tuple<>> {
    static const bool value = true;
};

template <template <typename> class Pred, typename List>
struct anyOf;

template <template <typename> class Pred, typename Head, typename... Tail>
struct anyOf<Pred, std::tuple<Head, Tail...>> {
    static const bool value =
        Pred<Head>::value ? true : anyOf<Pred, std::tuple<Tail...>>::value;
};

template <template <typename> class Pred>
struct anyOf<Pred, std::tuple<>> {
    static const bool value = false;
};

} // namespace Detail

template <template <typename> class Pred, typename... Tp>
struct allOf {
    static const bool value = Detail::allOf<Pred, std::tuple<Tp...>>::value;
};

template <template <typename> class Pred, typename... Tp>
constexpr bool allOf_v = allOf<Pred, Tp...>::value;

template <template <typename> class Pred, typename... Tp>
struct anyOf {
    static const bool value = Detail::anyOf<Pred, std::tuple<Tp...>>::value;
};

template <template <typename> class Pred, typename... Tp>
constexpr bool anyOf_v = anyOf<Pred, Tp...>::value;

} // namespace THI
