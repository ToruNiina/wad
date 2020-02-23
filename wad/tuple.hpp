#ifndef WAD_TUPLE_HPP
#define WAD_TUPLE_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <utility>
#include <tuple>

namespace wad
{
namespace detail
{

template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I >= std::tuple_size<Tuple>::value), bool>::type
save_tuple_impl(Arc&, const Tuple&, const SavePoint&)
{
    return true;
}
template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I < std::tuple_size<Tuple>::value), bool>::type
save_tuple_impl(Arc& arc, const Tuple& v, const SavePoint& savepoint)
{
    if(!save(arc, std::get<I>(v)))
    {
        arc.seek(savepoint);
        return false;
    }
    return save_tuple_impl<I+1>(arc, v, savepoint);
}

template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I >= std::tuple_size<Tuple>::value), bool>::type
load_tuple_impl(Arc&, Tuple&, const SavePoint&)
{
    return true;
}
template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I < std::tuple_size<Tuple>::value), bool>::type
load_tuple_impl(Arc& arc, Tuple& v, const SavePoint& savepoint)
{
    if(!load(arc, std::get<I>(v)))
    {
        arc.seek(savepoint);
        return false;
    }
    return load_tuple_impl<I+1>(arc, v, savepoint);
}

} // detail

template<typename Arc, typename ... Ts>
bool save(Arc& arc, const std::tuple<Ts...>& v)
{
    constexpr std::size_t size = sizeof...(Ts);
    const auto savepoint = arc.npos();

    if(size < 16)
    {
        const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) +
                               static_cast<std::uint8_t>(size);
        if(!save(arc, static_cast<tag>(t)))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(size <= 0xFFFF)
    {
        if(!save(arc, tag::array16)) {return false;}

        const std::uint16_t sz = size;
        if(!to_big_endian(arc, sz))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(size <= 0xFFFFFFFF)
    {
        if(!save(arc, tag::array32)) {return false;}

        const std::uint32_t sz = size;
        if(!to_big_endian(arc, sz))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else
    {
        return false;
    }
    return detail::save_tuple_impl<0>(arc, v, savepoint);
}

template<typename Arc, typename ... Ts>
bool load(Arc& arc, std::tuple<Ts...>& v)
{
    constexpr std::size_t size = sizeof...(Ts);
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(        static_cast<std::uint8_t>(tag::fixarray_lower) <= byte &&
       byte <= static_cast<std::uint8_t>(tag::fixarray_upper))
    {
        if(byte - static_cast<std::uint8_t>(tag::fixarray_lower) != size)
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(t == tag::array16)
    {
        std::uint16_t sz = 0;
        if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        if(size != sz)
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(t == tag::array32)
    {
        std::uint32_t sz = 0;
        if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        if(size != sz)
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else
    {
        arc.seek(savepoint);
        return false;
    }
    return detail::load_tuple_impl<0>(arc, v, savepoint);
}

} // wad
#endif// WAD_TUPLE_HPP
