#ifndef WAD_VALARRAY_HPP
#define WAD_VALARRAY_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <valarray>

namespace wad
{

template<typename T, typename Arc>
bool save(Arc& arc, const std::valarray<T>& v)
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

template<typename T, typename Arc>
bool load(Arc& arc, std::valarray<T>& v)
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
#endif// WAD_VALARRAY_HPP
