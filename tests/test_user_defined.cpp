#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/integer.hpp"
#include "wad/floating.hpp"
#include "wad/string.hpp"
#include "wad/in_place.hpp"
#include "wad/archive.hpp"
#include "wad/interface.hpp"

#include "utility.hpp"

namespace extlib
{
struct X
{
    std::string  s;
    std::int32_t i;
    double       d;

    template<typename Arc>
    bool archive(Arc& arc)
    {
        return wad::archive<wad::type::map>(arc, "s", s, "i", i, "d", d);
    }
};
struct Y
{
    std::string  s;
    std::int32_t i;
    double       d;

    template<typename Arc>
    bool archive(Arc& arc)
    {
        return wad::archive<wad::type::array>(arc, s, i, d);
    }
};
struct Z
{
    std::string  s;
    std::int32_t i;
    double       d;

    template<typename Arc>
    bool save(Arc& arc) const
    {
        return wad::save<wad::type::map>(arc, "s", s, "i", i, "d", d);
    }
    template<typename Arc>
    bool load(Arc& arc)
    {
        return wad::load<wad::type::map>(arc, "s", s, "i", i, "d", d);
    }
};
struct W
{
    std::string  s;
    std::int32_t i;
    double       d;

    template<typename Arc>
    bool save(Arc& arc) const
    {
        return wad::save<wad::type::array>(arc, s, i, d);
    }
    template<typename Arc>
    bool load(Arc& arc)
    {
        return wad::load<wad::type::array>(arc, s, i, d);
    }
};
struct V
{
    std::string  s;
    std::int32_t i;
    double       d;

};
template<typename Arc>
bool save(Arc& arc, const V& v)
{
    return wad::save<wad::type::array>(arc, v.s, v.i, v.d);
}
template<typename Arc>
bool load(Arc& arc, V& v)
{
    return wad::load<wad::type::array>(arc, v.s, v.i, v.d);
}
} // extlib

TEST_CASE( "user_defined<map>::archive()", "[user_defined<map>::archive()]" )
{
    extlib::X x{"foo", 42, 3.14};

    {
        extlib::X x1;
        wad::test_write_archive warc;
        wad::save(warc, x);

        wad::test_read_archive rarc(warc);
        wad::load(rarc, x1);

        REQUIRE(x.s == x1.s);
        REQUIRE(x.i == x1.i);
        REQUIRE(x.d == x1.d);
    }
    {
        extlib::X x1;
        wad::test_write_archive warc;
        wad::archive(warc, x);

        wad::test_read_archive rarc(warc);
        wad::archive(rarc, x1);

        REQUIRE(x.s == x1.s);
        REQUIRE(x.i == x1.i);
        REQUIRE(x.d == x1.d);
    }
}
TEST_CASE( "user_defined<array>::archive()", "[user_defined<array>::archive()]" )
{
    extlib::Y x{"foo", 42, 3.14};
    {
        extlib::Y x1;
        wad::test_write_archive warc;
        wad::save(warc, x);

        wad::test_read_archive rarc(warc);
        wad::load(rarc, x1);

        REQUIRE(x.s == x1.s);
        REQUIRE(x.i == x1.i);
        REQUIRE(x.d == x1.d);
    }
    {
        extlib::Y x1;
        wad::test_write_archive warc;
        wad::archive(warc, x);

        wad::test_read_archive rarc(warc);
        wad::archive(rarc, x1);

        REQUIRE(x.s == x1.s);
        REQUIRE(x.i == x1.i);
        REQUIRE(x.d == x1.d);
    }
}
TEST_CASE( "user_defined<map>::save/load()", "[user_defined<map>::save/load()]" )
{
    const extlib::Z x{"foo", 42, 3.14};
    const extlib::Z x1 = wad::save_load(x);

    REQUIRE(x.s == x1.s);
    REQUIRE(x.i == x1.i);
    REQUIRE(x.d == x1.d);
}
TEST_CASE( "user_defined<array>::save/load()", "[user_defined<array>::save/load()]" )
{
    const extlib::W x{"foo", 42, 3.14};
    const extlib::W x1 = wad::save_load(x);

    REQUIRE(x.s == x1.s);
    REQUIRE(x.i == x1.i);
    REQUIRE(x.d == x1.d);
}
TEST_CASE( "save/load(user_defined<array>)", "[save/load(user_defined<array>)]" )
{
    const extlib::V x{"foo", 42, 3.14};
    const extlib::V x1 = wad::save_load(x);

    REQUIRE(x.s == x1.s);
    REQUIRE(x.i == x1.i);
    REQUIRE(x.d == x1.d);
}
