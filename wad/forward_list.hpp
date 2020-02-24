#ifndef WAD_FORWARD_LIST_HPP
#define WAD_FORWARD_LIST_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <forward_list>

namespace wad
{

template<typename T, typename Alloc, typename Arc>
bool save(Arc& arc, const std::forward_list<T, Alloc>& v)
{
    const auto savepoint = arc.npos();

    if(!save<type::array>(arc, v.size()))
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

template<typename T, typename Alloc, typename Arc>
bool load(Arc& arc, std::forward_list<T, Alloc>& v)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t)) {return false;}

    std::size_t len;
    if(!load<type::array>(arc, len))
    {
        arc.seek(savepoint);
        return false;
    }
    v.resize(len);

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
#endif// WAD_FORWARD_LIST_HPP
