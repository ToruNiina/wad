#ifndef WAD_BOOLEAN_HPP
#define WAD_BOOLEAN_HPP
#include "tag.hpp"

namespace wad
{
template<typename Arc>
bool save(const bool v, Arc& arc)
{
    if(v)
    {
        return save(tag::true_tag, arc);
    }
    else
    {
        return save(tag::false_tag, arc);
    }
}

template<typename Arc>
bool load(bool& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

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
