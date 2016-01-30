#pragma once

#include <tuple>
#include <type_traits>

namespace THI {

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
void forEach(const Func&, const std::tuple<Tp...>&)
{
}

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
void forEach(const Func& func, const std::tuple<Tp...>& tuple)
{
    func(std::get<N>(tuple));
    forEach<N + 1>(func, tuple);
}

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
void forEachWithIndex(const Func&, const std::tuple<Tp...>&)
{
}

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
void forEachWithIndex(const Func& func, const std::tuple<Tp...>& tuple)
{
    func(N, std::get<N>(tuple));
    forEachWithIndex<N + 1>(func, tuple);
}

template <std::size_t N = 0, typename Func, typename Identity, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
auto foldL(const Func&, const Identity& identity, const std::tuple<Tp...>&)
{
    return identity;
}

template <std::size_t N = 0, typename Func, typename Identity, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
auto foldL(const Func& func, const Identity& identity,
           const std::tuple<Tp...>& tuple)
{
    return foldL<N + 1>(func, func(identity, std::get<N>(tuple)), tuple);
}

template <std::size_t N = 0, typename Func, typename Identity, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
auto foldR(const Func&, const Identity& identity, const std::tuple<Tp...>&)
{
    return identity;
}

template <std::size_t N = 0, typename Func, typename Identity, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
auto foldR(const Func& func, const Identity& identity,
           const std::tuple<Tp...>& tuple)
{
    return func(std::get<N>(tuple), foldR<N + 1>(func, identity, tuple));
}

template <std::size_t N = 0, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
auto reverse(const std::tuple<Tp...>&)
{
    return std::make_tuple();
}

template <std::size_t N = 0, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
auto reverse(const std::tuple<Tp...>& tuple)
{
    return std::tuple_cat(reverse<N + 1>(tuple),
                          std::make_tuple(std::get<N>(tuple)));
}

} // namespace THI
