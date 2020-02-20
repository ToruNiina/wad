#ifndef WAD_RANDOM_HPP
#define WAD_RANDOM_HPP
#include "binary_cast.hpp"
#include "tag.hpp"
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

template<typename UInt, UInt A, UInt C, UInt M, typename Arc>
bool save(const std::linear_congruential_engine<UInt, A, C, M>& v, Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::linear_congruential_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename UInt, UInt A, UInt C, UInt M, typename Arc>
bool load(std::linear_congruential_engine<UInt, A, C, M>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::linear_congruential_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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

template<typename UInt,
         std::size_t W, std::size_t N, std::size_t M, std::size_t R,
         UInt A, std::size_t U, UInt D, std::size_t S, UInt B, std::size_t T,
         UInt C, std::size_t L, UInt F, typename Arc>
bool save(const std::mersenne_twister_engine<UInt,W,N,M,R,A,U,D,S,B,T,C,L,F>& v,
          Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::mersenne_twister_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename UInt,
         std::size_t W, std::size_t N, std::size_t M, std::size_t R,
         UInt A, std::size_t U, UInt D, std::size_t S, UInt B, std::size_t T,
         UInt C, std::size_t L, UInt F, typename Arc>
bool load(std::mersenne_twister_engine<UInt,W,N,M,R,A,U,D,S,B,T,C,L,F>& v,
          Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::mersenne_twister_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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

template<typename UInt, std::size_t W, std::size_t S, std::size_t R,
         typename Arc>
bool save(const std::subtract_with_carry_engine<UInt, W, S, R>& v, Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::subtract_with_carry_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename UInt, std::size_t W, std::size_t S, std::size_t R,
         typename Arc>
bool load(std::subtract_with_carry_engine<UInt, W, S, R>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::subtract_with_carry_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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

template<typename Engine, std::size_t P, std::size_t R, typename Arc>
bool save(const std::discard_block_engine<Engine, P, R>& v, Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::discard_block_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename Engine, std::size_t P, std::size_t R, typename Arc>
bool load(std::discard_block_engine<Engine, P, R>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::discard_block_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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

template<typename Engine, std::size_t W, typename UInt, typename Arc>
bool save(const std::independent_bits_engine<Engine, W, UInt>& v, Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::independent_bits_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename Engine, std::size_t W, typename UInt, typename Arc>
bool load(std::independent_bits_engine<Engine, W, UInt>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::independent_bits_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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

template<typename Engine, std::size_t K, typename Arc>
bool save(const std::shuffle_order_engine<Engine, K>& v, Arc& arc)
{
    const auto savepoint = arc.npos();
    const std::uint8_t t = static_cast<std::uint8_t>(tag::fixmap_lower) + 2;
    if(!save(static_cast<tag>(t), arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string name_key("name");
    if(!save(name_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    const std::string name = "std::shuffle_order_engine";
    if(!save(name, arc))
    {
        arc.seek(savepoint);
        return false;
    }

    const std::string state_key("state");
    if(!save(state_key, arc))
    {
        arc.seek(savepoint);
        return false;
    }
    std::ostringstream oss;
    oss << v;
    if(!save(oss.str(), arc))
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

template<typename Engine, std::size_t K, typename Arc>
bool load(std::shuffle_order_engine<Engine, K>& v, Arc& arc)
{
    const auto savepoint = arc.npos();

    tag t;
    if(!load(t, arc)) {return false;}
    const std::uint8_t expected =
        static_cast<std::uint8_t>(tag::fixmap_lower) + 2;

    if(static_cast<std::uint8_t>(t) != expected)
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name_key;
    if(!load(name_key, arc) || name_key != "name")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string name;
    if(!load(name, arc) || name != "std::shuffle_order_engine")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state_key;
    if(!load(state_key, arc) || state_key != "state")
    {
        arc.seek(savepoint);
        return false;
    }

    std::string state;
    if(!load(state, arc))
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
} // wad
#endif// WAD_RANDOM_HPP
