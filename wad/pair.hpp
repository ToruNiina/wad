#ifndef WAD_PAIR_HPP
#define WAD_PAIR_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <utility>

namespace wad
{

template<typename T1, typename T2, typename Arc>
bool save(Arc& arc, const std::pair<T1, T2>& v)
{
    const auto savepoint = arc.npos();

    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) + 2u;
    if(!save(arc, static_cast<tag>(t)))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!save(arc, v.first))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!save(arc, v.second))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename T1, typename T2, typename Arc>
bool load(Arc& arc, std::pair<T1, T2>& v)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t)) {return false;}

    const std::uint8_t expected = static_cast<std::uint8_t>(tag::fixarray_lower) + 2u;
    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }
    if(!load(arc, v.first))
    {
        arc.seek(savepoint);
        return false;
    }
    if(!load(arc, v.second))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

} // wad
#endif// WAD_PAIR_HPP
