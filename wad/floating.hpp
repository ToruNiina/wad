#ifndef WAD_FLOATING_HPP
#define WAD_FLOATING_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <algorithm>
#include <utility>

namespace wad
{

template<typename Arc>
bool save(Arc& arc, const float& v)
{
    if(!save(arc, tag::float32)) {return false;}
    if(!to_big_endian(arc, v))   {arc.retrace(1); return false;}
    return true;
}

template<typename Arc>
bool save(Arc& arc, const double& v)
{
    if(!save(arc, tag::float64)) {return false;}
    if(!to_big_endian(arc, v))   {arc.retrace(1); return false;}
    return true;
}


template<typename Arc>
bool load(Arc& arc, float& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(t != tag::float32 || !from_big_endian(arc, v))
    {
        arc.retrace(1);
        return false;
    }
    return true;
}

template<typename Arc>
bool load(Arc& arc, double& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(t != tag::float64 || !from_big_endian(arc, v))
    {
        arc.retrace(1);
        return false;
    }
    return true;
}


} // wad
#endif// WAD_FLOATING_HPP
