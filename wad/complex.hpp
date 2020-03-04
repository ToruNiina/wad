#ifndef WAD_COMPLEX_HPP
#define WAD_COMPLEX_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include "string.hpp"
#include <utility>
#include <complex>

namespace wad
{

template<typename T, typename Arc>
bool save(Arc& arc, const std::complex<T>& v)
{
    const auto savepoint = arc.npos();
    constexpr std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2u;

    if(!to_big_endian(arc, t) ||
       !save(arc, "real") || !save(arc, v.real()) ||
       !save(arc, "imag") || !save(arc, v.imag()))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename T, typename Arc>
bool load(Arc& arc, std::complex<T>& v)
{
    using value_type = typename std::complex<T>::value_type;
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load_length<type::map>(arc, len) || len != 2)
    {
        arc.seek(savepoint);
        return false;
    }

    value_type  val;
    std::string key;
    if(!load(arc, key) || key != "real" || !load(arc, val))
    {
        arc.seek(savepoint);
        return false;
    }
    v.real(val);
    if(!load(arc, key) || key != "imag" || !load(arc, val))
    {
        arc.seek(savepoint);
        return false;
    }
    v.imag(val);
    return true;
}

} // wad
#endif// WAD_COMPLEX_HPP
