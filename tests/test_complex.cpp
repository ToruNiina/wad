#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/complex.hpp"
#include "wad/floating.hpp"

#include "utility.hpp"

TEST_CASE( "std::complex<double> save/load", "[std::complex, double]" )
{
    const std::complex<double> z(3.14, 2.71);
    REQUIRE(wad::save_load(z) == z);
}

TEST_CASE( "std::complex<float> save/load", "[std::complex, float]" )
{
    const std::complex<float> z(3.14, 2.71);
    REQUIRE(wad::save_load(z) == z);
}
