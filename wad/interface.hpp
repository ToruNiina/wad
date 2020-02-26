#ifndef WAD_INTERFACE_HPP
#define WAD_INTERFACE_HPP
#include "type_traits.hpp"

namespace wad
{

// ---------------------------------------------------------------------------
// support v.save(arc) and v.load(arc);

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
// support v.archive(arc).
//
// if v has v.archive(arc) method instead of v.save/load, call v.archive from
// wad::save() and wad::load().

template<typename T, typename Arc>
typename std::enable_if<detail::conjunction<
    has_write_archive_method<T>, detail::negation<has_save_method<T>>,
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>
    >::value, bool>::type
save(Arc& arc, T& v)
{
    return v.archive(arc);
}

template<typename T, typename Arc>
typename std::enable_if<detail::conjunction<
    has_read_archive_method<T>, detail::negation<has_load_method<T>>,
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>
    >::value, bool>::type
load(Arc& arc, T& v)
{
    return v.archive(arc);
}

} // wad
#endif// WAD_INTERFACE_HPP
