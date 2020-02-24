#ifndef WAD_TYPE_HPP
#define WAD_TYPE_HPP
#include "tag.hpp"

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

namespace detail
{
template<type T> struct save_load_type_tag_impl;
} // detail

// This generates appropreate type tag and append length. These function only
// supports the following type.
// - str
// - bin
// - array
// - map
// - ext

template<type T, typename Arc>
bool save_length(Arc& arc, const std::size_t len)
{
    return detail::save_load_type_tag_impl<T>::save_impl(arc, len);
}

template<type T, typename Arc>
bool load_length(Arc& arc, std::size_t& len)
{
    return detail::save_load_type_tag_impl<T>::load_impl(arc, len);
}

namespace detail
{
template<>
struct save_load_type_tag_impl<type::str>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const std::size_t len)
    {
        const auto savepoint = arc.npos();
        if(len < 32)
        {
            const std::uint8_t t = static_cast<std::uint8_t>(tag::fixstr_lower) +
                                   static_cast<std::uint8_t>(len);
            if(!save(arc, static_cast<tag>(t))) {return false;}
        }
        else if(len <= 0xFF)
        {
            if(!save(arc, tag::str8)) {return false;}
            const std::uint8_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFF)
        {
            if(!save(arc, tag::str16)) {return false;}
            const std::uint16_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(arc, tag::str32)) {return false;}
            const std::uint32_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else
        {
            return false;
        }
        return true;
    }

    template<typename Arc>
    static bool load_impl(Arc& arc, std::size_t& len)
    {
        const auto savepoint = arc.npos();
        tag t;
        if(!load(arc, t)) {return false;}

        const auto byte = static_cast<std::uint8_t>(t);
        if(        static_cast<std::uint8_t>(tag::fixstr_lower) <= byte &&
           byte <= static_cast<std::uint8_t>(tag::fixstr_upper))
        {
            len = byte - static_cast<std::uint8_t>(tag::fixstr_lower);
        }
        else if(t == tag::str8)
        {
            std::uint8_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::str16)
        {
            std::uint16_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::str32)
        {
            std::uint32_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }
};

template<>
struct save_load_type_tag_impl<type::bin>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const std::size_t len)
    {
        const auto savepoint = arc.npos();
        if(len <= 0xFF)
        {
            if(!save(arc, tag::bin8)) {return false;}
            const std::uint8_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFF)
        {
            if(!save(arc, tag::bin16)) {return false;}
            const std::uint16_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(arc, tag::bin32)) {return false;}
            const std::uint32_t sz = len;
            if(!to_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
        }
        else
        {
            return false;
        }
        return true;
    }

    template<typename Arc>
    static bool load_impl(Arc& arc, std::size_t& len)
    {
        const auto savepoint = arc.npos();
        tag t;
        if(!load(arc, t)) {return false;}

        if(t == tag::bin8)
        {
            std::uint8_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::bin16)
        {
            std::uint16_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::bin32)
        {
            std::uint32_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }

};

template<>
struct save_load_type_tag_impl<type::array>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const std::size_t len)
    {
        const auto savepoint = arc.npos();
        if(len < 16)
        {
            const std::uint8_t t = static_cast<std::uint8_t>(tag::fixarray_lower) +
                                   static_cast<std::uint8_t>(len);
            if(!save(arc, static_cast<tag>(t)))
            {
                arc.seek(savepoint);
                return false;
            }
        }
        else if(len <= 0xFFFF)
        {
            if(!save(arc, tag::array16)) {return false;}

            const std::uint16_t sz = len;
            if(!to_big_endian(arc, sz))
            {
                arc.seek(savepoint);
                return false;
            }
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(arc, tag::array32)) {return false;}

            const std::uint32_t sz = len;
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
        return true;
    }

    template<typename Arc>
    static bool load_impl(Arc& arc, std::size_t& len)
    {
        const auto savepoint = arc.npos();

        tag t;
        if(!load(arc, t)) {return false;}

        const auto byte = static_cast<std::uint8_t>(t);
        if(        static_cast<std::uint8_t>(tag::fixarray_lower) <= byte &&
           byte <= static_cast<std::uint8_t>(tag::fixarray_upper))
        {
            len = byte - static_cast<std::uint8_t>(tag::fixarray_lower);
        }
        else if(t == tag::array16)
        {
            std::uint16_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::array32)
        {
            std::uint32_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }
};

template<>
struct save_load_type_tag_impl<type::map>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const std::size_t len)
    {
        const auto savepoint = arc.npos();

        if(len < 16)
        {
            const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) +
                                   static_cast<std::uint8_t>(len);
            if(!save(arc, static_cast<tag>(t)))
            {
                arc.seek(savepoint);
                return false;
            }
        }
        else if(len <= 0xFFFF)
        {
            if(!save(arc, tag::map16)) {return false;}

            const std::uint16_t sz = len;
            if(!to_big_endian(arc, sz))
            {
                arc.seek(savepoint);
                return false;
            }
        }
        else if(len <= 0xFFFFFFFF)
        {
            if(!save(arc, tag::map32)) {return false;}

            const std::uint32_t sz = len;
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
        return true;
    }

    template<typename Arc>
    static bool load_impl(Arc& arc, std::size_t& len)
    {
        const auto savepoint = arc.npos();

        tag t;
        if(!load(arc, t)) {return false;}

        const auto byte = static_cast<std::uint8_t>(t);
        if(        static_cast<std::uint8_t>(tag::fixmap_lower) <= byte &&
           byte <= static_cast<std::uint8_t>(tag::fixmap_upper))
        {
            len = byte - static_cast<std::uint8_t>(tag::fixmap_lower);
        }
        else if(t == tag::map16)
        {
            std::uint16_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else if(t == tag::map32)
        {
            std::uint32_t sz = 0;
            if(!from_big_endian(arc, sz)) {arc.seek(savepoint); return false;}
            len = sz;
        }
        else
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }
};

template<>
struct save_load_type_tag_impl<type::ext>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const std::size_t len)
    {
        const auto savepoint = arc.npos();
        if(len == 1)
        {
            if(!save(arc, tag::fixext1))   {                     return false;}
        }
        else if(len == 2)
        {
            if(!save(arc, tag::fixext2))   {                     return false;}
        }
        else if(len == 4)
        {
            if(!save(arc, tag::fixext4))   {                     return false;}
        }
        else if(len == 8)
        {
            if(!save(arc, tag::fixext8))   {                     return false;}
        }
        else if(len == 16)
        {
            if(!save(arc, tag::fixext16))  {                     return false;}
        }
        else if(len <= 0xFF)
        {
            const std::uint8_t l = len;
            if(!save(arc, tag::ext8))  {                     return false;}
            if(!to_big_endian(arc, l)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFF)
        {
            const std::uint16_t l = len;
            if(!save(arc, tag::ext16)) {                     return false;}
            if(!to_big_endian(arc, l)) {arc.seek(savepoint); return false;}
        }
        else if(len <= 0xFFFFFFFF)
        {
            const std::uint32_t l = len;
            if(!save(arc, tag::ext32)) {                     return false;}
            if(!to_big_endian(arc, l)) {arc.seek(savepoint); return false;}
        }
        return true;
    }

    template<typename Arc>
    static bool load_impl(Arc& arc, std::size_t& len)
    {
        const auto savepoint = arc.npos();

        tag t;
        if(!load(arc, t)) {return false;}

        switch(t)
        {
            case tag::fixext1:  {len =  1; return true;}
            case tag::fixext2:  {len =  2; return true;}
            case tag::fixext4:  {len =  4; return true;}
            case tag::fixext8:  {len =  8; return true;}
            case tag::fixext16: {len = 16; return true;}
            case tag::ext8:
            {
                std::uint8_t l;
                if(!from_big_endian(l, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len = l;
                return true;
            }
            case tag::ext16:
            {
                std::uint16_t l;
                if(!from_big_endian(l, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len = l;
                return true;
            }
            case tag::ext32:
            {
                std::uint32_t l;
                if(!from_big_endian(l, arc))
                {
                    arc.seek(savepoint);
                    return false;
                }
                len = l;
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
