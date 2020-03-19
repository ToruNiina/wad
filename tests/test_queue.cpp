#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/deque.hpp"
#include "wad/queue.hpp"
#include "wad/integer.hpp"

#include "utility.hpp"

TEST_CASE( "std::queue save/load", "[std::queue]" )
{
    const std::queue<int> upto_16{{1,2,3,4,5}};

    std::queue<int> upto_u16;
    std::queue<int> upto_u32;
    for(int i=0; i<20;    ++i) {upto_u16.push(i);}
    for(int i=0; i<70000; ++i) {upto_u32.push(i);}

    REQUIRE(wad::save_load(upto_16)  == upto_16);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}
