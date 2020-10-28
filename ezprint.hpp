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
                size_t sz;
                count_r<T, ubiq_t<is>...>(sz, 0);
                return sz;
            });
        }

        template <typename T>
        using count_t = std::integral_constant<size_t, count<T>()>;

        using std::begin;
        using std::end;
        using std::get;

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
                for(auto& x : t)
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
} // namespace ez

#endif // EZPRINT_HPP_INCLUDED
