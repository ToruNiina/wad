#ifndef WAD_QUEUE_HPP
#define WAD_QUEUE_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <queue>

namespace wad
{

template<typename T, typename Container, typename Arc>
bool save(Arc& arc, const std::queue<T, Container>& v)
{
    const auto savepoint = arc.npos();

    if(!save_length<type::array>(arc, v.size()))
    {
        arc.seek(savepoint);
        return false;
    }

    auto v_ = v;

    while(!v_.empty())
    {
        const auto elem = v_.front();
        v_.pop();
        if(!save(arc, elem))
        {
            arc.seek(savepoint);
            return false;
        }
    }
    return true;
}

template<typename T, typename Container, typename Arc>
bool load(Arc& arc, std::queue<T, Container>& v)
{
    static_assert(std::is_default_constructible<T>::value,
                  "To load a type T, T must be default constructible.");
    const auto savepoint = arc.npos();

    std::size_t len;
    if(!load_length<type::array>(arc, len))
    {
        arc.seek(savepoint);
        return false;
    }
    for(std::size_t i=0; i<len; ++i)
    {
        T elem;
        if(!load(arc, elem))
        {
            arc.seek(savepoint);
            return false;
        }
        v.push(elem);
    }
    return true;
}

} // wad
#endif// WAD_QUEUE_HPP
