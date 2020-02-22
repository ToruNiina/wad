#ifndef WAD_LIST_HPP
#define WAD_LIST_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <utility>
#include <list>

namespace wad
{

template<typename T, typename Alloc, typename Arc>
bool save(const std::list<T, Alloc>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    if(v.size() < 16)
    {
        const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) +
                               static_cast<std::uint8_t>(v.size());
        if(!save(static_cast<tag>(t), arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(v.size() <= 0xFFFF)
    {
        if(!save(tag::array16, arc)) {return false;}

        const std::uint16_t sz = v.size();
        if(!to_big_endian(sz, arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(v.size() <= 0xFFFFFFFF)
    {
        if(!save(tag::array32, arc)) {return false;}

        const std::uint32_t sz = v.size();
        if(!to_big_endian(sz, arc))
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
        if(!save(elem, arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

template<typename T, typename Alloc, typename Arc>
bool load(std::list<T, Alloc>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(        static_cast<std::uint8_t>(tag::fixarray_lower) <= byte &&
       byte <= static_cast<std::uint8_t>(tag::fixarray_upper))
    {
        v.resize(byte - static_cast<std::uint8_t>(tag::fixarray_lower));
    }
    else if(t == tag::array16)
    {
        std::uint16_t sz = 0;
        if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        v.resize(sz);
    }
    else if(t == tag::array32)
    {
        std::uint32_t sz = 0;
        if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        v.resize(sz);
    }
    else
    {
        arc.seek(savepoint);
        return false;
    }

    for(auto& elem : v)
    {
        if(!load(elem, arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

} // wad
#endif// WAD_LIST_HPP
