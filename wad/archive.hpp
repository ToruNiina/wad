#ifndef WAD_ARCHIVE_HPP
#define WAD_ARCHIVE_HPP
#include "type_traits.hpp"
#include "in_place.hpp"

namespace wad
{
// ---------------------------------------------------------------------------
// archive(arc, T) that works as both save and load.

template<typename Arc, typename T>
// arc.sink() is valid, arc.src() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    detail::negation<has_write_archive_method<T>>
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return save(arc, arg);
}

template<typename Arc, typename T>
// arc.src() is valid, arc.sink() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_src_method<Arc>, detail::negation<has_sink_method<Arc>>,
    detail::negation<has_read_archive_method<T>>
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return load(arc, arg);
}

template<typename Arc, typename T>
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    has_write_archive_method<T>
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return arg.archive(arc);
}

template<typename Arc, typename T>
typename std::enable_if<detail::conjunction<
    has_src_method<Arc>, detail::negation<has_sink_method<Arc>>,
    has_read_archive_method<T>
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return arg.archive(arc);
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

// -------------------------------------------------------------------------------
// archive version

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, Args&& ... args)
{
    return save<T>(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, Args&& ... args)
{
    return load<T>(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    return save<T>(arc, bb, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    return load<T>(arc, bb, std::forward<Args>(args) ...);
}

} // wad
#endif//WAD_ARCHIVE_HPP
