#ifndef WAD_OPTIONAL_HPP
#define WAD_OPTIONAL_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <optional>

namespace wad
{

template<typename T, typename Arc>
bool save(Arc& arc, const std::optional<T>& v)
{
    const auto savepoint = arc.npos();

    if(v)
    {
        return save(arc, *v);
    }
    // if v does not have its value, write nil.
    if(!save(arc, tag::nil))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename T, typename Arc>
bool load(Arc& arc, std::optional<T>& v)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t))
    {
        return false;
    }
    if(t == tag::nil)
    {
        v = std::nullopt;
        return ;
    }
    arc.seek(savepoint); // to load tag one more time
    if(!load(arc, *v))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

} // wad
#endif// WAD_OPTIONAL_HPP
