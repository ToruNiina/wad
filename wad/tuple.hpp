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
save_tuple_impl(const Tuple&, Arc&, const SavePoint&)
{
    return true;
}
template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I < std::tuple_size<Tuple>::value), bool>::type
save_tuple_impl(const Tuple& v, Arc& arc, const SavePoint& savepoint)
{
    if(!save(std::get<I>(v), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return save_tuple_impl<I+1>(v, arc, savepoint);
}

template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I >= std::tuple_size<Tuple>::value), bool>::type
load_tuple_impl(Tuple&, Arc&, const SavePoint&)
{
    return true;
}
template<std::size_t I, typename Tuple, typename Arc, typename SavePoint>
typename std::enable_if<(I < std::tuple_size<Tuple>::value), bool>::type
load_tuple_impl(Tuple& v, Arc& arc, const SavePoint& savepoint)
{
    if(!load(std::get<I>(v), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return load_tuple_impl<I+1>(v, arc, savepoint);
}

} // detail

template<typename Arc, typename ... Ts>
bool save(const std::tuple<Ts...>& v, Arc& arc)
{
    constexpr std::size_t size = sizeof...(Ts);
    const auto savepoint = arc.npos();

    if(size < 16)
    {
        const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) +
                               static_cast<std::uint8_t>(size);
        if(!save(static_cast<tag>(t), arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(size <= 0xFFFF)
    {
        if(!save(tag::array16, arc)) {return false;}

        const std::uint16_t sz = size;
        if(!to_big_endian(sz, arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(size <= 0xFFFFFFFF)
    {
        if(!save(tag::array32, arc)) {return false;}

        const std::uint32_t sz = size;
        if(!to_big_endian(sz, arc))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else
    {
        return false;
    }
    return detail::save_tuple_impl<0>(v, arc, savepoint);
}

template<typename Arc, typename ... Ts>
bool load(std::tuple<Ts...>& v, Arc& arc)
{
    constexpr std::size_t size = sizeof...(Ts);
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}

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
        if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        if(size != sz)
        {
            arc.seek(savepoint);
            return false;
        }
    }
    else if(t == tag::array32)
    {
        std::uint32_t sz = 0;
        if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
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
    return detail::load_tuple_impl<0>(v, arc, savepoint);
}

} // wad
#endif// WAD_TUPLE_HPP
