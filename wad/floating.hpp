#ifndef WAD_FLOATING_HPP
#define WAD_FLOATING_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <algorithm>
#include <utility>

namespace wad
{

template<typename Arc>
bool save(const float& v, Arc& arc)
{
    if(!save(tag::float32, arc)) {return false;}
    if(!to_big_endian(v, arc))   {arc.retrace(1); return false;}
    return true;
}

template<typename Arc>
bool save(const double& v, Arc& arc)
{
    if(!save(tag::float64, arc)) {return false;}
    if(!to_big_endian(v, arc))   {arc.retrace(1); return false;}
    return true;
}


template<typename Arc>
bool load(float& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(t != tag::float32 || !from_big_endian(v, arc))
    {
        arc.retrace(1);
        return false;
    }
    return true;
}

template<typename Arc>
bool load(double& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(t != tag::float64 || !from_big_endian(v, arc))
    {
        arc.retrace(1);
        return false;
    }
    return true;
}


} // wad
#endif// WAD_FLOATING_HPP
