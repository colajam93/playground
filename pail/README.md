Pail
====

Compile time arithmetic operation library using pointer.

## Description

Pail treat the number of pointers as number, for example `int` is `0`, `int*` is `1` and `int**` is `2`.
This provides unsigined integer arithmetic operations, logical operations and simple algorithm using pointer type transforming on compile time.
I test on Linux and clang 3.7.

## Requirement

- clang C++14 mode (not working on gcc currently)

## Feature

- `+`, `-`, `*`, `/`, `%`
- `==`, `!=`, `>`, `<`, `>=`, `<=`
- gcd

## Usage

include "Pail.hpp"

## Demo

The function template `Pail::n<N>()` returns `PailType<int***.....>`.
`PailType<T>` represents the same value of the number of `T`'s pointers.
All functions take the `PailType<T>` and return same template type.

```
#include "Pail.hpp"

int main()
{
    using namespace Pail;

    static_assert(n<10>() == n<10>(), "");
    static_assert(n<5>() < n<10>(), "");

    static_assert(n<10>() + n<10>() == n<20>(), "");
    static_assert(n<28>() / n<7>() == n<4>(), "");
    static_assert(n<43>() % n<6>() == n<1>(), "");

    static_assert(gcd(n<240>(), n<54>()) == n<6>(), "");
    static_assert((n<60>() / n<13>()).toInt() == 4, "");
}
```

## License

MIT

