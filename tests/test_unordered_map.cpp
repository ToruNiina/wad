#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/unordered_map.hpp"
#include "wad/integer.hpp"
#include "wad/string.hpp"

#include "utility.hpp"

TEST_CASE( "std::unordered_map<std::string, int> save/load", "[std::unordered_map<std::string, int>]" )
{
    const std::unordered_map<std::string, int> upto_16{
        {"foo", 1}, {"bar", 2}, {"baz", 3},
        {"qux", 4}, {"quux", 5}
    };
    std::unordered_map<std::string, int> upto_u16;
    std::unordered_map<std::string, int> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16[std::to_string(i)] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[std::to_string(i)] = i;}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}

TEST_CASE( "std::unordered_map<int, std::string> save/load", "[std::unordered_map<int, std::string>]" )
{
    const std::unordered_map<int, std::string> upto_16{
        {1, "foo"}, {2, "bar"}, {3, "baz"},
        {4, "qux"}, {5, "quux"}
    };
    std::unordered_map<int, std::string> upto_u16;
    std::unordered_map<int, std::string> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16[i] = std::to_string(i);}
    for(int i=0; i<70000; ++i) {upto_u32[i] = std::to_string(i);}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}
