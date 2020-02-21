#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/floating.hpp"

#include "utility.hpp"

TEST_CASE( "float32 save/load", "[float]" ) {
    REQUIRE(wad::save_load(1.0f    ) == 1.0f    );
    REQUIRE(wad::save_load(3.1415f ) == 3.1415f );
    REQUIRE(wad::save_load(6.02e23f) == 6.02e23f);
    REQUIRE(wad::save_load(1.6e-19f) == 1.6e-19f);
}

TEST_CASE( "float64 save/load", "[double]" ) {
    REQUIRE(wad::save_load(1.0    ) == 1.0    );
    REQUIRE(wad::save_load(3.1415 ) == 3.1415 );
    REQUIRE(wad::save_load(6.02e23) == 6.02e23);
    REQUIRE(wad::save_load(1.6e-19) == 1.6e-19);
}
