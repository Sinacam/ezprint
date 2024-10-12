#ifndef EZPRINT_HPP_INCLUDED
#define EZPRINT_HPP_INCLUDED

#include <experimental/type_traits>
#include <iostream>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <utility>

#if __cpp_lib_format >= 201907L
#define EZPRINT_ENABLE_FORMAT 1
#include<format>
#else
#define EZPRINT_ENABLE_FORMAT 0
#endif // __cpp_lib_format >= 201907L

namespace ez::detail
{
    inline constexpr size_t max_fields = 64;

    // as_tuple returns the fields of an aggregate as a tuple, in order
    template <typename T>
    inline auto as_tuple(T&&, std::integral_constant<size_t, 0>)
    {
        return std::tuple{};
    }

    // Overlaying a tuple onto the object violates strict-aliasing, requires a custom tuple
    // implementation to ensure layout and has problems with nested/enum/non-pod types.
    // In short, it was a very risky way of doing things unsatisfactorily prior C++17.
    // So instead we brute force with "all" possible field counts with structured bindings.
    // Unfortunately, this means we have a limit on field counts.
    // Current limit of 64 has absolutely no reason whatsoever, 42 might have been better. Ah well.
    // The macro is undefined at the end.
    // Returns the fields as a tuple of lvalue references to them, in order.
#define DEFINE_AS_TUPLE(N, ...)                                                                    \
    template <typename T>                                                                          \
    inline auto as_tuple(T&& t, std::integral_constant<size_t, N>)                                 \
    {                                                                                              \
        auto& [__VA_ARGS__] = t;                                                                   \
        return std::forward_as_tuple(__VA_ARGS__);                                                 \
    }

    DEFINE_AS_TUPLE(1, x0)
    DEFINE_AS_TUPLE(2, x0, x1)
    DEFINE_AS_TUPLE(3, x0, x1, x2)
    DEFINE_AS_TUPLE(4, x0, x1, x2, x3)
    DEFINE_AS_TUPLE(5, x0, x1, x2, x3, x4)
    DEFINE_AS_TUPLE(6, x0, x1, x2, x3, x4, x5)
    DEFINE_AS_TUPLE(7, x0, x1, x2, x3, x4, x5, x6)
    DEFINE_AS_TUPLE(8, x0, x1, x2, x3, x4, x5, x6, x7)
    DEFINE_AS_TUPLE(9, x0, x1, x2, x3, x4, x5, x6, x7, x8)
    DEFINE_AS_TUPLE(10, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9)
    DEFINE_AS_TUPLE(11, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)
    DEFINE_AS_TUPLE(12, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)
    DEFINE_AS_TUPLE(13, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)
    DEFINE_AS_TUPLE(14, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13)
    DEFINE_AS_TUPLE(15, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14)
    DEFINE_AS_TUPLE(16, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15)
    DEFINE_AS_TUPLE(17, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16)
    DEFINE_AS_TUPLE(18, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17)
    DEFINE_AS_TUPLE(19, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18)
    DEFINE_AS_TUPLE(20, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19)
    DEFINE_AS_TUPLE(21, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20)
    DEFINE_AS_TUPLE(22, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21)
    DEFINE_AS_TUPLE(23, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22)
    DEFINE_AS_TUPLE(24, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23)
    DEFINE_AS_TUPLE(25, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24)
    DEFINE_AS_TUPLE(26, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25)
    DEFINE_AS_TUPLE(27, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26)
    DEFINE_AS_TUPLE(28, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27)
    DEFINE_AS_TUPLE(29, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28)
    DEFINE_AS_TUPLE(30, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29)
    DEFINE_AS_TUPLE(31, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30)
    DEFINE_AS_TUPLE(32, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31)
    DEFINE_AS_TUPLE(33, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32)
    DEFINE_AS_TUPLE(34, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33)
    DEFINE_AS_TUPLE(35, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34)
    DEFINE_AS_TUPLE(36, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35)
    DEFINE_AS_TUPLE(37, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36)
    DEFINE_AS_TUPLE(38, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37)
    DEFINE_AS_TUPLE(39, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38)
    DEFINE_AS_TUPLE(40, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39)
    DEFINE_AS_TUPLE(41, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40)
    DEFINE_AS_TUPLE(42, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41)
    DEFINE_AS_TUPLE(43, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42)
    DEFINE_AS_TUPLE(44, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43)
    DEFINE_AS_TUPLE(45, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44)
    DEFINE_AS_TUPLE(46, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45)
    DEFINE_AS_TUPLE(47, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46)
    DEFINE_AS_TUPLE(48, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47)
    DEFINE_AS_TUPLE(49, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48)
    DEFINE_AS_TUPLE(50, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49)
    DEFINE_AS_TUPLE(51, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50)
    DEFINE_AS_TUPLE(52, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51)
    DEFINE_AS_TUPLE(53, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52)
    DEFINE_AS_TUPLE(54, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53)
    DEFINE_AS_TUPLE(55, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54)
    DEFINE_AS_TUPLE(56, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55)
    DEFINE_AS_TUPLE(57, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56)
    DEFINE_AS_TUPLE(58, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57)
    DEFINE_AS_TUPLE(59, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58)
    DEFINE_AS_TUPLE(60, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59)
    DEFINE_AS_TUPLE(61, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60)
    DEFINE_AS_TUPLE(62, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61)
    DEFINE_AS_TUPLE(63, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62)
    DEFINE_AS_TUPLE(64, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, x63)

#undef DEFINE_AS_TUPLE
} // namespace ez::detail

namespace ez
{
    namespace detail
    {
        using std::size_t;

        // indexing helper, see index_upto
        template <typename F, size_t... Is>
        inline constexpr decltype(auto) index_over(F&& f, std::index_sequence<Is...>)
        {
            return std::forward<F>(f)(std::integral_constant<size_t, Is>{}...);
        }

        // indexing helper, use as
        //      index_upto<N>([&](auto... is){});
        // where is are integral constants in [0, N).
        template <size_t N, typename F>
        inline constexpr decltype(auto) index_upto(F&& f)
        {
            return index_over(std::forward<F>(f), std::make_index_sequence<N>{});
        }

        // ubiq is used for scanning aggregate fields
        struct ubiq
        {
            template <typename T>
            operator T();
        };

        // ubiq_t is used for exapnding indices into ubiq packs
        template <size_t>
        using ubiq_t = ubiq;

        // base case of counting fields
        // selected only when the ubiqs pack is the same length as the fields of T
        template <typename T, typename... Ubiqs>
        inline constexpr auto count_r(size_t& sz, int) -> std::void_t<decltype(T{Ubiqs{}...})>
        {
            sz = sizeof...(Ubiqs);
        }

        // recursion for counting fields, taking off one ubiq at a time
        // selected only when the base case cannot be selected
        // float parameter used to prefer base case in overload resolution
        template <typename T, typename, typename... Ubiqs>
        inline constexpr auto count_r(size_t& sz, float)
        {
            count_r<T, Ubiqs...>(sz, 0);
        }

        // returns number of fields in T
        // number of fields in T muts be less than sizeof(T)
        template <typename T>
        inline constexpr auto count()
        {
            return index_upto<std::min(sizeof(T), max_fields)>([](auto... is) {
                size_t sz = 0;
                count_r<T, ubiq_t<is>...>(sz, 0);
                return sz;
            });
        }

        template <typename T>
        using count_t = std::integral_constant<size_t, count<T>()>;

        using std::begin;
        using std::end;
        using std::get;

        #if __cpp_char8_t < 201811L
            using char8_t = char;
        #endif

        template <typename T, typename... Chars>
        inline constexpr bool is_string_of_v = (std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>, Chars> || ...);
        template <typename T>
        inline constexpr bool is_string_v = is_string_of_v<T, char, wchar_t, char8_t, char16_t, char32_t>;
        template <typename T>
        using stream_t = decltype(std::declval<std::ostream&>() << std::declval<T>());
        template <typename T>
        using begin_t = decltype(begin(std::declval<T>()));
        template <typename T>
        using end_t = decltype(end(std::declval<T>()));
        template <typename T>
        using mapped_type = typename T::mapped_type;
        template <typename T>
        using tuple_size_t = typename std::tuple_size<T>::type;

        template <typename T>
        inline void fprint(std::ostream& os, T&& t)
        {
            // T is always a lvalue reference
            using rT = std::remove_reference_t<T>;

            // order of if is important, the desired behaviour is to supply alternative
            // printing formats only when the most natural isn't available
            if constexpr(std::experimental::is_detected_v<stream_t, T> && (!std::is_array_v<rT> || is_string_v<rT>))
                os << t;
            else if constexpr(std::experimental::is_detected_v<begin_t, T> &&
                              std::experimental::is_detected_v<end_t, T>)
            {
                int i = 0;
                os << '{';
                for(auto&& x : t)
                {
                    if(i > 0)
                        os << ' ';

                    // this is a somewhat special case, where the presence of mapped_type is
                    // taken as being an associative container
                    if constexpr(std::experimental::is_detected_v<mapped_type, rT>)
                    {
                        fprint(os, x.first);
                        os << ": ";
                        fprint(os, x.second);
                    }
                    else
                        fprint(os, x);

                    i++;
                }
                os << '}';
            }
            else if constexpr(std::experimental::is_detected_v<tuple_size_t, rT>)
            {
                constexpr auto len = std::tuple_size_v<rT>;
                if constexpr(len > 0)
                    index_upto<len>([&](auto, auto... is) {
                        os << '{';
                        fprint(os, get<0>(t));
                        (fprint(os << ' ', get<decltype(is){}>(t)), ...);
                        os << '}';
                    });
                else
                    os << "{}";
            }
            else if constexpr(std::is_aggregate_v<rT>)
                fprint(os, as_tuple(t, count_t<rT>{}));
            else
                os << "!(UNKNOWN TYPE)";
        }

        template <typename T, typename... Ts>
        inline void fprint(std::ostream& os, T&& t, Ts&&... ts)
        {
            fprint(os, t);
            (fprint(os << ' ', ts), ...);
        }

        inline void fprint(std::ostream&) {}

    } // namespace detail

    template <typename... Ts>
    inline void fprint(std::ostream& os, Ts&&... ts)
    {
        detail::fprint(os, std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    inline void fprintln(std::ostream& os, Ts&&... ts)
    {
        fprint(os, std::forward<Ts>(ts)...);
        os << '\n';
    }

    template <typename... Ts>
    inline void print(Ts&&... ts)
    {
        fprint(std::cout, std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    inline void println(Ts&&... ts)
    {
        fprint(std::cout, std::forward<Ts>(ts)...);
        std::cout << '\n';
    }

    template <typename... Ts>
    inline std::string sprint(Ts&&... ts)
    {
        std::stringstream ss;
        fprint(ss, std::forward<Ts>(ts)...);
        return ss.str();
    }

#if EZPRINT_ENABLE_FORMAT
    namespace detail
    {
#if __cplusplus >= 202302L
        template<typename T>
        concept formattable = std::formattable<T, char>;
#else
        template<typename T, typename = void>
        struct formattable_t : std::false_type
        {
        };

        template<typename T>
        struct formattable_t<T, std::void_t<
            decltype(std::formatter<std::remove_cvref_t<T>>().format(
                std::declval<std::remove_cvref_t<T>>(), std::declval<std::format_context&>()
            ))
        >> : std::true_type
        {
        };

        template<typename T>
        inline constexpr bool formattable = formattable_t<T>::value;
#endif // __cplusplus >= 202302L
        template<typename T>
        decltype(auto) ez_arg(T&& x)
        {
            if constexpr(formattable<T>)
                return std::forward<T>(x);
            else
                return sprint(x);
        }

        template<typename T>
        using ez_arg_value_t = decltype(ez_arg(std::declval<T>()));
    }

    template<typename... Args>
    using format_string_of = std::format_string<detail::ez_arg_value_t<std::remove_reference_t<Args>>...>;

    template<typename... Args>
    inline std::string sprintf(format_string_of<Args...> fmt, Args&&... args)
    {
        auto args_tuple = std::forward_as_tuple(detail::ez_arg(args)...);
        return std::vformat(
            fmt.get(),
            std::apply([](auto&... args) { return std::make_format_args(args...); }, args_tuple)
        );
    }

    template<typename... Args>
    inline void fprintf(std::ostream& os, format_string_of<Args...> fmt, Args&&... args)
    {
        auto args_tuple = std::forward_as_tuple(detail::ez_arg(args)...);
        os << std::vformat(
            fmt.get(),
            std::apply([](auto&... args) { return std::make_format_args(args...); }, args_tuple)
        );
    }

    template<typename... Args>
    inline void printf(format_string_of<Args...> fmt, Args&&... args)
    {
        auto args_tuple = std::forward_as_tuple(detail::ez_arg(args)...);
        std::cout << std::vformat(
            fmt.get(),
            std::apply([](auto&... args) { return std::make_format_args(args...); }, args_tuple)
        );
    }

    template<typename T>
    struct formatter : std::formatter<std::string>
    {
        template<class Ctx>
        Ctx::iterator format(T& x, Ctx& ctx) const
        {
            return std::formatter<std::string>::format(sprint(x), ctx);
        }
    };

#endif // EZPRINT_ENABLE_FORMAT
#undef EZPRINT_ENABLE_FORMAT

} // namespace ez

#endif // EZPRINT_HPP_INCLUDED
