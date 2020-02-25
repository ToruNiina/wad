#ifndef WAD_ARCHIVE_HPP
#define WAD_ARCHIVE_HPP
#include "type_traits.hpp"

namespace wad
{

template<typename Arc, typename T>
// arc.sink() is valid, arc.src() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>
    detail::negation<has_write_archive_method<T>>,
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return save(arc, arg);
}
template<typename Arc, typename T>
// arc.src() is valid, arc.sink() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_src_method<Arc>, detail::negation<has_sink_method<Arc>>
    detail::negation<has_read_archive_method<T>>,
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return load(arc, arg);
}

} // wad
#endif//WAD_ARCHIVE_HPP
