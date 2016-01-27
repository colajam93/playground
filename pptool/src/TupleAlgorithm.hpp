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

} // namespace THI
