#ifndef WAD_MAP_HPP
#define WAD_MAP_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <utility>
#include <map>

namespace wad
{

template<typename Key, typename T, typename Compare, typename Alloc, typename Arc>
bool save(Arc& arc, const std::map<Key, T, Compare, Alloc>& v)
{
    const auto savepoint = arc.npos();

    if(v.size() < 16)
    {
        const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) +
                               static_cast<std::uint8_t>(v.size());
        if(!save(arc, static_cast<tag>(t)))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(v.size() <= 0xFFFF)
    {
        if(!save(arc, tag::map16)) {return false;}

        const std::uint16_t sz = v.size();
        if(!to_big_endian(arc, sz))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(v.size() <= 0xFFFFFFFF)
    {
        if(!save(arc, tag::map32)) {return false;}

        const std::uint32_t sz = v.size();
        if(!to_big_endian(arc, sz))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else
    {
        return false;
    }
    for(const auto& elem : v)
    {
        if(!save(arc, elem.first))
        {
            arc.seek(savepoint);
            return false;
        }
        if(!save(arc, elem.second))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

template<typename Key, typename T, typename Compare, typename Alloc, typename Arc>
bool load(Arc& arc, std::map<Key, T, Compare, Alloc>& v)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t)) {return false;}

    std::size_t size = 0;
    const auto byte = static_cast<std::uint8_t>(t);
    if(        static_cast<std::uint8_t>(tag::fixmap_lower) <= byte &&
       byte <= static_cast<std::uint8_t>(tag::fixmap_upper))
    {
        size = byte - static_cast<std::uint8_t>(tag::fixmap_lower);
    }
    else if(t == tag::map16)
    {
        std::uint16_t sz = 0;
        if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        size = sz;
    }
    else if(t == tag::map32)
    {
        std::uint32_t sz = 0;
        if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        size = sz;
    }
    else
    {
        arc.seek(savepoint);
        return false;
    }

    for(std::size_t i=0; i<size; ++i)
    {
        Key k; T val;
        if(!load(arc, k))
        {
            arc.seek(savepoint);
            return false;
        }
        if(!load(arc, val))
        {
            arc.seek(savepoint);
            return false;
        }
        v[k] = val;
    }
    return true;
}

} // wad
#endif// WAD_MAP_HPP
