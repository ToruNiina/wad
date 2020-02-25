#ifndef WAD_TYPE_TRAITS_HPP
#define WAD_TYPE_TRAITS_HPP
#include <type_traits>

namespace wad
{

// ============================================================================
// std::-like meta functions

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

// ============================================================================
// has_xxx

// ----------------------------------------------------------------------------
// has_save_method, has_load_method

namespace detail
{
// this also finds template member functions.
template<typename T, bool (T::*ptr)(wad::write_archive&) const = &T::save>
std::true_type  has_save_method_impl(T*);
template<typename T>
std::false_type has_save_method_impl(...);

template<typename T, bool (T::*ptr)(wad::read_archive&) = &T::load>
std::true_type  has_load_method_impl(T*);
template<typename T>
std::false_type has_load_method_impl(...);
} // detail

template<typename T>
struct has_save_method : decltype(detail::has_save_method_impl<T>(nullptr)) {};
template<typename T>
struct has_load_method : decltype(detail::has_load_method_impl<T>(nullptr)) {};

// ----------------------------------------------------------------------------
// has_write_archive_method, has_read_archive_method

namespace detail
{
template<typename T, bool (T::*ptr)(wad::write_archive&) = &T::archive>
std::true_type  has_write_archive_method_impl(T*);
template<typename T>
std::false_type has_write_archive_method_impl(...);

template<typename T, bool (T::*ptr)(wad::read_archive&) = &T::archive>
std::true_type  has_read_archive_method_impl(T*);
template<typename T>
std::false_type has_read_archive_method_impl(...);
} // detail

template<typename T>
struct has_write_archive_method: decltype(detail::has_write_archive_method_impl<T>(nullptr)) {};
template<typename T>
struct has_read_archive_method : decltype(detail::has_write_archive_method_impl<T>(nullptr)) {};

} // wad
#endif// WAD_TYPE_TRAITS_HPP
