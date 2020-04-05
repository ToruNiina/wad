#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/atomic.hpp"
#include "utility.hpp"

TEST_CASE( "atomic<int> save/load", "[atomic<int>]" ) {
    std::atomic<int> a(42);
    std::atomic<int> b(54);

    wad::test_write_archive warc;
    wad::save(warc, a);

    wad::test_read_archive rarc(warc);
    wad::load(rarc, b);

    REQUIRE(a.load() == b.load());
}

