#ifndef WAD_TYPE_HPP
#define WAD_TYPE_HPP
#include "tag.hpp"
#include "boolean.hpp"
#include "integer.hpp"
#include "floating.hpp"
#include "string.hpp"
#include "tuple.hpp"
#include <cstring>

namespace wad
{
enum class type: std::uint8_t
{
    nil,
    boolean,
    integer,
    floating,
    str,
    bin,
    array,
    map,
    ext
};

template<typename T> struct is_character : std::false_type {};
template<> struct is_character<char>     : std::true_type {};
template<> struct is_character<wchar_t>  : std::true_type {};
template<> struct is_character<char16_t> : std::true_type {};
template<> struct is_character<char32_t> : std::true_type {};

// The main purpose of this feature is to provide the more convenient way to
// write types that has dynamic size, such as array and map.
//
// Writing correct type tag is a hard task.
// To make it easier, this function is provided. E.g.
// ```cpp
// struct X
// {
//     int a;
//     double b;
//     std::string c;
//
//     template<typename Arc>
//     bool save(Arc& arc) const
//     {
//         // here it counts number of arguments and add appropreate tag
//         return save<wad::type::map>("a", a, "b", b, "c", c, arc);
//     }
// };
// ```

namespace detail
{
// forward declarations
template<type T> struct save_type_impl;
template<type T> struct load_type_impl;
} // detail

template<type T, typename ... Args>
bool save(Args&& ... args)
{
    return detail::save_type_impl<T>::invoke(std::forward<Args>(args) ...);
}

template<type T, typename ... Args>
bool load(Args&& ... args)
{
    return detail::load_type_impl<T>::invoke(std::forward<Args>(args) ...);
}

namespace detail
{

/*                         _                 _   */
/*   ___  __ ___   _____  (_)_ __ ___  _ __ | |  */
/*  / __|/ _` \ \ / / _ \ | | '_ ` _ \| '_ \| |  */
/*  \__ \ (_| |\ V /  __/ | | | | | | | |_) | |  */
/*  |___/\__,_| \_/ \___| |_|_| |_| |_| .__/|_|  */
/*                                    |_|        */

template<>
struct save_type_impl<type::nil>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        return save(tag::nil, arc);
    }
};
template<>
struct save_type_impl<type::boolean>
{
    template<typename Arc>
    static bool invoke(const bool v, Arc& arc) noexcept
    {
        return save(v, arc);
    }
};
template<>
struct save_type_impl<type::integer>
{
    template<typename T, typename Arc>
    static bool invoke(const T v, Arc& arc) noexcept
    {
        static_assert(std::is_integral<T>::value,
                      "type of an integer value should be an integer");
        return save(v, arc);
    }
};
template<>
struct save_type_impl<type::floating>
{
    template<typename T, typename Arc>
    static bool invoke(const T v, Arc& arc) noexcept
    {
        static_assert(std::is_floating_point<T>::value,
                      "type of an integer value should be an integer");
        return save(v, arc);
    }
};
template<>
struct save_type_impl<type::str>
{
    template<typename traits, typename Alloc, typename Arc>
    static bool invoke(const std::basic_string<char, traits, Alloc>& v,
                       Arc& arc) noexcept
    {
        return save(v, arc);
    }

    // for string literal
    template<typename Arc>
    static bool invoke(const char* v, Arc& arc) noexcept
    {
        return save(v, arc);
    }
};

template<>
struct save_type_impl<type::bin>
{
    // for std::basic_string<charT, traitsT, Alloc>
    template<typename charT, typename traitsT, typename Alloc, typename Arc>
    static bool invoke(const std::basic_string<charT, traitsT, Alloc>& v,
                       Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();
        const auto len = sizeof(charT) * v.size();
        if(len <= 0xFF)
        {
            if(!save(tag::bin8, arc)) {return false;}
            const std::uint8_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFF)
        {
            if(!save(tag::bin16, arc)) {return false;}
            const std::uint16_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(tag::bin32, arc)) {return false;}
            const std::uint32_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else
        {
            return false;
        }
        if(!arc.is_writable(len)) {arc.seek(savepoint); return false;}

        std::copy_n(reinterpret_cast<const char*>(v.data()), len, arc.sink());
        arc.advance(len);
        return true;
    }

    // for char* and their variants
    template<typename C, typename Arc>
    static typename std::enable_if<is_character<C>::value, bool>::type
    invoke(const C* v, Arc& arc) noexcept
    {
        using traitsT = std::char_traits<C>;

        const auto savepoint = arc.npos();
        const auto len = sizeof(C) * traitsT::length(v);
        if(len <= 0xFF)
        {
            if(!save(tag::bin8, arc)) {return false;}
            const std::uint8_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFF)
        {
            if(!save(tag::bin16, arc)) {return false;}
            const std::uint16_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(tag::bin32, arc)) {return false;}
            const std::uint32_t sz = len;
            if(!to_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
        }
        else
        {
            return false;
        }
        if(!arc.is_writable(len)) {arc.seek(savepoint); return false;}

        std::copy_n(reinterpret_cast<const char*>(v), len, arc.sink());
        arc.advance(len);
        return true;
    }
};

template<>
struct save_type_impl<type::array>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        return save(tag::fixarray_lower /*len = 0*/, arc);
    }

    template<typename T, typename T2, typename ... Ts>
    static bool invoke(const T& head, T2&& head2, Ts&& ... tail) noexcept
    {
        return invoke_impl(std::tie(head), std::forward<T2>(head2),
                           std::forward<Ts>(tail) ...);
    }

    template<typename ... Ts, typename Head, typename Head2, typename ...Tail>
    static bool invoke_impl(std::tuple<const Ts& ...> refs, const Head& head,
                            Head2&& head2, Tail&& ... tail) noexcept
    {
        return invoke_impl(std::tuple_cat(refs, std::tie(head)),
                std::forward<Head2>(head2), std::forward<Tail>(tail)...);
    }

    template<typename ... Ts, typename Arc>
    static bool invoke_impl(std::tuple<const Ts&...> refs, Arc& arc)
    {
        return save(refs, arc);
    }
};

template<>
struct save_type_impl<type::map>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        return save(tag::fixmap_lower /*len = 0*/, arc);
    }

    template<typename K, typename V, typename T, typename ... Ts>
    static bool invoke(const K& key, const V& val,
                       T&& head, Ts&& ... tail) noexcept
    {
        static_assert(sizeof...(tail) % 2 == 0);
        return invoke_impl(std::tie(key, val), std::forward<T>(head),
                           std::forward<Ts>(tail) ...);
    }

    template<typename ... Ts, typename K, typename V,
             typename Head, typename ...Tail>
    static bool invoke_impl(std::tuple<const Ts& ...> refs,
         const K& key, const V& val, Head&& head, Tail&& ... tail) noexcept
    {
        return invoke_impl(std::tuple_cat(refs, std::tie(key, val)),
                std::forward<Head>(head), std::forward<Tail>(tail)...);
    }

    template<typename ... Ts, typename Arc>
    static bool invoke_impl(std::tuple<const Ts&...> refs, Arc& arc)
    {
        constexpr std::size_t size = sizeof...(Ts) / 2;
        const auto savepoint = arc.npos();

        if(size < 16)
        {
            const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) +
                                   static_cast<std::uint8_t>(size);
            if(!save(static_cast<tag>(t), arc))
            {
                return false;
            }
        }
        else if(size <= 0xFFFF)
        {
            if(!save(tag::map16, arc)) {return false;}

            const std::uint16_t sz = size;
            if(!to_big_endian(sz, arc))
            {
                arc.seek(savepoint);
                return false;
            }
        }
        else if(size <= 0xFFFFFFFF)
        {
            if(!save(tag::map32, arc)) {return false;}

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
        // defined in tuple.hpp
        return detail::save_tuple_impl<0>(refs, arc, savepoint);
    }
};

template<>
struct save_type_impl<type::ext>
{
    // It only writes tag and length. The content itself should be written
    // manually by the users.
    template<typename Arc>
    static bool invoke(const std::uint8_t exttag, const std::size_t len_bytes,
                       Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();
        if(len_bytes == 1)
        {
            if(!save(tag::fixext1, arc))   {                     return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes == 2)
        {
            if(!save(tag::fixext2, arc))   {                     return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes == 4)
        {
            if(!save(tag::fixext4, arc))   {                     return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes == 8)
        {
            if(!save(tag::fixext8, arc))   {                     return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes == 16)
        {
            if(!save(tag::fixext16, arc))  {                     return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes <= 0xFF)
        {
            const std::uint8_t len = len_bytes;
            if(!save(tag::ext8, arc))      {                     return false;}
            if(!to_big_endian(len, arc))   {arc.seek(savepoint); return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes <= 0xFFFF)
        {
            const std::uint16_t len = len_bytes;
            if(!save(tag::ext16, arc))     {                     return false;}
            if(!to_big_endian(len, arc))   {arc.seek(savepoint); return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        else if(len_bytes <= 0xFFFFFFFF)
        {
            const std::uint32_t len = len_bytes;
            if(!save(tag::ext32, arc))     {                     return false;}
            if(!to_big_endian(len, arc))   {arc.seek(savepoint); return false;}
            if(!to_big_endian(exttag, arc)){arc.seek(savepoint); return false;}
        }
        return true;
    }
};

/*   _                 _   _                 _   */
/*  | | ___   __ _  __| | (_)_ __ ___  _ __ | |  */
/*  | |/ _ \ / _` |/ _` | | | '_ ` _ \| '_ \| |  */
/*  | | (_) | (_| | (_| | | | | | | | | |_) | |  */
/*  |_|\___/ \__,_|\__,_| |_|_| |_| |_| .__/|_|  */
/*                                    |_|        */

template<>
struct load_type_impl<type::nil>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        tag t;
        if(!load(t, arc)) {return false;}
        if(t != tag::nil) {arc.retrace(1); return false;}
        return true;
    }
};
template<>
struct load_type_impl<type::boolean>
{
    template<typename Arc>
    static bool invoke(bool& v, Arc& arc) noexcept
    {
        return load(v, arc);
    }
};
template<>
struct load_type_impl<type::integer>
{
    template<typename T, typename Arc>
    static bool invoke(T& v, Arc& arc) noexcept
    {
        static_assert(std::is_integral<T>::value,
                      "type of an integer value should be an integer");
        return load(v, arc);
    }
};
template<>
struct load_type_impl<type::floating>
{
    template<typename T, typename Arc>
    static bool invoke(T& v, Arc& arc) noexcept
    {
        static_assert(std::is_floating_point<T>::value,
                      "type of an integer value should be an integer");
        return load(v, arc);
    }
};
template<>
struct load_type_impl<type::str>
{
    // for std::basic_string<char, traitsT, Alloc>
    template<typename traits, typename Alloc, typename Arc>
    static bool invoke(std::basic_string<char, traits, Alloc>& v, Arc& arc) noexcept
    {
        return load(v, arc);
    }

//     // string literal disabled
//     template<typename Arc>
//     static bool invoke(const char* v, Arc& arc) noexcept
//     {
//         return load(v, arc);
//     }
};

template<>
struct load_type_impl<type::bin>
{
    template<typename charT, typename traits, typename Alloc, typename Arc>
    static bool invoke(std::basic_string<charT, traits, Alloc>& v,
                       Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();

        tag t;
        if(!load(t, arc)) {return false;}

        std::size_t len = 0;
        if(t == tag::bin8)
        {
            std::uint8_t u8;
            if(!from_big_endian(u8, arc)){arc.seek(savepoint); return false;}
            len = u8;
        }
        else if(t == tag::bin16)
        {
            std::uint16_t u16;
            if(!from_big_endian(u16, arc)){arc.seek(savepoint); return false;}
            len = u16;
        }
        else if(t == tag::bin32)
        {
            std::uint32_t u32;
            if(!from_big_endian(u32, arc)){arc.seek(savepoint); return false;}
            len = u32;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }
        if(!arc.is_readable(len) || len % sizeof(charT) != 0)
        {
            arc.seek(savepoint);
            return false;
        }

        v.resize(len / sizeof(charT));
        for(auto& c : v)
        {
            std::copy_n(arc.src(), sizeof(charT),
                        reinterpret_cast<char*>(std::addressof(c)));
            arc.advance(sizeof(charT));
        }
        return true;
    }
};

template<>
struct load_type_impl<type::array>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        tag t;
        if(!load(t, arc))            {return false;}
        if(t != tag::fixarray_lower) {arc.retrace(1); return false;}
        return true;
    }

    template<typename T, typename T2, typename ... Ts>
    static bool invoke(T& head, T2& head2, Ts& ... tail) noexcept
    {
        return invoke_impl(std::tie(head), head2, tail...);
    }

    template<typename ... Ts, typename Head, typename Head2, typename ...Tail>
    static bool invoke_impl(std::tuple<Ts& ...> refs,
                            Head& head, Head2& head2, Tail& ... tail) noexcept
    {
        return invoke_impl(std::tuple_cat(refs, std::tie(head)), head2, tail...);
    }

    template<typename ... Ts, typename Arc>
    static bool invoke_impl(std::tuple<Ts&...> refs, Arc& arc)
    {
        return load(refs, arc);
    }
};

template<>
struct load_type_impl<type::map>
{
    template<std::size_t N, typename V, typename Arc, typename SavePoint>
    static bool load_kv_impl(std::tuple<const char(&)[N], V&> kv,
                             Arc& arc, const SavePoint& savepoint)
    {
        std::string key;
        if(!load(key, arc))        {arc.seek(savepoint); return false;}
        if(key != std::get<0>(kv)) {arc.seek(savepoint); return false;}
        return load(std::get<1>(kv), arc);
    }
    template<typename V, typename Arc, typename SavePoint>
    static bool load_kv_impl(std::tuple<const char*const&, V&> kv,
                             Arc& arc, const SavePoint& savepoint)
    {
        std::string key;
        if(!load(key, arc))        {arc.seek(savepoint); return false;}
        if(key != std::get<0>(kv)) {arc.seek(savepoint); return false;}
        return load(std::get<1>(kv), arc);
    }
    template<typename K, typename V, typename Arc, typename SavePoint>
    static typename std::enable_if<!std::is_same<K, const char*>::value, bool>::type
    load_kv_impl(std::tuple<K const&, V&> kv,
                 Arc& arc, const SavePoint& savepoint)
    {
        K key;
        if(!load(key, arc))        {arc.seek(savepoint); return false;}
        if(key != std::get<0>(kv)) {arc.seek(savepoint); return false;}
        return load(std::get<1>(kv), arc);
    }
    template<std::size_t I, typename ... Ts, typename Arc, typename SavePoint>
    static typename std::enable_if<(I >= sizeof...(Ts)), bool>::type
    load_kv_rec(std::tuple<Ts...>, Arc&, const SavePoint&)
    {
        return true;
    }
    template<std::size_t I, typename ... Ts, typename Arc, typename SavePoint>
    static typename std::enable_if<(I < sizeof...(Ts)), bool>::type
    load_kv_rec(std::tuple<Ts...> kvs, Arc& arc, const SavePoint& savepoint)
    {
        if(!load_kv_impl(std::get<I>(kvs), arc, savepoint)) {return false;}
        return load_kv_rec<I+1>(kvs, arc, savepoint);
    }

    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        tag t;
        if(!load(t, arc))          {return false;}
        if(t != tag::fixmap_lower) {arc.retrace(1); return false;}
        return true;
    }

    template<typename K, typename V, typename Head, typename ... Tail>
    static bool invoke(const K& key, V& val,
                       Head& head, Tail& ... tail) noexcept
    {
        static_assert(sizeof...(tail) % 2 == 0);
        return invoke_impl(std::make_tuple(std::tie(key, val)), head, tail...);
    }

    template<typename ... Ts, typename K, typename V,
             typename Head, typename ...Tail>
    static bool invoke_impl(std::tuple<Ts...> refs,
         const K& key, V& val, Head& head, Tail& ... tail) noexcept
    {
        return invoke_impl(
                std::tuple_cat(refs, std::make_tuple(std::tie(key, val))),
                head, tail...);
    }

    template<typename ... Ts, typename Arc>
    static bool invoke_impl(std::tuple<Ts...> refs, Arc& arc)
    {
        // It does not support reorder, currently...

        constexpr std::size_t size = sizeof...(Ts);
        const auto savepoint = arc.npos();

        tag t;
        if(!load(t, arc)) {return false;}

        std::size_t len = 0;
        const auto byte = static_cast<std::uint8_t>(t);
        if(static_cast<std::uint8_t>(tag::fixmap_lower) <= byte &&
           byte <= static_cast<std::uint8_t>(tag::fixmap_upper))
        {
            len = byte - static_cast<std::uint8_t>(tag::fixmap_lower);
        }
        else if(t == tag::map16)
        {
            std::uint16_t sz;
            if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::map32)
        {
            std::uint32_t sz;
            if(!from_big_endian(sz, arc)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }

        if(len != size)
        {
            arc.seek(savepoint);
            return false;
        }
        return load_kv_rec<0>(refs, arc, savepoint);
    }
};

template<>
struct load_type_impl<type::ext>
{
    // It only writes tag and length. The content itself should be written
    // manually by the users.
    template<typename Arc>
    static bool invoke(std::uint8_t& exttag, std::size_t& len_bytes,
                       Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();

        tag t;
        if(!load(t, arc)) {return false;}

        switch(t)
        {
            case tag::fixext1:
            {
                if(!from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = 1;
                return true;
            }
            case tag::fixext2:
            {
                if(!from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = 2;
                return true;
            }
            case tag::fixext4:
            {
                if(!from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = 4;
                return true;
            }
            case tag::fixext8:
            {
                if(!from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = 8;
                return true;
            }
            case tag::fixext16:
            {
                if(!from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = 16;
                return true;
            }
            case tag::ext8:
            {
                std::uint8_t len;
                if(!from_big_endian(len, arc) || !from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = len;
                return true;
            }
            case tag::ext16:
            {
                std::uint16_t len;
                if(!from_big_endian(len, arc) || !from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = len;
                return true;
            }
            case tag::ext32:
            {
                std::uint32_t len;
                if(!from_big_endian(len, arc) || !from_big_endian(exttag, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len_bytes = len;
                return true;
            }
            default:
            {
                arc.seek(savepoint);
                return false;
            }
        }
    }
};


} // detail
} // wad
#endif// WAD_TYPE_HPP
