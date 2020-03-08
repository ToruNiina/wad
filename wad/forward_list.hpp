#ifndef WAD_FORWARD_LIST_HPP
#define WAD_FORWARD_LIST_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <iterator>
#include <forward_list>

namespace wad
{

template<typename T, typename Alloc, typename Arc>
bool save(Arc& arc, const std::forward_list<T, Alloc>& v)
{
    const auto savepoint = arc.npos();

    const std::size_t len = std::distance(v.begin(), v.end());
    if(!save_length<type::array>(arc, len))
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
    static_assert(std::is_default_constructible<T>::value,
                  "To load a type T, T must be default constructible.");
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load_length<type::array>(arc, len))
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
