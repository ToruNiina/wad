#ifndef WAD_PAIR_HPP
#define WAD_PAIR_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <utility>

namespace wad
{

template<typename T1, typename T2, typename Arc>
bool save(const std::pair<T1, T2>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) + 2u;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!save(v.first, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!save(v.second, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename T1, typename T2, typename Arc>
bool load(std::pair<T1, T2>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}

    const std::uint8_t expected = static_cast<std::uint8_t>(tag::fixarray_lower) + 2u;
    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }
    if(!load(v.first, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!load(v.second, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

} // wad
#endif// WAD_PAIR_HPP
