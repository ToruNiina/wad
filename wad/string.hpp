#ifndef WAD_STRING_HPP
#define WAD_STRING_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include "integer.hpp"
#include <algorithm>
#include <utility>

namespace wad
{

template<typename Traits, typename Alloc, typename Arc>
bool save(const std::basic_string<char, Traits, Alloc>& v, Arc& arc)
{
    std::size_t written = 0;
    if(v.size() < 32)
    {
        const std::uint8_t t = static_cast<std::uint8_t>(tag::fixstr_lower) +
                               static_cast<std::uint8_t>(v.size());
        if(!save(static_cast<tag>(t), arc)) {return false;}
        written = 1;
    }
    else if(v.size() <= 0xFF)
    {
        if(!save(tag::str8, arc)) {return false;}
        const std::uint8_t sz = v.size();
        save(sz, arc);
        written = 2;
    }
    else if(v.size() <= 0xFFFF)
    {
        if(!save(tag::str16, arc)) {return false;}
        const std::uint16_t sz = v.size();
        save(sz, arc);
        written = 3;
    }
    else if(v.size() <= 0xFFFFFFFF)
    {
        if(!save(tag::str32, arc)) {return false;}
        const std::uint32_t sz = v.size();
        save(sz, arc);
        written = 5;
    }
    else
    {
        return false;
    }
    if(!arc.is_writable(v.size())) {arc.retrace(written); return false;}

    std::copy(v.begin(), v.end(), arc.sink());
    arc.advance(v.size());
    return true;
}

template<typename Traits, typename Alloc, typename Arc>
bool load(std::basic_string<char, Traits, Alloc>& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    std::size_t read = 0;

    const auto byte = static_cast<std::uint8_t>(t);
    if(        static_cast<std::uint8_t>(tag::fixstr_lower) <= byte &&
       byte <= static_cast<std::uint8_t>(tag::fixstr_upper))
    {
        v.resize(byte - static_cast<std::uint8_t>(tag::fixstr_lower));
        read = 1;
    }
    else if(t == tag::str8)
    {
        std::uint8_t sz = 0;
        if(!load(sz, arc)) {arc.retrace(1); return false;}
        v.resize(sz);
        read = 2;
    }
    else if(t == tag::str16)
    {
        std::uint16_t sz = 0;
        if(!load(sz, arc)) {arc.retrace(1); return false;}
        v.resize(sz);
        read = 3;
    }
    else if(t == tag::str32)
    {
        std::uint32_t sz = 0;
        if(!load(sz, arc)) {arc.retrace(1); return false;}
        v.resize(sz);
        read = 5;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
    if(!arc.is_readable(v.size())) {arc.retrace(read); return false;}

    std::copy_n(arc.src(), v.size(), v.begin());
    arc.advance(v.size());
    return true;
}

} // wad
#endif// WAD_STRING_HPP
