#ifndef WAD_RANDOM_HPP
#define WAD_RANDOM_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
#include "string.hpp"
#include <utility>
#include <random>
#include <sstream>

// Here, it serialize random engine in the following format. Ofcourse defining a
// new extension type is more efficient. But to avoid ext-tag collision with
// other code base, it uses the standard format.
//
// fixmap<2>{name: "typename", state: "state string"}
//
namespace wad
{
namespace detail
{
template<typename RNG, typename Arc>
bool save_rng(Arc& arc, const RNG& v, const std::string& name)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(arc, static_cast<tag>(t)))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    const std::string state_key("state");
    std::ostringstream oss;
    oss << v;

    if(!save(arc, name_key)  || !save(arc, name) ||
       !save(arc, state_key) || !save(arc, oss.str()))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}
template<typename RNG, typename Arc>
bool load_rng(Arc& arc, RNG& v, const std::string& ref_name)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(arc, t)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string key, name, state;
    if(!load(arc, key)   || key != "name"    ||
       !load(arc, name)  || name != ref_name ||
       !load(arc, key)   || key != "state"   ||
       !load(arc, state))
    {
        arc.seek(savepoint);
        return false;
    }

    std::istringstream iss(state);
    iss >> v;
    // check failbit status.
    if((iss.rdstate() & std::ios_base::failbit) != 0)
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}
} // detail


template<typename UInt, UInt A, UInt C, UInt M, typename Arc>
bool save(Arc& arc, const std::linear_congruential_engine<UInt, A, C, M>& v)
{
    return detail::save_rng(arc, v, "std::linear_congruential_engine");
}

template<typename UInt, UInt A, UInt C, UInt M, typename Arc>
bool load(Arc& arc, std::linear_congruential_engine<UInt, A, C, M>& v)
{
    return detail::load_rng(arc, v, "std::linear_congruential_engine");
}

template<typename UInt,
         std::size_t W, std::size_t N, std::size_t M, std::size_t R,
         UInt A, std::size_t U, UInt D, std::size_t S, UInt B, std::size_t T,
         UInt C, std::size_t L, UInt F, typename Arc>
bool save(Arc& arc,
    const std::mersenne_twister_engine<UInt,W,N,M,R,A,U,D,S,B,T,C,L,F>& v)
{
    return detail::save_rng(arc, v, "std::mersenne_twister_engine");
}

template<typename UInt,
         std::size_t W, std::size_t N, std::size_t M, std::size_t R,
         UInt A, std::size_t U, UInt D, std::size_t S, UInt B, std::size_t T,
         UInt C, std::size_t L, UInt F, typename Arc>
bool load(Arc& arc,
          std::mersenne_twister_engine<UInt,W,N,M,R,A,U,D,S,B,T,C,L,F>& v)
{
    return detail::load_rng(arc, v, "std::mersenne_twister_engine");
}

template<typename UInt, std::size_t W, std::size_t S, std::size_t R,
         typename Arc>
bool save(Arc& arc, const std::subtract_with_carry_engine<UInt, W, S, R>& v)
{
    return detail::save_rng(arc, v, "std::subtract_with_carry_engine");
}

template<typename UInt, std::size_t W, std::size_t S, std::size_t R,
         typename Arc>
bool load(Arc& arc, std::subtract_with_carry_engine<UInt, W, S, R>& v)
{
    return detail::load_rng(arc, v, "std::subtract_with_carry_engine");
}

template<typename Engine, std::size_t P, std::size_t R, typename Arc>
bool save(Arc& arc, const std::discard_block_engine<Engine, P, R>& v)
{
    return detail::save_rng(arc, v, "std::discard_block_engine");
}

template<typename Engine, std::size_t P, std::size_t R, typename Arc>
bool load(Arc& arc, std::discard_block_engine<Engine, P, R>& v)
{
    return detail::load_rng(arc, v, "std::discard_block_engine");
}

template<typename Engine, std::size_t W, typename UInt, typename Arc>
bool save(Arc& arc, const std::independent_bits_engine<Engine, W, UInt>& v)
{
    return detail::save_rng(arc, v, "std::independent_bits_engine");
}

template<typename Engine, std::size_t W, typename UInt, typename Arc>
bool load(Arc& arc, std::independent_bits_engine<Engine, W, UInt>& v)
{
    return detail::load_rng(arc, v, "std::independent_bits_engine");
}

template<typename Engine, std::size_t K, typename Arc>
bool save(Arc& arc, const std::shuffle_order_engine<Engine, K>& v)
{
    return detail::save_rng(arc, v, "std::shuffle_order_engine");
}

template<typename Engine, std::size_t K, typename Arc>
bool load(Arc& arc, std::shuffle_order_engine<Engine, K>& v)
{
    return detail::load_rng(arc, v, "std::shuffle_order_engine");
}
} // wad
#endif// WAD_RANDOM_HPP
