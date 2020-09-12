#ifndef EZPRINT_HPP_INCLUDED
#define EZPRINT_HPP_INCLUDED

#include <experimental/type_traits>
#include <iostream>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "as_tuple.hpp"

namespace ez
{
    namespace detail
    {
        template <typename F, size_t... Is>
        auto index_over(F&& f, std::index_sequence<Is...>)
        {
            return std::forward<F>(f)(std::integral_constant<size_t, Is>{}...);
        }

        template <size_t N, typename F>
        auto index_upto(F&& f)
        {
            return index_over(std::forward<F>(f), std::make_index_sequence<N>{});
        }

        using std::begin;
        template <typename T>
        using begin_t = decltype(begin(std::declval<T>()));
        using std::end;
        template <typename T>
        using end_t = decltype(end(std::declval<T>()));
        using std::get;
        template <typename T>
        using tuple_size_t = typename std::tuple_size<T>::type;
        template <typename T>
        using mapped_type = typename T::mapped_type;
        template <typename T>
        using stream_t = decltype(std::declval<std::ostream&>() << std::declval<T>());

        template <typename T>
        void fprint_aggregate(std::ostream&, T&&);

        template <typename T>
        void fprint(std::ostream& os, T&& t)
        {
            using rT = std::remove_reference_t<T>;
            if constexpr(std::experimental::is_detected_v<stream_t, T>)
                os << t;
            else if constexpr(std::experimental::is_detected_v<begin_t, T> &&
                              std::experimental::is_detected_v<end_t, T>)
            {
                auto b = begin(t);
                auto e = end(t);
                int i = 0;
                os << '{';
                for(; b != e; i++, b++)
                {
                    if(i > 0)
                        os << ' ';
                    if constexpr(std::experimental::is_detected_v<mapped_type, rT>)
                    {
                        fprint(os, b->first);
                        os << ": ";
                        fprint(os, b->second);
                    }
                    else
                        fprint(os, *b);
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
                fprint_aggregate(os, t);
            else
                os << "!(UNKNOWN TYPE)";
        }

        struct ubiq
        {
            template <typename T>
            operator T();
        };

        template <size_t>
        using ubiq_t = ubiq;

        template <typename T, typename... Ubiqs>
        constexpr auto count_r(size_t& sz, int) -> std::void_t<decltype(T{Ubiqs{}...})>
        {
            sz = sizeof...(Ubiqs);
        }

        template <typename T, typename, typename... Ubiqs>
        constexpr auto count_r(size_t& sz, float)
        {
            count_r<T, Ubiqs...>(sz, 0);
        }

        template <typename T, size_t... Is>
        constexpr auto count(std::index_sequence<Is...>)
        {
            size_t sz;
            count_r<T, ubiq_t<Is>...>(sz, 0);
            return sz;
        }

        template <typename T>
        constexpr auto count()
        {
            return count<T>(std::make_index_sequence<sizeof(T)>{});
        }

        template <typename T>
        using count_t = std::integral_constant<size_t, count<T>()>;

        template <typename T>
        void fprint_aggregate(std::ostream& os, T&& t)
        {
            fprint(os, as_tuple(t, count_t<std::remove_reference_t<T>>{}));
        }

        template <typename T, typename... Ts>
        void fprint(std::ostream& os, T&& t, Ts&&... ts)
        {
            fprint(os, t);
            (fprint(os << ' ', ts), ...);
        }
    } // namespace detail

    template <typename... Ts>
    void fprint(std::ostream& os, Ts&&... ts)
    {
        detail::fprint(os, std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    void fprintln(std::ostream& os, Ts&&... ts)
    {
        fprint(os, std::forward<Ts>(ts)...);
        os << '\n';
    }

    template <typename... Ts>
    void print(Ts&&... ts)
    {
        fprint(std::cout, std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    void println(Ts&&... ts)
    {
        fprint(std::cout, std::forward<Ts>(ts)...);
        std::cout << '\n';
    }

    template <typename... Ts>
    std::string sprint(Ts&&... ts)
    {
        std::stringstream ss;
        fprint(ss, std::forward<Ts>(ts)...);
        return ss.str();
    }
} // namespace ez

#endif // EZPRINT_HPP_INCLUDED
