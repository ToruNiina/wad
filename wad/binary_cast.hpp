#ifndef WAD_BINARY_CAST_HPP
#define WAD_BINARY_CAST_HPP
#include "endian.hpp"
#include <type_traits>
#include <utility>
#include <algorithm>
#include <cassert>

namespace wad
{

template<typename T, typename Arc>
bool to_big_endian(Arc& arc, const T& v)
{
    const char* src = reinterpret_cast<const char*>(std::addressof(v));
    if(!arc.is_writable(sizeof(T))) {return false;}

#if defined(WAD_USE_BIG_ENDIAN)
    std::copy(src, src + sizeof(v), arc.sink());
#elif defined(WAD_USE_LITTLE_ENDIAN)
    std::reverse_copy(src, src + sizeof(v), arc.sink());
#else
#error "Neither WAD_USE_BIG_ENDIAN or WAD_USE_LITTLE_ENDIAN is defined."
#endif

    arc.advance(sizeof(T));
    return true;
}

template<typename T, typename Arc>
bool from_big_endian(Arc& arc, T& v)
{
    char* dst = reinterpret_cast<char*>(std::addressof(v));
    if(!arc.is_readable(sizeof(T))) {return false;}

#if defined(WAD_USE_BIG_ENDIAN)
    std::copy_n(arc.src(), sizeof(T), dst);
#elif defined(WAD_USE_LITTLE_ENDIAN)
    std::copy_n(arc.src(), sizeof(T), std::reverse_iterator<char*>(dst + sizeof(T)));
#else
#error "Neither WAD_USE_BIG_ENDIAN or WAD_USE_LITTLE_ENDIAN is defined."
#endif
    arc.advance(sizeof(T));
    return true;
}

} // wad
#endif//WAD_BINARY_CAST_HPP
