#ifndef WAD_UNORDERED_MAP_HPP
#define WAD_UNORDERED_MAP_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <unordered_map>

namespace wad
{

template<typename Key, typename T, typename Hash, typename Pred, typename Alloc, typename Arc>
bool save(Arc& arc, const std::unordered_map<Key, T, Hash, Pred, Alloc>& v)
{
    const auto savepoint = arc.npos();

    if(!save_length<type::map>(arc, v.size()))
    {
        arc.seek(savepoint);
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

template<typename Key, typename T, typename Hash, typename Pred, typename Alloc, typename Arc>
bool load(Arc& arc, std::unordered_map<Key, T, Hash, Pred, Alloc>& v)
{
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load_length<type::array>(arc, len))
    {
        arc.seek(savepoint);
        return false;
    }

    for(std::size_t i=0; i<len; ++i)
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
#endif// WAD_UNORDERED_MAP_HPP
