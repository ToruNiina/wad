#ifndef WAD_ENUM_HPP
#define WAD_ENUM_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <type_traits>

namespace wad
{

template<typename T, typename Arc>
typename std::enable_if<std::is_enum<T>::value, bool>::type
save(Arc& arc, const T& v)
{
    return save(arc, static_cast<typename std::underlying_type<T>::type>(v));
}

template<typename T, typename Arc>
typename std::enable_if<std::is_enum<T>::value, bool>::type
load(Arc& arc, T& v)
{
    typename std::underlying_type<T>::type underlying;
    return load(arc, underlying);
}

} // wad
#endif// WAD_DEQUE_HPP
