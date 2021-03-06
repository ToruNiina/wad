#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/array.hpp"
#include "wad/integer.hpp"

#include "utility.hpp"

TEST_CASE( "std::array save/load", "[std::array]" )
{
    const std::array<int, 5>     upto_16{{1,2,3,4,5}};

    std::array<int, 20>    upto_u16;
    std::array<int, 70000> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16[i] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[i] = i;}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}

TEST_CASE( "std::array archive", "[archive(std::array)]" )
{
    const std::array<int, 5> upto_16{{1,2,3,4,5}};

    std::array<int, 20>    upto_u16;
    std::array<int, 70000> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16[i] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[i] = i;}

    REQUIRE(wad::archive_save_load(upto_16)  == upto_16);
    REQUIRE(wad::archive_save_load(upto_u16) == upto_u16);
    REQUIRE(wad::archive_save_load(upto_u32) == upto_u32);
}
