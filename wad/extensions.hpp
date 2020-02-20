#ifndef WAD_EXTENSIONS_HPP
#define WAD_EXTENSIONS_HPP
#include "archive.hpp"
#include <type_traits>

// These template functions will be activated only when a type T has memfun
// bool save(Arc&) and bool load(Arc&).

namespace wad
{
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

template<typename T, typename Arc>
typename std::enable_if<has_save_method<T>::value, bool>::type
save(const T& v, Arc& arc)
{
    return v.save(arc);
}
template<typename T, typename Arc>
typename std::enable_if<has_load_method<T>::value, bool>::type
load(T& v, Arc& arc)
{
    return v.load(arc);
}

} // wad
#endif// WAD_EXTENSIONS_HPP
