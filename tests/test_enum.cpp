#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/enum.hpp"
#include "utility.hpp"

enum color
{
    red, green, blue
};

TEST_CASE( "default enum save/load", "[enum-default]" ) {
    REQUIRE(wad::save_load(red)   == red);
    REQUIRE(wad::save_load(green) == green);
    REQUIRE(wad::save_load(blue)  == blue);
}

enum class fruit : std::int8_t
{
    apple = 0,
    banana = 1,
    orange = 2,
    peach = 3
};


TEST_CASE( "enum class save/load", "[enum-class]" ) {
    REQUIRE(wad::save_load(fruit::apple)  == fruit::apple);
    REQUIRE(wad::save_load(fruit::banana) == fruit::banana);
    REQUIRE(wad::save_load(fruit::orange) == fruit::orange);
    REQUIRE(wad::save_load(fruit::peach)  == fruit::peach);
}
