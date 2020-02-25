#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/tag.hpp"

#include "utility.hpp"

TEST_CASE( "wad::tag save/load", "[wad::tag]" )
{
    for(std::uint32_t i=0; i<256; ++i)
    {
        const std::uint8_t t_ = static_cast<std::uint8_t>(i);
        const wad::tag     t  = static_cast<wad::tag>(t_);
        REQUIRE(wad::save_load(t) == t);
    }
}
