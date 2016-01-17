#ifndef PAIL_HPP
#define PAIL_HPP

#include <utility>
#include <type_traits>

namespace Pail {

// zero
template <typename T>
T zero(T);

template <typename T>
auto zero(T* t)
{
    return zero(*t);
}

// one
template <typename T>
auto one(T) -> std::add_pointer_t<decltype(zero(std::declval<T>()))>;

// plus
template <typename T>
T plus(T, ...);

template <typename T, typename U>
auto plus(T t, U* u)
{
    return plus(&t, *u);
}

// minus
template <typename T>
T minus(T, ...);

template <typename T, typename U>
auto minus(T* t, U* u)
{
    return minus(*t, *u);
}

// equalTo
template <typename T, typename U>
constexpr auto equalTo =
    !std::is_pointer<decltype(
        minus(std::declval<T>(), std::declval<U>()))>::value &&
    !std::is_pointer<decltype(
        minus(std::declval<U>(), std::declval<T>()))>::value;

// notEqualTo
template <typename T, typename U>
constexpr auto notEqualTo =
    std::is_pointer<decltype(
        minus(std::declval<T>(), std::declval<U>()))>::value ||
    std::is_pointer<decltype(
        minus(std::declval<U>(), std::declval<T>()))>::value;

// greater
template <typename T, typename U>
constexpr auto greater = std::is_pointer<decltype(
    minus(std::declval<T>(), std::declval<U>()))>::value;

// less
template <typename T, typename U>
constexpr auto less = std::is_pointer<decltype(
    minus(std::declval<U>(), std::declval<T>()))>::value;

// greaterEqual
template <typename T, typename U>
constexpr auto greaterEqual = !std::is_pointer<decltype(
    minus(std::declval<U>(), std::declval<T>()))>::value;

// lessEqual
template <typename T, typename U>
constexpr auto lessEqual = !std::is_pointer<decltype(
    minus(std::declval<T>(), std::declval<U>()))>::value;

// isZero
template <typename T>
constexpr auto isZero =
    std::is_same<T, decltype(zero(std::declval<T>()))>::value;

// multiplies
template <typename T>
T multiplies(T, ...);

template <typename T, typename U>
auto multiplies(T* t, U u)
{
    return plus(u, multiplies(*t, u));
}

// divides
template <
    typename T, typename U,
    typename std::enable_if_t<!isZero<U> && !greaterEqual<T, U>>* = nullptr>
auto divides(T t, U)
{
    return zero(t);
}

template <
    typename T, typename U,
    typename std::enable_if_t<!isZero<U> && greaterEqual<T, U>>* = nullptr>
auto divides(T t, U u)
{
    return plus(divides(minus(t, u), u), one(t));
}

// modulus
template <
    typename T, typename U,
    typename std::enable_if_t<!isZero<U> && !greaterEqual<T, U>>* = nullptr>
T modulus(T, U);

template <
    typename T, typename U,
    typename std::enable_if_t<!isZero<U> && greaterEqual<T, U>>* = nullptr>
auto modulus(T t, U u)
{
    return modulus(minus(t, u), u);
}

// gcd
template <typename T, typename U,
          typename std::enable_if_t<isZero<U>>* = nullptr>
T gcd(T, U);

template <typename T, typename U,
          typename std::enable_if_t<!isZero<U>>* = nullptr>
auto gcd(T t, U u)
{
    return gcd(u, modulus(t, u));
}

using IntType = int;
using DefaultType = int;

template <IntType N>
struct StaticInt {
};

template <typename T>
struct PailType {
    constexpr auto toInt() const;
};

// n
template <IntType N, typename T = DefaultType,
          typename std::enable_if_t<N == 0>* = nullptr>
constexpr auto n()
{
    return PailType<T>();
}

template <IntType N, typename T = DefaultType,
          typename std::enable_if_t<(N > 0)>* = nullptr>
constexpr auto n()
{
    return n<N - 1, std::add_pointer_t<T>>();
}

template <typename T, IntType N>
constexpr auto toStaticIntImpl(T, StaticInt<N>)
{
    return StaticInt<N>();
}

template <typename T, IntType N>
constexpr auto toStaticIntImpl(T* t, StaticInt<N>)
{
    return toStaticIntImpl(*t, StaticInt<N + 1>());
}

template <typename T>
constexpr auto toStaticInt(T t)
{
    return toStaticIntImpl(t, StaticInt<0>());
}

template <IntType N>
constexpr auto staticIntToInt(StaticInt<N>)
{
    return N;
}

template <typename T>
constexpr auto PailType<T>::toInt() const
{
    return staticIntToInt(decltype(toStaticInt(std::declval<T>())){});
}

template <typename T>
constexpr auto toInt(PailType<T> t)
{
    return t.toInt();
}

// operators
template <typename T, typename U>
constexpr auto operator+(PailType<T>, PailType<U>)
{
    return PailType<decltype(plus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto operator-(PailType<T>, PailType<U>)
{
    return PailType<decltype(minus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto operator*(PailType<T>, PailType<U>)
{
    return PailType<decltype(
        multiplies(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto operator/(PailType<T>, PailType<U>)
{
    return PailType<decltype(divides(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto operator%(PailType<T>, PailType<U>)
{
    return PailType<decltype(modulus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto operator==(PailType<T>, PailType<U>)
{
    return equalTo<T, U>;
}

template <typename T, typename U>
constexpr auto operator!=(PailType<T>, PailType<U>)
{
    return notEqualTo<T, U>;
}

template <typename T, typename U>
constexpr auto operator>(PailType<T>, PailType<U>)
{
    return greater<T, U>;
}

template <typename T, typename U>
constexpr auto operator<(PailType<T>, PailType<U>)
{
    return less<T, U>;
}

template <typename T, typename U>
constexpr auto operator>=(PailType<T>, PailType<U>)
{
    return greaterEqual<T, U>;
}

template <typename T, typename U>
constexpr auto operator<=(PailType<T>, PailType<U>)
{
    return lessEqual<T, U>;
}

// PailType overload
template <typename T, typename U>
constexpr auto plus(PailType<T>, PailType<U>)
{
    return PailType<decltype(plus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto minus(PailType<T>, PailType<U>)
{
    return PailType<decltype(minus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto multiplies(PailType<T>, PailType<U>)
{
    return PailType<decltype(
        multiplies(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto divides(PailType<T>, PailType<U>)
{
    return PailType<decltype(divides(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto modulus(PailType<T>, PailType<U>)
{
    return PailType<decltype(modulus(std::declval<T>(), std::declval<U>()))>();
}

template <typename T, typename U>
constexpr auto gcd(PailType<T>, PailType<U>)
{
    return PailType<decltype(gcd(std::declval<T>(), std::declval<U>()))>();
}

} // namespace Pail

#endif // PAIL_HPP
