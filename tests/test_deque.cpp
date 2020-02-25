#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/deque.hpp"
#include "wad/integer.hpp"

#include "utility.hpp"

TEST_CASE( "std::deque save/load", "[std::deque]" )
{
    const std::deque<int> upto_16{{1,2,3,4,5}};

    std::deque<int> upto_u16(20);
    std::deque<int> upto_u32(70000);
    for(int i=0; i<20;    ++i) {upto_u16[i] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[i] = i;}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}
