#ifndef WAD_ATOMIC_HPP
#define WAD_ATOMIC_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <atomic>

namespace wad
{

template<typename T, typename Arc>
bool save(Arc& arc, const std::atomic<T>& v)
{
    return save(arc, v.load());
}

template<typename T, typename Arc>
bool load(Arc& arc, std::atomic<T>& v)
{
    T x;
    if(load(arc, x))
    {
        v.store(x);
        return true;
    }
    return false;
}

} // wad
#endif// WAD_ARRAY_HPP
