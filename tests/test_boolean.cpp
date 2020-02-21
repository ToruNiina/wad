#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utility.hpp"
#include "wad/boolean.hpp"

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

TEST_CASE( "boolean save/load", "[bool]" ) {
    REQUIRE(save_load(true)  == true);
    REQUIRE(save_load(false) == false);
}
