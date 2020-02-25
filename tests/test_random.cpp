#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/random.hpp"

#include "utility.hpp"

TEST_CASE( "std::mt19937 save/load", "[std::mt19937]" )
{
    std::mt19937 rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::mt19937_64 save/load", "[std::mt19937_64]" )
{
    std::mt19937_64 rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::minstd_rand0 save/load", "[std::minstd_rand0]" )
{
    std::minstd_rand0 rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::minstd_rand save/load", "[std::minstd_rand]" )
{
    std::minstd_rand rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::ranlux24 save/load", "[std::ranlux24]" )
{
    std::ranlux24 rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::ranlux48 save/load", "[std::ranlux48]" )
{
    std::ranlux48 rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}

TEST_CASE( "std::knuth_b save/load", "[std::knuth_b]" )
{
    std::knuth_b rng(12345);
    rng.discard(100);
    REQUIRE(wad::save_load(rng) == rng);
}
