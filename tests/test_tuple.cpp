#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/tuple.hpp"
#include "wad/integer.hpp"
#include "wad/floating.hpp"
#include "wad/string.hpp"

#include "utility.hpp"

TEST_CASE( "std::tuple save/load", "[std::tuple]" )
{
    const std::tuple<int, double, std::string> t{42, 3.14, "foo"};

    REQUIRE(wad::save_load(t) == t);
}
