#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/map.hpp"
#include "wad/integer.hpp"
#include "wad/string.hpp"

#include "utility.hpp"

TEST_CASE( "std::map<std::string, int> save/load", "[std::map<std::string, int>]" )
{
    const std::map<std::string, int> upto_16{
        {"foo", 1}, {"bar", 2}, {"baz", 3},
        {"qux", 4}, {"quux", 5}
    };
    std::map<std::string, int> upto_u16;
    std::map<std::string, int> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16[std::to_string(i)] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[std::to_string(i)] = i;}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}
