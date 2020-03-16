#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/valarray.hpp"
#include "wad/floating.hpp"

#include "utility.hpp"

TEST_CASE( "std::valarray save/load", "[std::valarray]" )
{
    const std::valarray<double> upto_16{{1,2,3,4,5}};
    std::valarray<double> upto_u16(20);
    std::valarray<double> upto_u32(70000);
    for(int i=0; i<20;    ++i) {upto_u16[i] = i;}
    for(int i=0; i<70000; ++i) {upto_u32[i] = i;}

    const auto test_upto_16  = wad::save_load(upto_16);
    const auto test_upto_u16 = wad::save_load(upto_u16);
    const auto test_upto_u32 = wad::save_load(upto_u32);

    for(std::size_t i=0; i<test_upto_16.size(); ++i)
    {
        REQUIRE(test_upto_16[i] == upto_16[i]);
    }
    for(std::size_t i=0; i<test_upto_u16.size(); ++i)
    {
        REQUIRE(test_upto_u16[i] == upto_u16[i]);
    }
    for(std::size_t i=0; i<test_upto_u32.size(); ++i)
    {
        REQUIRE(test_upto_u32[i] == upto_u32[i]);
    }
}
