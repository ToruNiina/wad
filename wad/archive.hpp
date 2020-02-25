#ifndef WAD_ARCHIVE_HPP
#define WAD_ARCHIVE_HPP
#include "type_traits.hpp"
#include <vector>
#include <iterator>
#include <fstream>
#include <cstdint>

namespace wad
{

class write_archive
{
  public:
    using sink_iterator = std::back_insert_iterator<std::vector<std::uint8_t>>;

    // return an iterator through which the binary will be written.
    //
    // use back_insert_iterator to write into this archive.
    std::back_insert_iterator<std::vector<std::uint8_t>>
    sink() noexcept {return std::back_inserter(buffer_);}

    // Check if we can write some bytes in to the buffer.
    //
    // It is true, regardless of the size to be written.
    // Since the internal buffer is just a std::vector, we can anytime
    // push_back to it.
    bool is_writable(const std::size_t) const noexcept {return true;}

    // Advance the internal pointer.
    //
    // It does nothing. Because sink() iterator always points to the
    // last element of the buffer_.
    void advance(const std::size_t) noexcept {return;}

    // Retrace the internal pointer.
    //
    // It pops the last element. `sink()` iterator always points to the last
    // element of the buffer.
    void retrace(const std::size_t n) noexcept
    {
        for(std::size_t i=0; i<n; ++i) {buffer_.pop_back();}
        return;
    }

    // write the current buffer into the file.
    void dump(const std::string& filename) const
    {
        std::ofstream ofs(filename);
        if(!ofs.good())
        {
            throw std::runtime_error("[error] file open error: " + filename);
        }
        ofs.write(reinterpret_cast<const char*>(buffer_.data()),
                  buffer_.size());
        return ;
    }

    // save the current position.
    std::size_t npos() const noexcept {return buffer_.size();}

    void seek(const std::size_t n) noexcept
    {
        buffer_.erase(buffer_.begin() + n, buffer_.end());
        return ;
    }

  private:
    std::vector<std::uint8_t> buffer_;
};

class read_archive
{
  public:
    using src_iterator = std::vector<std::uint8_t>::const_iterator;

    explicit read_archive(const std::string& filename)
    {
        std::ifstream ifs(filename);
        if(!ifs.good())
        {
            throw std::runtime_error("[error] file open error: " + filename);
        }

        // count file size
        const auto beg = ifs.tellg();
        ifs.seekg(0, std::ios::end);
        const auto end = ifs.tellg();
        const auto file_size = end - beg;
        ifs.seekg(beg);

        this->buffer_.resize(file_size);
        ifs.read(reinterpret_cast<char*>(buffer_.data()), file_size);

        this->iter_ = this->buffer_.begin();
    }
    ~read_archive() = default;

    // return an iterator through which we can read some bytes from the buffer.
    //
    // We need to store the current "last" element in the buffer because it
    // first load all the file content to the memory, in this implementation.
    std::vector<std::uint8_t>::const_iterator
    src() const noexcept {return iter_;}

    // Check if we can read some bytes from the buffer.
    //
    // It basically checks distance from pointer to the end.
    bool is_readable(const std::size_t len) const noexcept
    {
        const std::size_t residual = std::distance(iter_, buffer_.end());
        return len <= residual;
    }

    // Advance the internal pointer.
    void advance(const std::size_t len) noexcept {iter_ += len; return ;}
    // Retrace the internal pointer.
    void retrace(const std::size_t len) noexcept {iter_ -= len; return ;}

    // save the current position.
    std::vector<std::uint8_t>::const_iterator npos() const noexcept {return iter_;}

    void seek(const std::vector<std::uint8_t>::const_iterator it) noexcept
    {
        iter_ = it;
        return ;
    }

  private:

    std::vector<std::uint8_t> buffer_;
    std::vector<std::uint8_t>::const_iterator iter_;
};

static_assert( has_sink_method<write_archive>::value, "");
static_assert(!has_src_method <write_archive>::value, "");
static_assert(!has_sink_method<read_archive >::value, "");
static_assert( has_src_method <read_archive >::value, "");

template<typename Arc, typename T>
// arc.sink() is valid, arc.src() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_sink_method<Arc>, detail::negation<has_src_method<Arc>>
    detail::negation<has_write_archive_method<T>>,
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return save(arc, arg);
}
template<typename Arc, typename T>
// arc.src() is valid, arc.sink() is not valid, & arg.archive(arc) isn't defined
typename std::enable_if<detail::conjunction<
    has_src_method<Arc>, detail::negation<has_sink_method<Arc>>
    detail::negation<has_read_archive_method<T>>,
    >::value, bool>::type
archive(Arc& arc, T& arg)
{
    return load(arc, arg);
}

} // wad
#endif//WAD_ARCHIVE_HPP
