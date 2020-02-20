# wad

serialize into msgpack.

```cpp
#include <map>
#include <wad/map.hpp>

namespace user
{
struct defined
{
    std::map<std::string, int> values;

    template<typename Archive>
    bool save(Archive& arc) const
    {
        return wad::save(this->values, arc);
    }
    template<typename Archive>
    bool load(Archive& arc)
    {
        return wad::load(this->values, arc);
    }
};
} // user

int main()
{
    std::vector<user::defined> uds = {/* ... */};

    wad::write_archive sink;
    wad::save(uds, sink);
    sink.dump("checkpoint.msg");

    // ...

    wad::read_archive src("checkpoint.msg");
    wad::load(uds, src);

    // ...

    return 0;
}
```

## Supported types

| types                | header file             |
|:---------------------|:------------------------|
| `bool`               | `wad/boolean.hpp`       |
| integers             | `wad/integer.hpp`       |
| `float` and `double` | `wad/floating.hpp`      |
| `std::array`         | `wad/array.hpp`         |
| `std::deque`         | `wad/deque.hpp`         |
| `std::string`        | `wad/string.hpp`        |
| `std::vector`        | `wad/vector.hpp`        |
| `std::map`           | `wad/map.hpp`           |
| `std::unordered_map` | `wad/unordered_map.hpp` |
| `std::pair`          | `wad/pair.hpp`          |
| `std::tuple`         | `wad/tuple.hpp`         |
| random engines       | `wad/random.hpp`        |

## Archiver requirements

### Writable archiver requiremnets

```cpp
class write_archive
{
  public:

    // Return an iterator through which the binary will be written.
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
class write_archive
{
  public:

    // Return an iterator through which the binary will be read.
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

## Licensing terms

This product is licensed under the terms of the [MIT License](LICENSE).

- Copyright (c) 2020 Toru Niina

All rights reserved.
