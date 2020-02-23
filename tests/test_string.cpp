#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/string.hpp"

#include "utility.hpp"

TEST_CASE( "std::string save/load", "[std::string]" )
{
    const std::string upto_32("foo");
    const std::string upto_u8("Lorem ipsum dolor sit amet, consectetur "
                              "adipiscing elit, sed do eiusmod tempor "
                              "incididunt ut labore et dolore magna aliqua.");

    const std::string lorem_ipsum("Lorem ipsum dolor sit amet, consectetur "
        "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
        "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco"
        " laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor"
        " in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
        " pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa"
        " qui officia deserunt mollit anim id est laborum.");// 445 chars > 255

    const std::string upto_u16(lorem_ipsum);
    const std::string upto_u32 = [&]() -> std::string {
        std::string str;
        for(std::size_t i=0; i<(70000 / lorem_ipsum.size() + 1); ++i)
        {
            str += lorem_ipsum;
        }
        return str;
    }();

    REQUIRE(wad::save_load(upto_32)  == upto_32);
    REQUIRE(wad::save_load(upto_u8)  == upto_u8);
    REQUIRE(wad::save_load(upto_u16) == upto_u16);
    REQUIRE(wad::save_load(upto_u32) == upto_u32);
}

TEST_CASE( "const char* save/std::string load", "[const char*]" )
{
    REQUIRE(wad::save_load_convert<std::string>("foo") == "foo");
    REQUIRE(wad::save_load_convert<std::string>(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.") ==
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    REQUIRE(wad::save_load_convert<std::string>("Lorem ipsum dolor sit amet,"
        " consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore"
        " et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud"
        " exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."
        " Duis aute irure dolor in reprehenderit in voluptate velit esse cillum"
        " dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non"
        " proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
        == "Lorem ipsum dolor sit amet,"
        " consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore"
        " et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud"
        " exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."
        " Duis aute irure dolor in reprehenderit in voluptate velit esse cillum"
        " dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non"
        " proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
}
