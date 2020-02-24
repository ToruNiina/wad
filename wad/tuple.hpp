#ifndef WAD_TUPLE_HPP
#define WAD_TUPLE_HPP
#include "binary_cast.hpp"
#include "tuple.hpp"
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

    if(!save<type::array>(arc, size))
    {
        arc.seek(savepoint);
        return false;
    }
    return detail::save_tuple_impl<0>(arc, v, savepoint);
}

template<typename Arc, typename ... Ts>
bool load(Arc& arc, std::tuple<Ts...>& v)
{
    constexpr std::size_t size = sizeof...(Ts);
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load<type::array>(arc, len) || len != size)
    {
        arc.seek(savepoint);
        return false;
    }
    return detail::load_tuple_impl<0>(arc, v, savepoint);
}

} // wad
#endif// WAD_TUPLE_HPP
