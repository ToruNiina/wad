#ifndef WAD_EXTENSIONS_HPP
#define WAD_EXTENSIONS_HPP
#include "archive.hpp"
#include "type_traits.hpp"

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
save(Arc& arc, const T& v)
{
    return v.save(arc);
}
template<typename T, typename Arc>
typename std::enable_if<has_load_method<T>::value, bool>::type
load(Arc& arc, T& v)
{
    return v.load(arc);
}

// ---------------------------------------------------------------------------
// support v.archive(arc);

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

template<typename T, typename Arc>
typename std::enable_if<detail::conjunction<has_write_archive_method<T>,
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>
    >::value, bool>::type
save(Arc& arc, T& v)
{
    return v.archive(arc);
}

template<typename T, typename Arc>
typename std::enable_if<detail::conjunction<has_read_archive_method<T>,
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>
    >::value, bool>::type
load(Arc& arc, T& v)
{
    return v.archive(arc);
}

} // wad
#endif// WAD_EXTENSIONS_HPP
