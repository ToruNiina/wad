#ifndef WAD_BOOLEAN_HPP
#define WAD_BOOLEAN_HPP
#include "tag.hpp"

namespace wad
{
template<typename Arc>
bool save(Arc& arc, const bool v)
{
    if(v)
    {
        return save(arc, tag::true_tag);
    }
    else
    {
        return save(arc, tag::false_tag);
    }
}

template<typename Arc>
bool load(Arc& arc, bool& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(t == tag::true_tag)
    {
        v = true;
    }
    else if(t == tag::false_tag)
    {
        v = false;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
    return true;
}

} // wad
#endif// WAD_BOOLEAN_HPP
