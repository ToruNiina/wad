#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/pair.hpp"
#include "wad/integer.hpp"
#include "wad/floating.hpp"

#include "utility.hpp"

TEST_CASE( "std::pair save/load", "[std::pair]" )
{
    const std::pair<int, double> p{42, 3.14};

    REQUIRE(wad::save_load(p) == p);
}
