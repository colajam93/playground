#pragma once

#include <tuple>
#include <type_traits>

namespace THI {

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
void tupleForEach(const Func&, const std::tuple<Tp...>&)
{
}

template <std::size_t N = 0, typename Func, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
void tupleForEach(const Func& func, const std::tuple<Tp...>& tuple)
{
    func(std::get<N>(tuple));
    tupleForEach<N + 1, Func, Tp...>(func, tuple);
}

template <std::size_t N = 0, typename Func, typename MergeFunc,
          typename Identity, typename... Tp,
          typename std::enable_if_t<N == sizeof...(Tp)>* = nullptr>
decltype(auto) tupleForEach(const Func&, const MergeFunc&,
                            const Identity& identity, const std::tuple<Tp...>&)
{
    return identity;
}

template <std::size_t N = 0, typename Func, typename MergeFunc,
          typename Identity, typename... Tp,
          typename std::enable_if_t<(N < sizeof...(Tp))>* = nullptr>
decltype(auto) tupleForEach(const Func& func, const MergeFunc& mergeFunc,
                            const Identity& identity,
                            const std::tuple<Tp...>& tuple)
{
    auto&& result = func(std::get<N>(tuple));
    return mergeFunc(result,
                     tupleForEach<N + 1, Func, MergeFunc, Identity, Tp...>(
                         func, mergeFunc, identity, tuple));
}

} // namespace THI
