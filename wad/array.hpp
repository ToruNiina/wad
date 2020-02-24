#ifndef WAD_ARRAY_HPP
#define WAD_ARRAY_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include "type.hpp"
#include <utility>
#include <array>

namespace wad
{

template<typename T, std::size_t N, typename Arc>
bool save(Arc& arc, const std::array<T, N>& v)
{
    const auto savepoint = arc.npos();

    if(!save<type::array>(arc, N))
    {
        arc.seek(savepoint);
        return false;
    }
    for(const auto& elem : v)
    {
        if(!save(arc, elem))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

template<typename T, std::size_t N, typename Arc>
bool load(Arc& arc, std::array<T, N>& v)
{
    const auto savepoint = arc.npos();
    std::size_t len;
    if(!load<type::array>(arc, len) || len != N)
    {
        arc.seek(savepoint);
        return false;
    }
    for(auto& elem : v)
    {
        if(!load(arc, elem))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

} // wad
#endif// WAD_ARRAY_HPP
