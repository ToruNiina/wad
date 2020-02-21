#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/boolean.hpp"
#include "utility.hpp"

TEST_CASE( "boolean save/load", "[bool]" ) {
    REQUIRE(wad::save_load(true)  == true);
    REQUIRE(wad::save_load(false) == false);
}
