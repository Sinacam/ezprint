# ezprint

ezprint is what printing should've always looked like.

````c++
struct Point { int x, y; };
std::map<std::string, Point> vec = {{"a", {1, 2}}, {"b", {3, 4}}};
ez::println(vec);
// prints {a: {1 2}  b: {4 5}}
````

Plug and play, arbitrary composition, concise, cross-platform.

### Easy.

# Installation
Just `#include "ezprint.hpp"`. Requires C++17.

# Examples

````c++
ez::println(1);  // 1

ez::println(1, "2", "3"s, "4"sv, 5.0);  // 1 2 3 4 5
ez::println(Point{1, 2}, Point{3, 4});  // {1 2} {3 4}
ez::println(std::array{1, 2, 3, 4});    // {1 2 3 4}
ez::println(std::vector{1, 2, 3});      // {1 2 3}

ez::println(std::tuple{Point{1, 2}, "3"s, 4.0});  // {{1 2} 3 4}

struct Rec { Point a, b; };
ez::println(Rec{{1, 2}, {3, 4}});  // {{1 2} {3 4}}

ez::println(std::tuple{Rec{1, 2, 3, 4}, "5", 6.0, "7"s, std::pair{std::tuple{8}, std::tuple{Point{10, 11}, "12"sv}}});
// {{{1 2} {3 4}} 5 6 7 {{8} {{10 11} 12}}}
````

It just work.

# Usage

ezprint prints _streamable_ types, types that can be streamed through `std::ostream&` and their composition.

````c++
ez::fprint(os, args...)
ez::fprintln(os, args...)
````

Prints `args` to the `os`. The suffix `ln` adds a newline at the end.

````c++
ez::print(args...)
ez::println(args...)
````

Shorthand for `fprint(std::cout, args...)`.

````c++
ez::sprint(args...)
````

Prints to a `std::string` and returns it.

# Streamable Types

A streamable type is one of the following.

1. A type that can be streamed to `std::ostream&`.
2. A range-like type, ie can be looped over.
3. A tuple-like type, ie has `tuple_size`.
4. An aggregate.

An unstreamable type is printed as an error string. This makes it possible to print types composed of both streamable and unstreamable types.

# Limits

An aggregate can have at most 64 data members. This is controlled by the implementation in `as_tuple.hpp`.
