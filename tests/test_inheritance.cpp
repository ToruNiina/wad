#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/integer.hpp"
#include "wad/floating.hpp"
#include "wad/string.hpp"
#include "wad/in_place.hpp"
#include "wad/archive.hpp"
#include "wad/interface.hpp"
#include "wad/inheritance.hpp"
#include "wad/unique_ptr.hpp"
#include "utility.hpp"
#include <iostream>

namespace extlib
{
struct B1
{
    std::string  s;

    B1() = default;
    explicit B1(std::string s_): s(s_){}
    virtual ~B1() = default;

    template<typename Arc>
    bool save(Arc& arc) const
    {
        return wad::save<wad::type::map>(arc, "s", s);
    }
    template<typename Arc>
    bool load(Arc& arc)
    {
        return wad::load<wad::type::map>(arc, "s", s);
    }

    virtual std::string dump() const = 0;
};
struct D1 : B1
{
    std::int32_t i;
    double       d;

    D1() = default;
    D1(const std::string s, const int i, const double d): B1(s), i(i), d(d) {}
    ~D1() override = default;

    template<typename Arc>
    bool save(Arc& arc) const
    {
        return wad::save<wad::type::map>(arc,
                wad::base_of<B1>(this), "i", i, "d", d);
    }
    template<typename Arc>
    bool load(Arc& arc)
    {
        return wad::load<wad::type::map>(arc,
                wad::base_of<B1>(this), "i", i, "d", d);
    }

    std::string dump() const override
    {
        return "D1{s:" + this->s + ", i:" + std::to_string(i) +
               ", d:" + std::to_string(d) + "}";
    }
};
} // extlib

namespace wad {
template<>
struct register_subclass<extlib::B1, extlib::D1>
{
    static constexpr const char* name() noexcept {return "extlib::D1";}
    static const registered<extlib::B1, extlib::D1> bound;
};
const registered<extlib::B1, extlib::D1>
    register_subclass<extlib::B1, extlib::D1>::bound(
        bind_archivers<test_write_archive, test_read_archive>{});
} /* wad */

TEST_CASE( "polymorphic save/load", "[save/load]" )
{
    std::unique_ptr<extlib::B1> x(new extlib::D1("foo", 42, 3.14));
    {
        std::unique_ptr<extlib::B1> x1 = nullptr;
        wad::test_write_archive warc;
        REQUIRE(wad::save(warc, x));

        wad::test_read_archive rarc(warc);
        REQUIRE(wad::load(rarc, x1));
        REQUIRE(x->dump() == x1->dump());
    }
    {
        std::unique_ptr<extlib::B1> x1 = nullptr;
        wad::test_write_archive warc;
        REQUIRE(wad::archive(warc, x));

        wad::test_read_archive rarc(warc);
        REQUIRE(wad::archive(rarc, x1));
        REQUIRE(x->dump() == x1->dump());
    }
}
