PPTool
======

Python like print function and Keyword Argument.

## Idea From

[C++11 named parameters using operator suffixes and variadic templates](http://justinvh.blogspot.jp/2012/10/c11-named-parameters-using-operator.html)

## Requirement

- clang C++14 mode

## Feature

- format
- print
- keyword argument

## Usage

### format and print

```
#include "FormatPrint.hpp"

void f()
{
    using THI::operator"" _kw;
    THI::print(10, 1.0, false, "hello", "end"_kw ="\nhello\n", "sep"_kw = ", ");
    auto s = THI::format(10, 1.0, false, "hello", "sep"_kw = "|");
    std::cout << s << std::endl;
    // 10, 1, false, hello
    // hello
    // 10|1|false|hello
}
```

### Keyword Argument

#### include header

```
#include "KeywordArgument.hpp"
```

#### define variadic function

```
template <typename... Args>
auto format(const Args&... args)
```

#### register keyword arguments

```
ARGUMENT_PARSER
{
    ADD_ARGUMENT_CONSTRUCTOR(sep, " ");
    ADD_ARGUMENT_CONSTRUCTOR(end, "");
    ADD_ARGUMENT_CONSTRUCTOR(boolalpha, true);
};
ADD_ARGUMENT(sep, " ");
ADD_ARGUMENT(end, "");
ADD_ARGUMENT(boolalpha, true);
ARGUMENT_PARSER_END;
```

The first argument of `ADD_ARGUMENT_CONSTRUCTOR` is name of key. The second is default value (Type is deduced from the value).
`ADD_ARGUMENT_CONSTRUCTOR` should be placed in the `ARGUMENT_PARSER` block.

```
ARGUMENT_PARSER
{
    // here
};
```

Then, write `ADD_ARGUMENT` macro with same arguments. Finally write `ARGUMENT_PARSER_END` macro.

#### parse arguments

```
ArgumentParser parser;
auto parsed = parseArgs(parser, args...);
auto values = std::get<0>(parsed);
auto kws = std::get<1>(parsed);
```

Create `ArgumentParser` object. Then pass `ArgumentParser` object and arguments to `parseArgs` function.
That returns `std::tuple<std::tuple<Values...>, std::tuple<KeywordArgument...>>` and assign values to `ArgumentParser` object.
You can access values the following.

```
std::cout << parser.value << std::endl;
```

## Problems

- Compile time becomes very long
- It is impossible to parse the `KeywordArgument` and `std::tuple<KeywordArgument...>` together.
    - workaround `std::tuple_cat(std::tuple<KW...>, std::make_tuple("foo"_kw = bar))`

## License

MIT

