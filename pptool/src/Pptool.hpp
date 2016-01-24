#pragma once

#include <iostream>
#include <sstream>

namespace Pptool {

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
