#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/integer.hpp"
#include "utility.hpp"

namespace wad
{
template<typename U, typename T>
U save_load_convert(const T& t)
{
    test_write_archive warc;
    save(t, warc);

    U retval;
    test_read_archive rarc(warc);
    load(retval, rarc);

    return retval;
}
} // wad

TEST_CASE( "uint8_t save/load", "[u8]" ) {
    REQUIRE(wad::save_load(std::uint8_t(42))  ==  42u);
    REQUIRE(wad::save_load(std::uint8_t(142)) == 142u);

    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint8_t(42))  ==  42u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint8_t(142)) == 142u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint8_t(42))  ==  42u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint8_t(142)) == 142u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint8_t(42))  ==  42u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint8_t(142)) == 142u);

    REQUIRE(wad::save_load_convert<std::int8_t>(std::uint8_t(42))  ==  42);

    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint8_t(142)) == 142);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint8_t(142)) == 142);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint8_t(142)) == 142);
}

TEST_CASE( "uint16_t save/load", "[u16]" ) {
    REQUIRE(wad::save_load(std::uint16_t(    42)) ==  42u);
    REQUIRE(wad::save_load(std::uint16_t(   142)) == 142u);
    REQUIRE(wad::save_load(std::uint16_t(0xBEEF)) == 0xBEEFu);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint16_t(42))  ==  42u);
    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint16_t(142)) == 142u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint16_t(42))     ==  42u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint16_t(142))    == 142u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint16_t(0xBEEF)) == 0xBEEFu);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint16_t(42))     ==  42u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint16_t(142))    == 142u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint16_t(0xBEEF)) == 0xBEEFu);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::uint8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint16_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint16_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint16_t(0x0123)) == 0x0123);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint16_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint16_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint16_t(0xBEEF)) == 0xBEEF);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint16_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint16_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint16_t(0xBEEF)) == 0xBEEF);
}

TEST_CASE( "uint32_t save/load", "[u32]" ) {
    REQUIRE(wad::save_load(std::uint32_t(    42))     ==  42u);
    REQUIRE(wad::save_load(std::uint32_t(   142))     == 142u);
    REQUIRE(wad::save_load(std::uint32_t(0xBEEF))     == 0xBEEFu);
    REQUIRE(wad::save_load(std::uint32_t(0xDEADBEEF)) == 0xDEADBEEFu);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint32_t(42))     ==  42u);
    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint32_t(142))    == 142u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint32_t(42))     ==  42u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint32_t(142))    == 142u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint32_t(0xBEEF)) == 0xBEEFu);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint32_t(42))     ==  42u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint32_t(142))    == 142u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint32_t(0xBEEF)) == 0xBEEFu);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::uint32_t(0xDEADBEEF)) == 0xDEADBEEFu);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::uint32_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint32_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint32_t(142)) == 142);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint32_t(0x0123)) == 0x0123);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint32_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint32_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint32_t(0xBEEF)) == 0xBEEF);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint32_t(0x00C0FFEE)) == 0x00C0FFEE);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint32_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint32_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint32_t(0xBEEF)) == 0xBEEF);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint32_t(0x00C0FFEE)) == 0x00C0FFEE);
}

TEST_CASE( "uint64_t save/load", "[u64]" ) {
    REQUIRE(wad::save_load(std::uint64_t(    42))             ==  42u);
    REQUIRE(wad::save_load(std::uint64_t(   142))             == 142u);
    REQUIRE(wad::save_load(std::uint64_t(0xBEEF))             == 0xBEEFu);
    REQUIRE(wad::save_load(std::uint64_t(0xDEADBEEF))         == 0xDEADBEEFu);
    REQUIRE(wad::save_load(std::uint64_t(0xDEADBEEF00C0FFEE)) == 0xDEADBEEF00C0FFEEu);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint64_t(42))         ==  42u);
    REQUIRE(wad::save_load_convert<std::uint8_t >(std::uint64_t(142))        == 142u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint64_t(42))         ==  42u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint64_t(142))        == 142u);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::uint64_t(0xBEEF))     == 0xBEEFu);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint64_t(42))         ==  42u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint64_t(142))        == 142u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint64_t(0xBEEF))     == 0xBEEFu);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::uint64_t(0xDEADBEEF)) == 0xDEADBEEFu);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::uint64_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint64_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint64_t(142)) == 142);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::uint64_t(0x0123)) == 0x0123);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint64_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint64_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint64_t(0xBEEF)) == 0xBEEF);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::uint64_t(0x00C0FFEE)) == 0x00C0FFEE);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint64_t(42))     ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint64_t(142))    == 142);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint64_t(0xBEEF)) == 0xBEEF);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint64_t(0xDEADBEEF)) == 0xDEADBEEF);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::uint64_t(0x00C0FFEEDEADBEEF)) == 0x00C0FFEEDEADBEEF);
}

TEST_CASE( "int8_t save/load", "[i8]" ) {
    REQUIRE(wad::save_load(std::int8_t(42))  ==  42);
    REQUIRE(wad::save_load(std::int8_t(-42)) == -42);

    REQUIRE(wad::save_load_convert<std::int16_t>(std::int8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int8_t(-42)) == -42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int8_t(-42)) == -42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int8_t(42))  ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int8_t(-42)) == -42);

    REQUIRE(wad::save_load_convert<std::uint8_t>(std::int8_t(42))  ==  42);

    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int8_t(42))  == 42u);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int8_t(42))  == 42u);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int8_t(42))  == 42u);
}

TEST_CASE( "int16_t save/load", "[i16]" ) {
    REQUIRE(wad::save_load(std::int16_t(42))    ==  42);
    REQUIRE(wad::save_load(std::int16_t(-42))   == -42);
    REQUIRE(wad::save_load(std::int16_t(1024))  == 1024);
    REQUIRE(wad::save_load(std::int16_t(-1024)) == -1024);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::int16_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int8_t >(std::int16_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int16_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int16_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int16_t( 1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int16_t(-1024)) == -1024);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int16_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int16_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int16_t( 1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int16_t(-1024)) == -1024);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::int16_t(  42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int16_t(  42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int16_t(1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int16_t(  42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int16_t(1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int16_t(  42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int16_t(1024))  == 1024);
}

TEST_CASE( "int32_t save/load", "[i32]" ) {
    REQUIRE(wad::save_load(std::int32_t(     42)) ==       42);
    REQUIRE(wad::save_load(std::int32_t(    -42)) ==      -42);
    REQUIRE(wad::save_load(std::int32_t(   1024)) ==     1024);
    REQUIRE(wad::save_load(std::int32_t(  -1024)) ==    -1024);
    REQUIRE(wad::save_load(std::int32_t( 111111)) ==  111111);
    REQUIRE(wad::save_load(std::int32_t(-111111)) == -111111);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::int32_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int8_t >(std::int32_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int32_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int32_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int32_t( 1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int32_t(-1024)) == -1024);

    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t(     42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t(    -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t(   1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t(  -1024)) == -1024);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t( 111111)) ==  111111);
    REQUIRE(wad::save_load_convert<std::int64_t>(std::int32_t(-111111)) == -111111);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::int32_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int32_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int32_t(  1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int32_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int32_t(  1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int32_t(111111))  == 111111);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int32_t(  42))    ==   42);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int32_t(1024))    == 1024);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int32_t(111111))  == 111111);
}

TEST_CASE( "int64_t save/load", "[i64]" ) {
    REQUIRE(wad::save_load(std::int64_t(     42)) ==       42);
    REQUIRE(wad::save_load(std::int64_t(    -42)) ==      -42);
    REQUIRE(wad::save_load(std::int64_t(   1024)) ==     1024);
    REQUIRE(wad::save_load(std::int64_t(  -1024)) ==    -1024);
    REQUIRE(wad::save_load(std::int64_t( 111111)) ==  111111);
    REQUIRE(wad::save_load(std::int64_t(-111111)) == -111111);
    REQUIRE(wad::save_load(std::int64_t( 123456789012)) ==  123456789012);
    REQUIRE(wad::save_load(std::int64_t(-123456789012)) == -123456789012);

    REQUIRE(wad::save_load_convert<std::int8_t >(std::int64_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int8_t >(std::int64_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int64_t(   42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int64_t(  -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int64_t( 1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int16_t>(std::int64_t(-1024)) == -1024);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t(     42)) ==  42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t(    -42)) == -42);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t(   1024)) ==  1024);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t(  -1024)) == -1024);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t( 111111)) ==  111111);
    REQUIRE(wad::save_load_convert<std::int32_t>(std::int64_t(-111111)) == -111111);

    REQUIRE(wad::save_load_convert<std::uint8_t >(std::int64_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int64_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint16_t>(std::int64_t(  1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int64_t(    42))  ==   42);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int64_t(  1024))  == 1024);
    REQUIRE(wad::save_load_convert<std::uint32_t>(std::int64_t(111111))  == 111111);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int64_t(  42))    ==   42);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int64_t(1024))    == 1024);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int64_t(111111))  == 111111);
    REQUIRE(wad::save_load_convert<std::uint64_t>(std::int64_t(123456789012))  == 123456789012);
}
