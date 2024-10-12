# ezprint

ezprint is what printing should've always looked like.

```c++
struct Point { int x, y; };
std::vector<Point> v{{1, 2}, {3, 4}};
ez::println(v);  // prints {{1 2} {3 4}}
ez::printf("{0} {0}\n", v);  // new in C++20, prints {{1 2} {3 4}} {{1 2} {3 4}}
```

Plug and play, concise, cross-platform.

# Installation
ezprint is header only, just `#include "ezprint.hpp"`. Requires C++17.
Printing with format strings requires C++20.

# Examples

```c++
ez::println(1);  // 1

ez::println(1, "2", "3"s, "4"sv, 5.0);  // 1 2 3 4 5
ez::println(Point{1, 2}, Point{3, 4});  // {1 2} {3 4}
ez::println(std::array{1, 2, 3, 4});    // {1 2 3 4}
ez::println(std::vector{1, 2, 3});      // {1 2 3}

ez::println(std::tuple{Point{1, 2}, "3"s, 4.0});  // {{1 2} 3 4}

ez::println(std::map<std::string, Point>{{"a", {1, 2}}, {"b", {3, 4}}});  // {a: {1 2}  b: {3 4}}

struct Rec { Point a, b; };
ez::println(Rec{{1, 2}, {3, 4}});  // {{1 2} {3 4}}

struct NamedRec { Rec rec; std::string name; };
ez::println(NamedRec{{{1, 2}, {3, 4}}, "5"});  // {{{1 2} {3 4}} 5}

ez::println(
  std::tuple{Rec{1, 2, 3, 4}, "5", 6.0, "7"s, std::pair{std::tuple{8}, std::tuple{Point{10, 11}, "12"sv}}}
);  // {{{1 2} {3 4}} 5 6 7 {{8} {{10 11} 12}}}
```

[It just works](https://wandbox.org/permlink/8foWSB50ylQE3UAy).

## Printing with Format Strings (Requires C++20)
```c++
ez::printf("{:.2f} {}\n", 1., Point{2, 3});  // 1.00 {2 3}
```

## Formatter with `ez::print` Semantics (Requires C++20)
```c++
template<>
struct std::formatter<Point> : ez::formatter<Point> {};

std::print("{}\n", Point{1, 2});  // {1 2}
```

# Usage

ezprint prints [_streamable types_](#Streamable-Types), types that can be streamed through `std::ostream&`, and their composition.

```c++
ez::fprint(os, args...)
ez::fprintln(os, args...)
```
Prints `args` to `os`. The suffix `ln` adds a newline at the end.

```c++
ez::print(args...)
ez::println(args...)
```
Shorthand for `fprint(std::cout, args...)`.

```c++
ez::sprint(args...)
```
Prints to a `std::string` and returns it.

```c++
ez::fprintf(os, fmt, args...)
```
Prints `args` with format string `fmt` to `os`.
If `std::format` can format the string, then `std::format` is called on the argument.
Otherwise, falls back to `ez::sprint` and the format specification applies to the resulting string.
Types that became formattable after certain C++ versions will have printing behaviour depending on C++ standard version (e.g. ranges became formattable in C++23).

```c++
ez::printf(fmt, args...)
```
Shorthand for `fprintf(std::cout, fmt, args...)`.

```c++
ez::sprintf(fmt, args...)
```
Prints `args` with format string `fmt` to a `std::string`.

# Streamable Types

A streamable type is one of the following.

1. A type that can be streamed to `std::ostream&`.
2. A range-like type, ie can be looped over.
3. A tuple-like type, ie has `tuple_size`.
4. An aggregate.

An unstreamable type is printed as an error string. This makes it possible to print types composed of both streamable and unstreamable types.

An exception is array members
```c++
struct A
{
    int i[3];
};
```
`ez::println` will fail to print `A`.

# Remarks

* An aggregate can have at most 64 data members. This is controlled by the implementation by `DEFINE_AS_TUPLE`.

* ezprint is standard compliant and has no undefined behaviour.

* ezprint has no dependencies beyond the standard library.

# Acknowledgements
Inspiration behind the reflection is by [Antony Polukhin](https://www.youtube.com/watch?v=abdeAew3gmQ).
