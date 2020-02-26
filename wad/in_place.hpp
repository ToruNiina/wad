#ifndef WAD_IN_PLACE_HPP
#define WAD_IN_PLACE_HPP
#include "type.hpp"
#include "boolean.hpp"
#include "integer.hpp"
#include "floating.hpp"
#include "string.hpp"
#include "type_traits.hpp"
#include "inheritance.hpp"
#include <cstring>

namespace wad
{

// The main purpose of this feature is to provide the more convenient way to
// write types that has dynamic size, such as array and map.
//
// Writing correct type tag is a hard task.
// To make it easier, this function is provided. E.g.
// ```cpp
// struct X
// {
//     int a;
//     double b;
//     std::string c;
//
//     template<typename Arc>
//     bool save(Arc& arc) const
//     {
//         // here it counts number of arguments and add appropreate tag
//         return save<wad::type::map>(arc, "a", a, "b", b, "c", c);
//     }
// };
// ```

// forward declaration
namespace detail
{
template<type T> struct save_load_util;
} // detail

// ----------------------------------------------------------------------------
// the default version without base_binder.

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<is_base_binder<Args>>...>::value, bool>::type
save(Arc& arc, Args&& ... args)
{
    return detail::save_load_util<T>::save_impl(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<is_base_binder<Args>>...>::value, bool>::type
load(Arc& arc, Args&& ... args)
{
    return detail::save_load_util<T>::load_impl(arc, std::forward<Args>(args) ...);
}

// ----------------------------------------------------------------------------
// with base_binder.

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
bool save(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    save(arc, bb);
    return detail::save_load_util<T>::save_impl(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
bool load(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    load(arc, bb);
    return detail::save_load_util<T>::load_impl(arc, std::forward<Args>(args) ...);
}

// -------------------------------------------------------------------------------
// archive version

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, Args&& ... args)
{
    return save<T>(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, Args&& ... args)
{
    return load<T>(arc, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    return save<T>(arc, bb, std::forward<Args>(args) ...);
}

template<type T, typename Arc, typename Base, typename Derived, typename ... Args>
typename std::enable_if<detail::conjunction<
    detail::negation<has_sink_method<Arc>>, has_src_method<Arc>,
    detail::negation<is_base_binder<Args>> ...
    >::value, bool>::type
archive(Arc& arc, const base_binder<Base, Derived>& bb, Args&& ... args)
{
    return load<T>(arc, bb, std::forward<Args>(args) ...);
}

// ----------------------------------------------------------------------------
// implementation.

namespace detail
{

template<>
struct save_load_util<type::nil>
{
    template<typename Arc>
    static bool invoke(Arc& arc) noexcept
    {
        return save(arc, tag::nil);
    }

    template<typename Arc>
    static bool load_impl(Arc& arc)
    {
        const auto savepoint = arc.npos();
        tag t;
        if(!load(arc, t) || t != tag::nil)
        {
            return false;
        }
        return true;
    }
};
template<>
struct save_load_util<type::boolean>
{
    template<typename Arc>
    static bool save_impl(Arc& arc, const bool v) noexcept
    {
        return save(arc, v);
    }
    template<typename Arc>
    static bool load_impl(Arc& arc, const bool v) noexcept
    {
        return load(arc, v);
    }
};
template<>
struct save_load_util<type::integer>
{
    template<typename T, typename Arc>
    static bool save_impl(Arc& arc, const T v) noexcept
    {
        static_assert(std::is_integral<T>::value,
                      "type of an integer value should be an integer");
        return save(arc, v);
    }
    template<typename T, typename Arc>
    static bool load_impl(Arc& arc, T& v) noexcept
    {
        static_assert(std::is_integral<T>::value,
                      "type of an integer value should be an integer");
        return load(arc, v);
    }
};
template<>
struct save_load_util<type::floating>
{
    template<typename T, typename Arc>
    static bool save_impl(Arc& arc, const T v) noexcept
    {
        static_assert(std::is_floating_point<T>::value,
                      "type of an integer value should be an integer");
        return save(arc, v);
    }
    template<typename T, typename Arc>
    static bool load_impl(Arc& arc, const T v) noexcept
    {
        static_assert(std::is_floating_point<T>::value,
                      "type of an integer value should be an integer");
        return load(arc, v);
    }
};
template<>
struct save_load_util<type::str>
{
    template<typename traits, typename Alloc, typename Arc>
    static bool save_impl(Arc& arc,
            const std::basic_string<char, traits, Alloc>& v) noexcept
    {
        return save(arc, v);
    }
    template<typename traits, typename Alloc, typename Arc>
    static bool load_impl(Arc& arc,
            const std::basic_string<char, traits, Alloc>& v) noexcept
    {
        return load(arc, v);
    }
    template<typename Arc>
    static bool save_impl(Arc& arc, const char* v) noexcept
    {
        return save(arc, v);
    }
};

template<>
struct save_load_util<type::bin>
{
    // for std::basic_string<charT, traitsT, Alloc>
    template<typename charT, typename traitsT, typename Alloc, typename Arc>
    static bool save_impl(Arc& arc,
            const std::basic_string<charT, traitsT, Alloc>& v) noexcept
    {
        const auto savepoint = arc.npos();
        const auto len = sizeof(charT) * v.size();

        if(!save_length<type::bin>(arc, len) || !arc.is_writable(len))
        {
            arc.seek(savepoint);
            return false;
        }
        std::copy_n(reinterpret_cast<const char*>(v.data()), len, arc.sink());
        arc.advance(len);
        return true;
    }
    template<typename charT, typename traitsT, typename Alloc, typename Arc>
    static bool load_impl(Arc& arc,
            std::basic_string<charT, traitsT, Alloc>& v) noexcept
    {
        const auto savepoint = arc.npos();

        std::size_t len_bytes = 0;
        if(!load_length<type::bin>(arc, len_bytes) || !arc.is_readable(len_bytes))
        {
            arc.seek(savepoint);
            return false;
        }
        if(len_bytes % sizeof(charT) != 0)
        {
            return false;
        }
        const std::size_t len = len_bytes / sizeof(charT);

        std::vector<charT> buf(len);
        std::copy_n(arc.src(), len_bytes, reinterpret_cast<char*>(v.data()));
        arc.advance(len_bytes);

        v = std::basic_string<charT, traitsT, Alloc>(buf.data(), len);
        return true;
    }

    // for char* and their variants
    template<typename C, typename Arc>
    static bool save_impl(Arc& arc, const C* v) noexcept
    {
        using traitsT = std::char_traits<C>;
        const auto savepoint = arc.npos();
        const auto len = sizeof(C) * traitsT::length(v);

        if(!save_length<type::bin>(arc, len) || !arc.is_writable(len))
        {
            arc.seek(savepoint);
            return false;
        }
        std::copy_n(reinterpret_cast<const char*>(v), len, arc.sink());
        arc.advance(len);
        return true;
    }
};

template<>
struct save_load_util<type::array>
{
    template<typename Arc>
    static bool save_impl(Arc& arc) noexcept
    {
        return save(arc, tag::fixarray_lower);
    }

    template<typename Arc, typename T, typename ... Ts>
    static bool save_impl(Arc& arc, const T& head, const Ts& ... tail)
    {
        constexpr std::size_t len = sizeof...(Ts) + 1;
        const auto savepoint = arc.npos();

        if(!save_length<type::array>(arc, len))
        {
            arc.seek(savepoint);
            return false;
        }

        if(!save(arc, head) || !save_rec(arc, tail...))
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }

    template<typename Arc>
    static bool save_rec(Arc& arc)
    {
        return true;
    }
    template<typename Arc, typename T, typename ... Ts>
    static bool save_rec(Arc& arc, const T& head, const Ts& ... tail)
    {
        if(!save(arc, head)) {return false;}
        return save_rec(arc, tail ...);
    }

    // -----------------------------------------------------------------------

    template<typename Arc>
    static bool load_impl(Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();
        tag t;
        if(!load(arc, t) || t != tag::fixarray_lower)
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }

    template<typename Arc, typename T, typename ... Ts>
    static bool load_impl(Arc& arc, T& head, Ts& ... tail)
    {
        constexpr std::size_t len = sizeof...(Ts) + 1;
        const auto savepoint = arc.npos();

        if(!load_length<type::array>(arc, len))
        {
            arc.seek(savepoint);
            return false;
        }

        if(!load(arc, head) || !load_rec(arc, tail...))
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }

    template<typename Arc>
    static bool load_red(Arc& arc)
    {
        return true;
    }
    template<typename Arc, typename T, typename ... Ts>
    static bool load_rec(Arc& arc, T& head, Ts& ... tail)
    {
        if(!load(arc, head)) {return false;}
        return load_rec(arc, tail ...);
    }
};

template<>
struct save_load_util<type::map>
{
    template<typename Arc>
    static bool save_impl(Arc& arc) noexcept
    {
        return save(arc, tag::fixmap_lower);
    }

    template<typename Arc, typename K, typename V, typename ... Ts>
    static bool save_impl(Arc& arc, const K& key, const V& val,
                          const Ts& ... tail) noexcept
    {
        static_assert(sizeof...(tail) % 2 == 0, "");
        const auto savepoint = arc.npos();

        if(!save_length<type::map>(arc, sizeof...(tail) / 2 + 1))
        {
            arc.seek(savepoint);
            return false;
        }

        if(!save(arc, key) || !save(arc, val))
        {
            return false;
        }
        return save_rec(arc, savepoint, tail...);
    }

    template<typename Arc, typename SavePoint>
    static bool save_rec(Arc&, const SavePoint&) noexcept
    {
        return true;
    }

    template<typename Arc, typename SavePoint,
             typename K, typename V, typename ... Ts>
    static bool save_rec(Arc& arc, const SavePoint& savepoint,
            const K& key, const V& val, const Ts& ... tail) noexcept
    {
        if(!save(arc, key) || !save(arc, val))
        {
            arc.seek(savepoint);
            return false;
        }
        return save_rec(arc, savepoint, tail...);
    }

    // ------------------------------------------------------------------------

    template<typename Arc>
    static bool load_impl(Arc& arc) noexcept
    {
        const auto savepoint = arc.npos();
        tag t;
        if(!load(arc, t) || t != tag::fixmap_lower)
        {
            arc.seek(savepoint);
            return false;
        }
        return true;
    }

    template<typename Arc, typename ... Ts>
    static bool load_impl(Arc& arc, Ts&& ... tail) noexcept
    {
        static_assert(sizeof...(tail) % 2 == 0, "");
        const auto savepoint = arc.npos();

        std::size_t len;
        if(!load_length<type::map>(arc, len) || len != (sizeof...(tail) / 2))
        {
            arc.seek(savepoint);
            return false;
        }
        return load_rec(arc, savepoint, std::forward<Ts>(tail)...);
    }

    template<typename Arc, typename SavePoint>
    static bool load_rec(Arc&, const SavePoint&) noexcept
    {
        return true;
    }

    template<typename Arc, typename SavePoint,
             typename K, typename V, typename ... Ts>
    static typename std::enable_if<is_char_literal<const K&>::value, bool>::type
    load_rec(Arc& arc, const SavePoint& savepoint,
            const K& key, V& val, Ts&& ... tail) noexcept
    {
        std::string k;
        if(!load(arc, k) || k != key || !load(arc, val))
        {
            arc.seek(savepoint);
            return false;
        }
        return load_rec(arc, savepoint, std::forward<Ts>(tail)...);
    }

    template<typename Arc, typename SavePoint,
             typename K, typename V, typename ... Ts>
    static typename std::enable_if<!is_char_literal<const K&>::value, bool>::type
    load_rec(Arc& arc, const SavePoint& savepoint,
             const K& key, V& val, Ts&& ... tail) noexcept
    {
        K k;
        if(!load(arc, k) || k != key || !load(arc, val))
        {
            arc.seek(savepoint);
            return false;
        }
        return load_rec(arc, savepoint, std::forward<Ts>(tail)...);
    }
};

template<>
struct save_load_util<type::ext>
{
    static std::size_t total_size_of()
    {
        return 0;
    }
    template<typename T, typename ... Ts>
    static std::size_t total_size_of(const T&, const Ts& ... args)
    {
        return sizeof(T) + total_size_of(args...);
    }

    // ------------------------------------------------------------------------

    template<typename Arc, typename ... Ts>
    static bool save_impl(Arc& arc, const std::uint8_t exttag,
                          const Ts& ... tail) noexcept
    {
        const auto savepoint = arc.npos();
        const auto len_payload = total_size_of(tail...);

        if(!save_length<type::ext>(arc, len_payload) ||
           !to_big_endian(arc, exttag) || !arc.is_writable(len_payload))
        {
            arc.seek(savepoint);
            return false;
        }
        return save_rec(arc, savepoint, tail...);
    }
    template<typename Arc, typename SavePoint>
    static bool save_rec(Arc&, const SavePoint&) noexcept
    {
        return true;
    }
    template<typename Arc, typename SavePoint, typename T, typename ... Ts>
    static bool save_rec(Arc& arc, const SavePoint& savepoint,
                         const T& head, const Ts& ... tail) noexcept
    {
        std::copy_n(reinterpret_cast<const char*>(std::addressof(head)),
                    sizeof(T), arc.sink());
        arc.advance(sizeof(T));
        return save_rec(arc, savepoint, tail...);
    }

    // ------------------------------------------------------------------------

    template<typename Arc, typename ... Ts>
    static bool load_impl(Arc& arc, const std::uint8_t exttag,
                          Ts& ... tail) noexcept
    {
        const auto savepoint = arc.npos();
        const auto len_payload = total_size_of(tail...);

        std::size_t len = 0;
        std::uint8_t  t = 0;
        if(!load_length<type::ext>(arc, len) || len != len_payload ||
           !load(arc, t) || t != exttag || !arc.is_readable(len_payload))
        {
            arc.seek(savepoint);
            return false;
        }
        return load_rec(arc, savepoint, std::forward<Ts>(tail)...);
    }

    template<typename Arc, typename SavePoint>
    static bool load_rec(Arc&, const SavePoint&) noexcept
    {
        return true;
    }
    template<typename Arc, typename SavePoint, typename T, typename ... Ts>
    static bool load_rec(Arc& arc, const SavePoint& savepoint,
                         T& head, Ts& ... tail) noexcept
    {
        std::copy_n(arc.src(), sizeof(T),
                    reinterpret_cast<char*>(std::addressof(head)));
        arc.advance(sizeof(T));
        return save_rec(arc, savepoint, tail...);
    }
};

} // detail
} // wad
#endif// WAD_IN_PLACE_HPP
