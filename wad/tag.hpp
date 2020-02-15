#ifndef WAD_TAG_HPP
#define WAD_TAG_HPP
#include <cstdint>

namespace wad
{

enum class tag: std::uint8_t
{
    positive_fixint_lower = 0x00, // =   0
    positive_fixint_upper = 0x7f, // = 127
    fixmap_lower          = 0x80, // =  0 (elems)
    fixmap_upper          = 0x8f, // = 15
    fixarray_lower        = 0x90, // =  0
    fixarray_upper        = 0x9f, // = 15
    fixstr_lower          = 0xa0, // =  0
    fixstr_upper          = 0xbf, // = 31
    nil                   = 0xc0,
    unused                = 0xc1,
    false_tag             = 0xc2,
    true_tag              = 0xc3,
    bin8                  = 0xc4,
    bin16                 = 0xc5,
    bin32                 = 0xc6,
    ext8                  = 0xc7,
    ext16                 = 0xc8,
    ext32                 = 0xc9,
    float32               = 0xca,
    float64               = 0xcb,
    uint8                 = 0xcc,
    uint16                = 0xcd,
    uint32                = 0xce,
    uint64                = 0xcf,
    int8                  = 0xd0,
    int16                 = 0xd1,
    int32                 = 0xd2,
    int64                 = 0xd3,
    fixext1               = 0xd4,
    fixext2               = 0xd5,
    fixext4               = 0xd6,
    fixext8               = 0xd7,
    fixext16              = 0xd8,
    str8                  = 0xd9,
    str16                 = 0xda,
    str32                 = 0xdb,
    array16               = 0xdc,
    array32               = 0xdd,
    map16                 = 0xde,
    map32                 = 0xdf,
    negative_fixint_lower = 0xe0, // = -32
    negative_fixint_upper = 0xff  // = -1
};

template<typename Arc>
bool save(const tag t, Arc& arc)
{
    if(!arc.is_writable(1)) {return false;}

    *arc.sink() = static_cast<std::uint8_t>(t);
    arc.advance(1);
    return true;
}

// Note for professional C++ programmer:
// It loads a value of `tag` type as a byte. Sometimes it contains a value that
// is not listed in the `tag`. For example, a positive fixint value is combined
// with a tag. A value like "4" is represented as "0x04" and will be loaded as a
// tag.
// Actually, it is okay to load such a value as an enum. What is not allowed is
// loading a value that is out-of-range of the enum. Consider we have the
// following code. The first cast is okay. But the second one is not okay.
// ```cpp
// enum class color {red = 1, green = 2, blue = 4};
// color c = static_cast<color>(7); // It's okay
// color c = static_cast<color>(8); // Undefined (after c++17) or Unspecified
// ```
// Since tag::positive_fixint_lower == 0 and tag::negative_fixint_upper = 0xff,
// it covers all the values that can be represented by std::uint8_t. So any
// std::uint8_t value can be converted into `tag`.
template<typename Arc>
bool load(tag& t, Arc& arc)
{
    if(!arc.is_readable(1)) {return false;}

    const std::uint8_t byte = *arc.src();
    t = static_cast<tag>(byte);
    arc.advance(1);
    return true;
}

} // wad
#endif//WAD_TAG_HPP
