#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/bitset.hpp"

#include "utility.hpp"

TEST_CASE( "std::bitset save/load", "[std::bitset]" )
{
    const std::bitset<8> v(131uL);
    REQUIRE(wad::save_load(v) == v);
}
