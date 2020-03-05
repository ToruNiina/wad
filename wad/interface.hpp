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

} // wad
#endif// WAD_INTERFACE_HPP
