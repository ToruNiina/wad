#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/optional.hpp"
#include "wad/integer.hpp"
#include "utility.hpp"

TEST_CASE( "optional save/load", "[optional]" ) {
    const std::optional<int> opt1(42);
    const std::optional<int> opt2;

    REQUIRE(wad::save_load(opt1) == opt1);
    REQUIRE(wad::save_load(opt2) == opt2);
}
