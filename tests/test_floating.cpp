#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utility.hpp"
#include "wad/floating.hpp"

template<typename T>
T save_load(const T& t)
{
    wad::test_write_archive warc;
    wad::save(t, warc);

    T retval;
    wad::test_read_archive rarc(warc);
    wad::load(retval, rarc);

    return retval;
}

TEST_CASE( "float32 save/load", "[float]" ) {
    REQUIRE(save_load(1.0f    ) == 1.0f    );
    REQUIRE(save_load(3.1415f ) == 3.1415f );
    REQUIRE(save_load(6.02e23f) == 6.02e23f);
    REQUIRE(save_load(1.6e-19f) == 1.6e-19f);
}

TEST_CASE( "float64 save/load", "[double]" ) {
    REQUIRE(save_load(1.0    ) == 1.0    );
    REQUIRE(save_load(3.1415 ) == 3.1415 );
    REQUIRE(save_load(6.02e23) == 6.02e23);
    REQUIRE(save_load(1.6e-19) == 1.6e-19);
}
