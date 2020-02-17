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
bool save(const std::uint8_t& v, Arc& arc)
{
    if(v < 128)
    {
        // write as a positive fixint.
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(v, arc);
    }
    else // u8 integer.
    {
        if(!save(tag::uint8, arc)) {return false;}
        if(!to_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::uint16_t& v, Arc& arc)
{
    if(v < 128)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(v, arc);
    }
    else if (v <= std::uint16_t(std::numeric_limits<std::uint8_t>::max()))
    {
        if(!save(tag::uint8, arc))               {return false;}
        if(!to_big_endian(std::uint8_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::uint16, arc)) {return false;}
        if(!to_big_endian(v, arc))  {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::uint32_t& v, Arc& arc)
{
    if(v < 128)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(v, arc);
    }
    else if (v <= std::uint32_t(std::numeric_limits<std::uint8_t>::max()))
    {
        if(!save(tag::uint8, arc))               {return false;}
        if(!to_big_endian(std::uint8_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else if (v <= std::uint32_t(std::numeric_limits<std::uint16_t>::max()))
    {
        if(!save(tag::uint16, arc))               {return false;}
        if(!to_big_endian(std::uint16_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::uint32, arc)) {return false;}
        if(!to_big_endian(v, arc))  {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::uint64_t& v, Arc& arc)
{
    if(v < 128)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(v, arc);
    }
    else if (v <= std::uint64_t(std::numeric_limits<std::uint8_t>::max()))
    {
        if(!save(tag::uint8, arc))               {return false;}
        if(!to_big_endian(std::uint8_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else if (v <= std::uint64_t(std::numeric_limits<std::uint16_t>::max()))
    {
        if(!save(tag::uint16, arc))               {return false;}
        if(!to_big_endian(std::uint16_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else if (v <= std::uint64_t(std::numeric_limits<std::uint32_t>::max()))
    {
        if(!save(tag::uint32, arc))               {return false;}
        if(!to_big_endian(std::uint32_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::uint32, arc)) {return false;}
        if(!to_big_endian(v, arc))  {arc.retrace(1); return false;}
        return true;
    }
}

// ----------------------------------------------------------------------------
// load

template<typename Arc>
bool load(std::uint8_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::uint16_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(u8, arc)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::uint32_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(static_cast<std::uint8_t>(t) <=
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(u8, arc)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(u16, arc)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::uint64_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    if(static_cast<std::uint8_t>(t) <
       static_cast<std::uint8_t>(tag::positive_fixint_upper))
    {
        v = static_cast<std::uint8_t>(t);
        return true;
    }
    else if(t == tag::uint8)
    {
        std::uint8_t u8;
        if(!from_big_endian(u8, arc)) {arc.retrace(1); return false;}
        v = u8;
        return true;
    }
    else if(t == tag::uint16)
    {
        std::uint16_t u16;
        if(!from_big_endian(u16, arc)) {arc.retrace(1); return false;}
        v = u16;
        return true;
    }
    else if(t == tag::uint32)
    {
        std::uint32_t u32;
        if(!from_big_endian(u32, arc)) {arc.retrace(1); return false;}
        v = u32;
        return true;
    }
    else if(t == tag::uint64)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
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
bool save(const std::int8_t& v, Arc& arc)
{
    if(-32 <= v)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(v, arc);
    }
    else
    {
        if(!save(tag::int8, arc)) {return false;};
        if(!to_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::int16_t& v, Arc& arc)
{
    if(-32 <= v && v < 128)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(std::int8_t(v), arc);
    }
    else if (std::int16_t(std::numeric_limits<std::int8_t>::min) <= v &&
        v <= std::int16_t(std::numeric_limits<std::int8_t>::max))
    {
        if(!save(tag::int8, arc))               {return false;}
        if(!to_big_endian(std::int8_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::int16, arc)) {return false;}
        if(!to_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::int32_t& v, Arc& arc)
{
    if(-32 <= v && v < 128)
    {
        // we don't need to reorder a byte, but for consistency...
        return to_big_endian(std::int8_t(v), arc);
    }
    else if (std::int32_t(std::numeric_limits<std::int8_t>::min) <= v &&
        v <= std::int32_t(std::numeric_limits<std::int8_t>::max))
    {
        if(!save(tag::int8, arc))               {return false;}
        if(!to_big_endian(std::int8_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else if (std::int32_t(std::numeric_limits<std::int16_t>::min) <= v &&
        v <= std::int32_t(std::numeric_limits<std::int16_t>::max))
    {
        if(!save(tag::int16, arc))               {return false;}
        if(!to_big_endian(std::int16_t(v), arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::int32, arc)) {return false;}
        if(!to_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
}

template<typename Arc>
bool save(const std::int64_t& v, Arc& arc)
{
    if(-32 <= v && v < 128)
    {
        return to_big_endian(std::int8_t(v), arc);
    }
    else if (std::int64_t(std::numeric_limits<std::int8_t>::min) <= v &&
        v <= std::int64_t(std::numeric_limits<std::int8_t>::max))
    {
        if(!save(tag::int8, arc))              {return false;}
        if(!to_big_endian(std::int8_t(v), arc)){arc.retrace(1); return false;}
        return true;
    }
    else if (std::int64_t(std::numeric_limits<std::int16_t>::min) <= v &&
        v <= std::int64_t(std::numeric_limits<std::int16_t>::max))
    {
        if(!save(tag::int16, arc))              {return false;}
        if(!to_big_endian(std::int16_t(v), arc)){arc.retrace(1); return false;}
        return true;
    }
    else if (std::int64_t(std::numeric_limits<std::int32_t>::min) <= v &&
        v <= std::int64_t(std::numeric_limits<std::int32_t>::max))
    {
        if(!save(tag::int32, arc))              {return false;}
        if(!to_big_endian(std::int32_t(v), arc)){arc.retrace(1); return false;}
        return true;
    }
    else
    {
        if(!save(tag::int64, arc))              {return false;}
        if(!to_big_endian(std::int64_t(v), arc)){arc.retrace(1); return false;}
        return true;
    }
}

// ----------------------------------------------------------------------------
// load

template<typename Arc>
bool load(std::int8_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char>(std::addressof(v)));
        return true;
    }
    else if(t == tag::int8)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::int16_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(i8, arc)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::int32_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(i8, arc)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t i16;
        if(!from_big_endian(i16, arc)) {arc.retrace(1); return false;}
        v = i16;
        return true;
    }
    else if(t == tag::int32)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
        return true;
    }
    else
    {
        arc.retrace(1);
        return false;
    }
}

template<typename Arc>
bool load(std::int64_t& v, Arc& arc)
{
    tag t;
    if(!load(t, arc)) {return false;}

    const auto byte = static_cast<std::uint8_t>(t);
    if(byte <= static_cast<std::uint8_t>(tag::positive_fixint_upper) ||
               static_cast<std::uint8_t>(tag::negative_fixint_upper) <= byte)
    {
        // cstdints are guaranteed to be represented as 2's complement.
        std::int8_t i8;
        std::copy_n(reinterpret_cast<const char*>(std::addressof(byte)), 1,
                    reinterpret_cast<char>(std::addressof(i8)));
        v = i8;
        return true;
    }
    else if(t == tag::int8)
    {
        std::int8_t i8;
        if(!from_big_endian(i8, arc)) {arc.retrace(1); return false;}
        v = i8;
        return true;
    }
    else if(t == tag::int16)
    {
        std::int16_t i16;
        if(!from_big_endian(i16, arc)) {arc.retrace(1); return false;}
        v = i16;
        return true;
    }
    else if(t == tag::int32)
    {
        std::int32_t i32;
        if(!from_big_endian(i32, arc)) {arc.retrace(1); return false;}
        v = i32;
        return true;
    }
    else if(t == tag::int64)
    {
        if(!from_big_endian(v, arc)) {arc.retrace(1); return false;}
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
