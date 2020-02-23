#ifndef WAD_INTEGER_HPP
#define WAD_INTEGER_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include <algorithm>
#include <utility>
#include <limits>

namespace wad
{

// ============================================================================
// unsigned integers

// ----------------------------------------------------------------------------
// save

template<typename Arc>
bool save(Arc& arc, const std::uint8_t& v)
{
    if(v < 128)
    {
        // write as a positive fixint.
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(arc, v);
    }
    else // u8 integer.
    {
        if(!save(arc, tag::uint8)) {return false;}
        if(!to_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::uint16_t& v)
{
    if (v <= std::uint16_t(std::numeric_limits<std::uint8_t>::max()))
    {
        return save(arc, std::uint8_t(v));
    }
    else
    {
        if(!save(arc, tag::uint16)) {return false;}
        if(!to_big_endian(arc, v))  {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::uint32_t& v)
{
    if (v <= std::uint32_t(std::numeric_limits<std::uint16_t>::max()))
    {
        return save(arc, std::uint16_t(v));
    }
    else
    {
        if(!save(arc, tag::uint32)) {return false;}
        if(!to_big_endian(arc, v))  {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::uint64_t& v)
{
    if (v <= std::uint64_t(std::numeric_limits<std::uint32_t>::max()))
    {
        return save(arc, std::uint32_t(v));
    }
    else
    {
        if(!save(arc, tag::uint64)) {return false;}
        if(!to_big_endian(arc, v))  {arc.retrace(1); return false;}
        return true;
    }
}

// ----------------------------------------------------------------------------
// load

template<typename Arc>
bool load(Arc& arc, std::uint8_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(2); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::uint16_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(2); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(3); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::uint32_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(static_cast<std::uint8_t>(t) <=
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(arc, u16)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(2); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(3); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int32)
    {
        std::int32_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(5); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::uint64_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(arc, u16)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        std::uint32_t u32;
        if(!from_big_endian(arc, u32)) {arc.retrace(1); return false;}
        v = u32;
        return true;
    }
    else if(t == tag::uint64)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(2); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(3); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int32)
    {
        std::int32_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(5); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else if(t == tag::int64)
    {
        std::int32_t buf;
        if(!from_big_endian(arc, buf)) {arc.retrace(1); return false;}
        if(buf < 0)                    {arc.retrace(9); return false;}
        v = buf; // if it's positive, then it's okay.
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

// ============================================================================
// signed integers

// ----------------------------------------------------------------------------
// save

template<typename Arc>
bool save(Arc& arc, const std::int8_t& v)
{
    if(-32 <= v)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(arc, v);
    }
    else
    {
        if(!save(arc, tag::int8))  {return false;}
        if(!to_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::int16_t& v)
{
    if(0 <= v)
    {
        return save(arc, std::uint16_t(v));
    }
    else if (std::int16_t(std::numeric_limits<std::int8_t>::min()) <= v)
    {
        return save(arc, std::int8_t(v));
    }
    else
    {
        if(!save(arc, tag::int16)) {return false;}
        if(!to_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::int32_t& v)
{
    if(0 <= v)
    {
        return save(arc, std::uint32_t(v));
    }
    else if (std::int32_t(std::numeric_limits<std::int16_t>::min()) <= v)
    {
        return save(arc, std::int16_t(v));
    }
    else
    {
        if(!save(arc, tag::int32)) {return false;}
        if(!to_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(Arc& arc, const std::int64_t& v)
{
    if(0 <= v)
    {
        return save(arc, std::uint64_t(v));
    }
    else if (std::int64_t(std::numeric_limits<std::int32_t>::min()) <= v)
    {
        return save(arc, std::int32_t(v));
    }
    else
    {
        if(!save(arc, tag::int64))              {return false;}
        if(!to_big_endian(arc, std::int64_t(v))){arc.retrace(1); return false;}
        return true;
    }
}

// ----------------------------------------------------------------------------
// load

template<typename Arc>
bool load(Arc& arc, std::int8_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_lower) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char*>(std::addressof(v)));
        return true;
    }
    else if(t == tag::int8)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        if(127u < u8)                 {arc.retrace(2); return false;}
        v = u8;
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::int16_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char*>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(arc, i8)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(arc, u16)) {arc.retrace(1); return false;}
        if(std::numeric_limits<std::int16_t>::max() < u16)
        {
            arc.retrace(2);
            return false;
        }
        v = u16;
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::int32_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char*>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(arc, i8)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t i16;
        if(!from_big_endian(arc, i16)) {arc.retrace(1); return false;}
        v = i16;
        return true;
    }
    else if(t == tag::int32)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(arc, u16)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        std::uint32_t u32;
        if(!from_big_endian(arc, u32)) {arc.retrace(1); return false;}
        if(std::numeric_limits<std::int32_t>::max() < u32)
        {
            arc.retrace(5);
            return false;
        }
        v = u32;
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(Arc& arc, std::int64_t& v)
{
    tag t;
    if(!load(arc, t)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char*>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(arc, i8)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t i16;
        if(!from_big_endian(arc, i16)) {arc.retrace(1); return false;}
        v = i16;
        return true;
    }
    else if(t == tag::int32)
    {
        std::int32_t i32;
        if(!from_big_endian(arc, i32)) {arc.retrace(1); return false;}
        v = i32;
        return true;
    }
    else if(t == tag::int64)
    {
        if(!from_big_endian(arc, v)) {arc.retrace(1); return false;}
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(arc, u8)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(arc, u16)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        std::uint32_t u32;
        if(!from_big_endian(arc, u32)) {arc.retrace(1); return false;}
        v = u32;
        return true;
    }
    else if(t == tag::uint64)
    {
        std::uint64_t u64;
        if(!from_big_endian(arc, u64)) {arc.retrace(1); return false;}
        if(std::numeric_limits<std::int64_t>::max() < u64)
        {
            arc.retrace(9);
            return false;
        }
        v = u64;
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

} // wad
#endif//WAD_INTEGER_HPP
