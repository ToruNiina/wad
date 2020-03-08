#ifndef WAD_UNORDERED_SET_HPP
#define WAD_UNORDERED_SET_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <unordered_set>

namespace wad
{

template<typename T, typename Hash, typename Pred, typename Alloc, typename Arc>
bool save(Arc& arc, const std::unordered_set<T, Hash, Pred, Alloc>& v)
{
    const auto savepoint = arc.npos();
    if(!save_length<type::array>(arc, v.size()))
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

template<typename T, typename Hash, typename Pred, typename Alloc, typename Arc>
bool load(Arc& arc, std::unordered_set<T, Hash, Pred, Alloc>& v)
{
    static_assert(std::is_default_constructible<T>::value,
                  "To load a type T, T must be default constructible.");
    const auto savepoint = arc.npos();
    std::size_t len;
    if(!load_length<type::array>(arc, len))
    {
        arc.seek(savepoint);
        return false;
    }

    for(std::size_t i=0; i<len; ++i)
    {
        T elem;
        if(!load(arc, elem))
        {
            arc.seek(savepoint);
            return false;
        }
        v.insert(std::move(elem));
    }
    return true;
}

} // wad
#endif// WAD_UNORDERED_SET_HPP
