#ifndef WAD_UNIQUE_PTR_HPP
#define WAD_UNIQUE_PTR_HPP
#include "inheritance.hpp"
#include <memory>

namespace wad
{

template<typename Base, typename Deleter, typename Arc>
typename std::enable_if<std::is_polymorphic<Base>::value, bool>::type
save(Arc& arc, const std::unique_ptr<Base, Deleter>& v)
{
    const auto savepoint = arc.npos();

    for(const auto& kv: detail::subclass_saver_database<Base, Arc>::database())
    {
        if(kv.second(arc, v.get()))
        {
            return true;
        }
    }
    arc.seek(savepoint);
    return false; // no matching.
}

template<typename Base, typename Deleter, typename Arc>
typename std::enable_if<std::is_polymorphic<Base>::value, bool>::type
load(Arc& arc, std::unique_ptr<Base, Deleter>& v)
{
    const auto savepoint = arc.npos();

    for(const auto& kv: detail::subclass_loader_database<Base, Arc>::database())
    {
        Base* ptr = nullptr;
        if(kv.second(arc, ptr))
        {
            v.reset(ptr);
            return true;
        }
    }
    arc.seek(savepoint);
    return false; // no matching.
}

template<typename T, typename Deleter, typename Arc>
typename std::enable_if<!std::is_polymorphic<T>::value, bool>::type
save(Arc& arc, const std::unique_ptr<T, Deleter>& v)
{
    return save(arc, *v);
}

template<typename T, typename Deleter, typename Arc>
typename std::enable_if<!std::is_polymorphic<T>::value, bool>::type
load(Arc& arc, std::unique_ptr<T, Deleter>& v)
{
    if(!v)
    {
        v.reset(new T{});
    }
    return load(arc, *v);
}

} // wad
#endif// WAD_UNIQUE_PTR_HPP
