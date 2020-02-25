#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/list.hpp"
#include "wad/integer.hpp"

#include "utility.hpp"

TEST_CASE( "std::list save/load", "[std::list]" )
{
    std::list<int> upto_16;
    std::list<int> upto_u16;
    std::list<int> upto_u32;
    for(int i=0; i<    5; ++i) {upto_16 .push_back(i);}
    for(int i=0; i<   20; ++i) {upto_u16.push_back(i);}
    for(int i=0; i<70000; ++i) {upto_u32.push_back(i);}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}
