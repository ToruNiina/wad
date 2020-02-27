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

TEST_CASE("in_place wad::save/load<type::array>(...)", "[save/load<type::array>]")
{
    const int a = 42;
    const double d = 3.14;
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::save<wad::type::array>(warc, a, d, s);
    wad::test_read_archive rarc(warc);

    int         a_ = 0;
    double      d_ = 0;
    std::string s_ = "";

    wad::load<wad::type::array>(rarc, a_, d_, s_);

    REQUIRE(a_ == a);
    REQUIRE(d_ == d);
    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::archive<type::array>(...)", "[archive<type::array>]")
{
    const int a = 42;
    const double d = 3.14;
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::archive<wad::type::array>(warc, a, d, s);
    wad::test_read_archive rarc(warc);

    int         a_ = 0;
    double      d_ = 0;
    std::string s_ = "";

    wad::archive<wad::type::array>(rarc, a_, d_, s_);

    REQUIRE(a_ == a);
    REQUIRE(d_ == d);
    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::save/load<type::str>(...)", "[save/load<type::str>]")
{
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::save<wad::type::str>(warc, s);
    wad::test_read_archive rarc(warc);

    std::string s_ = "";

    wad::load<wad::type::str>(rarc, s_);

    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::archive<type::str>(...)", "[archive<type::str>]")
{
    const std::string s("foo");

    wad::test_write_archive warc;
    wad::save<wad::type::str>(warc, s);
    wad::test_read_archive rarc(warc);

    std::string s_ = "";

    wad::load<wad::type::str>(rarc, s_);

    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::save/load<type::bin>(...)", "[save/load<type::bin>]")
{
    const std::wstring s(L"foo");

    wad::test_write_archive warc;
    wad::save<wad::type::bin>(warc, s);
    wad::test_read_archive rarc(warc);

    std::wstring s_ = L"";

    wad::load<wad::type::bin>(rarc, s_);

    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::archive<type::bin>(...)", "[archive<type::bin>]")
{
    const std::wstring s(L"foo");

    wad::test_write_archive warc;
    wad::save<wad::type::bin>(warc, s);
    wad::test_read_archive rarc(warc);

    std::wstring s_ = L"";

    wad::load<wad::type::bin>(rarc, s_);

    REQUIRE(s_ == s);
}

TEST_CASE("in_place wad::save/load<type::floating>(...)", "[save/load<type::floating>]")
{
    const float f = 3.14f;

    wad::test_write_archive warc;
    wad::save<wad::type::floating>(warc, f);
    wad::test_read_archive rarc(warc);

    float f_ = 0.0;

    wad::load<wad::type::floating>(rarc, f_);

    REQUIRE(f_ == f);
}

TEST_CASE("in_place wad::archive<type::floating>(...)", "[archive<type::floating>]")
{
    const float f = 3.14f;

    wad::test_write_archive warc;
    wad::save<wad::type::floating>(warc, f);
    wad::test_read_archive rarc(warc);

    float f_ = 0.0;

    wad::load<wad::type::floating>(rarc, f_);

    REQUIRE(f_ == f);
}

TEST_CASE("in_place wad::save/load<type::integer>(...)", "[save/load<type::integer>]")
{
    const int i = 42;

    wad::test_write_archive warc;
    wad::save<wad::type::integer>(warc, i);
    wad::test_read_archive rarc(warc);

    int i_ = 0;

    wad::load<wad::type::integer>(rarc, i_);

    REQUIRE(i_ == i);
}

TEST_CASE("in_place wad::archive<type::integer>(...)", "[archive<type::integer>]")
{
    const int i = 42;

    wad::test_write_archive warc;
    wad::save<wad::type::integer>(warc, i);
    wad::test_read_archive rarc(warc);

    int i_ = 0;

    wad::load<wad::type::integer>(rarc, i_);

    REQUIRE(i_ == i);
}

TEST_CASE("in_place wad::save/load<type::boolean>(...)", "[save/load<type::boolean>]")
{
    const bool b = false;

    wad::test_write_archive warc;
    wad::save<wad::type::boolean>(warc, b);
    wad::test_read_archive rarc(warc);

    bool b_ = true;

    wad::load<wad::type::boolean>(rarc, b_);

    REQUIRE(b_ == b);
}

TEST_CASE("in_place wad::archive<type::boolean>(...)", "[archive<type::boolean>]")
{
    const bool b = false;

    wad::test_write_archive warc;
    wad::save<wad::type::boolean>(warc, b);
    wad::test_read_archive rarc(warc);

    bool b_ = true;

    wad::load<wad::type::boolean>(rarc, b_);

    REQUIRE(b_ == b);
}

TEST_CASE("in_place wad::save/load<type::nil>(...)", "[save/load<type::nil>]")
{
    wad::test_write_archive warc;
    REQUIRE(wad::save<wad::type::nil>(warc));
    wad::test_read_archive rarc(warc);
    REQUIRE(wad::load<wad::type::nil>(rarc));
}

TEST_CASE("in_place wad::archive<type::nil>(...)", "[archive<type::nil>]")
{
    wad::test_write_archive warc;
    REQUIRE(wad::save<wad::type::nil>(warc));
    wad::test_read_archive rarc(warc);
    REQUIRE(wad::load<wad::type::nil>(rarc));
}
