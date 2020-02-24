#ifndef WAD_STRING_HPP
#define WAD_STRING_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <algorithm>
#include <utility>
#include <string>
#include <cstring>

namespace wad
{

template<typename Traits, typename Alloc, typename Arc>
bool save(Arc& arc, const std::basic_string<char, Traits, Alloc>& v)
{
    const auto savepoint = arc.npos();

    if(!save_length<type::str>(arc, v.size()) || !arc.is_writable(v.size()))
    {
        arc.seek(savepoint);
        return false;
    }

    std::copy(v.begin(), v.end(), arc.sink());
    arc.advance(v.size());
    return true;
}

template<typename Traits, typename Alloc, typename Arc>
bool load(Arc& arc, std::basic_string<char, Traits, Alloc>& v)
{
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load_length<type::str>(arc, len) || !arc.is_readable(len))
    {
        arc.seek(savepoint);
        return false;
    }

    v.resize(len);
    std::copy_n(arc.src(), v.size(), v.begin());
    arc.advance(v.size());
    return true;
}

template<typename Arc>
bool save(Arc& arc, const char* v)
{
    const auto savepoint = arc.npos();

    const auto len = std::strlen(v);
    if(!save_length<type::str>(arc, len) || !arc.is_writable(len))
    {
        arc.seek(savepoint);
        return false;
    }

    std::copy_n(v, len, arc.sink());
    arc.advance(len);
    return true;
}

} // wad
#endif// WAD_STRING_HPP
