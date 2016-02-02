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

} // namespace Detail

template <template <typename> class Pred, typename... Tp>
struct allOf {
    static const bool value = Detail::allOf<Pred, std::tuple<Tp...>>::value;
};

template <template <typename> class Pred, typename... Tp>
constexpr bool allOf_v = allOf<Pred, Tp...>::value;

} // namespace THI
