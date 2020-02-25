#ifndef WAD_TYPE_TRAITS_HPP
#define WAD_TYPE_TRAITS_HPP
#include <type_traits>

namespace wad
{
namespace detail
{

template<typename T>
struct is_char_literal: std::false_type {};
template<std::size_t N>
struct is_char_literal<const char(&)[N]>: std::true_type {};
template<>
struct is_char_literal<const char*>: std::true_type {};

template<typename ...>
struct conjunction : std::true_type{};
template<typename T>
struct conjunction<T> : T
{
    static_assert(std::is_convertible<decltype(T::value), bool>::value,
                  "conjunction<T> requires T::value is convertible to bool");
};
template<typename T, typename ... Ts>
struct conjunction<T, Ts...> :
    std::conditional<static_cast<bool>(T::value), conjunction<Ts...>, T>::type
{};

template<typename ...>
struct disjunction : std::false_type{};
template<typename T>
struct disjunction<T> : T
{
    static_assert(std::is_convertible<decltype(T::value), bool>::value,
                  "disjunction<T> requires T::value is convertible to bool");
};
template<typename T, typename ... Ts>
struct disjunction<T, Ts...> :
    std::conditional<static_cast<bool>(T::value), T, disjunction<Ts...>>::type
{};

template<typename T>
struct negation : std::integral_constant<bool, !static_cast<bool>(T::value)>{};


} // detail
} // wad
#endif// WAD_TYPE_TRAITS_HPP
