#ifndef WAD_BITSET_HPP
#define WAD_BITSET_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include "string.hpp"
#include <utility>
#include <bitset>
#include <sstream>

namespace wad
{

template<std::size_t N, typename Arc>
bool save(Arc& arc, const std::bitset<N>& v)
{
    const auto savepoint = arc.npos();

    std::ostringstream oss;
    oss << v;
    const std::string str = oss.str();
    if(!save(arc, str))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<std::size_t N, typename Arc>
bool load(Arc& arc, std::bitset<N>& v)
{
    const auto savepoint = arc.npos();

    std::string str;
    if(!load(arc, str))
    {
        arc.seek(savepoint);
        return false;
    }
    std::istringstream iss(std::move(str));
    iss >> v;
    return true;
}

} // wad
#endif// WAD_BITSET_HPP
