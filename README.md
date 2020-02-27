# wad

**This repository is under development.**

serialize/deserialize into/from msgpack.

```cpp
#include <map>
#include <vector>
#include <string>
#include <wad.hpp>

namespace user
{
struct defined
{
    std::string         name;
    std::vector<double> values;
    std::map<std::string, int> nums;

    template<typename Archiver>
    bool archive(Archiver& arc) const
    {
        return wad::archive<wad::type::map>(arc,
            "name", name, "values", values, "nums", nums);
    }
};
} // user

int main()
{
    std::vector<user::defined> uds = {/* ... */};

    wad::write_archive sink;
    if(!wad::save(sink, uds)) {return 1;}
    sink.dump("checkpoint.msg");

    // ...

    wad::read_archive src("checkpoint.msg");
    if(!wad::load(src, uds)) {return 1;}

    // ...

    return 0;
}
```

The user-interface is strongly inspired by
[Boost.serialization](http://www.boost.org/doc/libs/release/libs/serialization/doc/index.html)
and [cereal](https://uscilab.github.io/cereal/).

## Table of Contents

- [Motivation](#motivation)
- [Supported Types](#supported-types)
- [User-defined Class](#user-defined-class)
- [Subclass](#subclass)
- [Archiver Requirements](#archiver-requirements)
- [References](#references)
- [Licensing Terms](#licensing-terms)

## Motivation

**wad** does not intend to be a general-purpose msgpack parser. It directly
serializes your class into and deserializes your class from msgpack, without
any internal representation.

**wad** does not plan to be a multi-format serializer. It uses msgpack only.

**wad** intends to be a simple, small library that enables you to serialize and
deserialize your class into/from msgpack.

## Supported Types

Basic types and STL classes listed below are supported. By including the header
file, you can save/load it into/from msgpack.

| types                 | header file             |
|:----------------------|:------------------------|
| `bool`                | `wad/boolean.hpp`       |
| integers              | `wad/integer.hpp`       |
| `float` and `double`  | `wad/floating.hpp`      |
| `std::string`         | `wad/string.hpp`        |
| `std::vector`         | `wad/vector.hpp`        |
| `std::array`          | `wad/array.hpp`         |
| `std::deque`          | `wad/deque.hpp`         |
| `std::list`           | `wad/list.hpp`          |
| `std::forward_list`   | `wad/forward_list.hpp`  |
| `std::set`            | `wad/set.hpp`           |
| `std::unordered_set`  | `wad/unordered_set.hpp` |
| `std::map`            | `wad/map.hpp`           |
| `std::unordered_map`  | `wad/unordered_map.hpp` |
| `std::pair`           | `wad/pair.hpp`          |
| `std::tuple`          | `wad/tuple.hpp`         |
| `std::unique_ptr`     | `wad/unique_ptr.hpp`    |
| RNGs (e.g. `mt19937`) | `wad/random.hpp`        |

## User-defined Class

By defining one of the following member or non-member functions,
you can serialize your class to msgpack with **wad**.

### archive() method

`archive` method works in both ways, save and load, depending on the archiver
passed.

Because it loads and changes the state of `*this` object, it must not be
marked as `const`.

```cpp
struct X
{
    std::string  a;
    double       b;
    std::int32_t c;

    template<typename Archiver>
    bool archive(Archiver& arc)
    {
        return wad::archive<wad::type::map>("a", a, "b", b, "c", c);
    }
};
```

### save() and load() methods

You can split `save` and `load` function.

Because `save` function does not change the state of `*this` object, it must be
marked as `const`. On the other hand, `load` must not be marked as `const`.

```cpp
struct X
{
    std::string  a;
    double       b;
    std::int32_t c;

    template<typename Archiver>
    bool save(Archiver& arc) const
    {
        return wad::save<wad::type::map>("a", a, "b", b, "c", c);
    }
    template<typename Archiver>
    bool load(Archiver& arc)
    {
        return wad::load<wad::type::map>("a", a, "b", b, "c", c);
    }
};
```

### Non-intrusive save() and load() methods

**wad** can find `save` and `load` function through argument dependent lookup (ADL).
By defining `save` and `load` function in your `namespace`, **wad** calls them.

```cpp
namespace foo {
struct X
{
    std::string  a;
    double       b;
    std::int32_t c;
};
template<typename Archiver>
bool save(Archiver& arc, const X& x)
{
    return wad::save<wad::type::map>("a", x.a, "b", x.b, "c", x.c);
}
template<typename Archiver>
bool load(Archiver& arc, X& x)
{
    return wad::save<wad::type::map>("a", x.a, "b", x.b, "c", x.c);
}
} // namespace foo
```

## Subclass



## Archiver requirements

You can use your own archive class in `wad::save`, `wad::load`, `wad::archive`
functions.

Note: `wad::archive(arc, ...)` function checks if `arc` has `sink()` or `src()`
method and dispatch `save(arc, ...)` or `load(arc, ...)` depending on the
provided member method. If `arc` supports both, the overload resolution becomes
ambiguous. To use `wad::archive` method with your archiver, it should have only
one of those member methods.

### Writable archiver requiremnets

```cpp
class write_archive
{
  public:
    // return type of sink.
    using sink_iterator = Iterator;

    // Return an iterator through which the binary will be written.
    // iterator value_type should be a `char`.
    Iterator sink() noexcept;

    // Check if we can write some bytes in to the buffer.
    bool is_writable(const std::size_t) const noexcept;

    // Advance the internal pointer.
    void advance(const std::size_t) noexcept;

    // Retrace the internal pointer.
    void retrace(const std::size_t) noexcept;

    // Save the current location
    Location npos() const noexcept;

    // restore the status to the passed location
    void seek(const Location&) noexcept;
};
```

### Readable archiver requiremnets

```cpp
class read_archive
{
  public:

    // return type of sink.
    using src_iterator = Iterator;

    // Return an iterator through which the binary will be read.
    // iterator value_type should be a `char`.
    Iterator src() const noexcept;

    // Check if we can read some bytes in to the buffer.
    bool is_readable(const std::size_t) const noexcept;

    // Advance the internal pointer.
    void advance(const std::size_t) noexcept;

    // Retrace the internal pointer.
    void retrace(const std::size_t) noexcept;

    // Save the current location
    Location npos() const noexcept;

    // Restore the status to the passed location
    void seek(const Location&) noexcept;
};
```

### Binding your archiver to the polymorphic loader

## Reference

## Licensing terms

This product is licensed under the terms of the [MIT License](LICENSE).

- Copyright (c) 2020 Toru Niina

All rights reserved.
