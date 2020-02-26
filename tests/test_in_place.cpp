#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/in_place.hpp"
#include "wad/integer.hpp"
#include "wad/floating.hpp"
#include "wad/string.hpp"

#include "utility.hpp"

TEST_CASE("in_place wad::save/load<type::map>(...)", "[save/load<type::map>]")
{
    const int a = 42;
    const double d = 3.14;
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::save<wad::type::map>(warc, "a", a, "d", d, "s", s);
    wad::test_read_archive rarc(warc);

    int         a_ = 0;
    double      d_ = 0;
    std::string s_ = "";

    wad::load<wad::type::map>(rarc, "a", a_, "d", d_, "s", s_);

    REQUIRE(a_ == a);
    REQUIRE(d_ == d);
    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::archive<type::map>(...)", "[archive<type::map>]")
{
    const int a = 42;
    const double d = 3.14;
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::archive<wad::type::map>(warc, "a", a, "d", d, "s", s);
    wad::test_read_archive rarc(warc);

    int         a_ = 0;
    double      d_ = 0;
    std::string s_ = "";

    wad::archive<wad::type::map>(rarc, "a", a_, "d", d_, "s", s_);

    REQUIRE(a_ == a);
    REQUIRE(d_ == d);
    REQUIRE(s_ == s);
}
