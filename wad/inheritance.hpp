#ifndef WAD_INHERITANCE_HPP
#define WAD_INHERITANCE_HPP
#include "binary_cast.hpp"
#include "type.hpp"
#include <utility>
#include <map>
#include <functional>
#include <string>
#include <type_traits>

namespace wad
{
// To initialize Derived classes while loading Base*, we need a map
// from type-name to initializer.

namespace detail
{
template<typename Base, typename Arc>
struct subclass_loader_database
{
    // When we make this std::map a static member variable, we got SEGFAULT.
    // It seems that this will not be initialized when we access it before
    // main().
    // On the other hand, static variables in a function will be initialized
    // at the first time the function is called. By making it a static object
    // in a function, the error dissapears (at least in my environment).
    //
    // related:
    // https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use-members
    static std::map<std::string, std::function<bool(Arc&, Base*&)>>&
    database()
    {
        static std::map<std::string, std::function<bool(Arc&, Base*&)>> db;
        return db;
    }
};

template<typename Base, typename Arc>
struct subclass_saver_database
{
    // the same thing as above.
    static std::map<std::string, std::function<bool(Arc&, const Base*)>>&
    database()
    {
        static std::map<std::string, std::function<bool(Arc&, const Base*)>> db;
        return db;
    }
};

template<typename Base, typename Derived, typename Func, typename Arc>
struct subclass_loader
{
    static_assert(std::is_base_of<Base, Derived>::value, "");

    explicit subclass_loader(Func const& f): initializer(f) {}

    bool operator()(Arc& arc, Base*& ptr)
    {
        Derived derived = initializer();
        if(!load(arc, derived))
        {
            return false;
        }
        ptr = new Derived(std::move(derived));
        return true;
    }
    Func initializer;
};

template<typename Base, typename Derived, typename Arc>
struct subclass_saver
{
    static_assert(std::is_base_of<Base, Derived>::value, "");

    bool operator()(Arc& arc, const Base* ptr)
    {
        const Derived* derived = dynamic_cast<const Derived*>(ptr);
        if(!derived)
        {
            return false;
        }
        return save(arc, *derived);
    }
};
} // detail

template<typename Base, typename Derived>
struct register_subclass;

// to bind user-defined archives.
template<typename ...Arcs>
struct bind_archivers {};

template<typename Base, typename Derived>
struct registered
{
    registered(): registered([](){Derived der; return der;}) {}

    template<typename Factory>
    explicit registered(const Factory& init)
        : registered(init, bind_archivers<write_archiver, read_archiver>())
    {}

    template<typename ... Arcs>
    explicit registered(const bind_archivers<Arcs...>& arcs)
        : registered([](){Derived der; return der;}, arcs)
    {}

    template<typename Factory, typename ... Arcs>
    explicit registered(const Factory& f, const bind_archivers<Arcs...>& arcs)
    {
        register_archiver(f, arcs);
    }

  private:

    template<typename Factory>
    static void register_archiver(const Factory&, const bind_archivers<>&)
    {
        return ;
    }
    template<typename Factory, typename Arc, typename ... Arcs>
    static void register_archiver(const Factory& f,
                                 const bind_archivers<Arc, Arcs...>&)
    {
        register_save<Arc>();
        register_load<Arc>(f);
        return register_archiver(f, bind_archivers<Arcs...>());
    }

    template<typename Arc>
    static typename std::enable_if<has_sink_method<Arc>::value>::type
    register_save()
    {
        const std::string name = register_subclass<Base, Derived>::name();

        auto& db = detail::subclass_saver_database<Base, Arc>::database();
        if(db.count(name) == 0)
        {
            auto saver = detail::subclass_saver<Base, Derived, Arc>();
            db[name] = std::function<bool(Arc&, const Base*)>(std::move(saver));
        }
        return;
    }
    template<typename Arc>
    static typename std::enable_if<!has_sink_method<Arc>::value>::type
    register_save()
    {
        // If Arc does not have .sink() method, it is not a write_archiver.
        // just skip this.
        return;
    }

    template<typename Arc, typename Factory>
    static typename std::enable_if<has_src_method<Arc>::value>::type
    register_load(const Factory& f)
    {
        using Initializer = typename std::remove_cv<
            typename std::remove_reference<Factory>::type>::type;

        const std::string name = register_subclass<Base, Derived>::name();

        auto& db = detail::subclass_loader_database<Base, Arc>::database();
        if(db.count(name) == 0)
        {
            db[name] = detail::subclass_loader<Base, Derived, Initializer, Arc>(f);
        }
        return;
    }
    template<typename Arc, typename Factory>
    static typename std::enable_if<!has_src_method<Arc>::value>::type
    register_load(const Factory&)
    {
        // If Arc does not have .src() method, it is not a read_archiver.
        // just skip this.
        return;
    }
};

template<typename Base, typename Derived>
struct base_binder
{
    using base_type    = typename std::remove_cv<Base>::type;
    using derived_type = typename std::remove_cv<Derived>::type;

    static constexpr const char* name()
    {
        return register_subclass<base_type, derived_type>::name();
    }

    explicit base_binder(Derived* s): self(s) {}
    Derived* self;
};
template<typename T>
struct is_base_binder: std::false_type {};
template<typename B, typename D>
struct is_base_binder<base_binder<B, D>>: std::true_type {};
template<typename T>
struct is_base_binder<const T>: is_base_binder<T> {};
template<typename T>
struct is_base_binder<volatile T>: is_base_binder<T> {};
template<typename T>
struct is_base_binder<T&>: is_base_binder<T> {};
template<typename T>
struct is_base_binder<T&&>: is_base_binder<T> {};

template<typename Base, typename Derived>
base_binder<Base, Derived> base_of(Derived* self)
{
    return base_binder<Base, Derived>(self);
}

// internal use.
template<typename Base, typename Derived, typename Arc>
bool save(Arc& arc, const base_binder<Base, Derived>& bb)
{
    using binder = base_binder<Base, Derived>;
    const auto savepoint = arc.npos();

    // fixmap<3> {
    //     "__name__": base_binder::name(arc),
    //     "__base__": save(base),
    //     "__data__": /* later, save(derived) */
    // }

    if(!save_length<type::map>(arc, 3) ||
       !save(arc, "__name__") || !save(arc, binder::name()) ||
       !save(arc, "__base__") || !save(arc, *static_cast<const Base*>(bb.self)) ||
       !save(arc, "__data__")) // we later put Derived here.
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

// internal use.
template<typename Base, typename Derived, typename Arc>
bool load(Arc& arc, const base_binder<Base, Derived>& bb)
{
    using binder = base_binder<Base, Derived>;
    const auto savepoint = arc.npos();

    // fixmap<3> {
    //     "__name__": base_binder::name(arc),
    //     "__base__": save(base),
    //     "__data__": /* later, save(derived) */
    // }
    std::size_t len = 0;
    std::string key;
    if(!load_length<type::map>(arc, len) || len != 3 ||
       !load(arc, key) || key != "__name__"          ||
       !load(arc, key) || key != binder::name()      ||
       !load(arc, key) || key != "__base__"          ||
       !load(arc, *static_cast<Base*>(bb.self))      ||
       !load(arc, key) || key != "__data__") // we later load Derived here.
    {
        arc.seek(savepoint);
        return false;
    }
    return true;
}

} // wad
#endif// WAD_INHERITANCE_HPP
