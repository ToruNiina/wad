#ifndef WAD_TEST_UTILITY_HPP
#define WAD_TEST_UTILITY_HPP
#include <vector>
#include <cstdint>

namespace wad
{
class test_write_archive
{
  public:

    std::back_insert_iterator<std::vector<std::uint8_t>>
    sink() noexcept {return std::back_inserter(buffer_);}

    bool is_writable(const std::size_t) const noexcept {return true;}

    void advance(const std::size_t) noexcept {return;}

    void retrace(const std::size_t n) noexcept
    {
        for(std::size_t i=0; i<n; ++i) {buffer_.pop_back();}
        return;
    }

    std::size_t npos() const noexcept {return buffer_.size();}

    void seek(const std::size_t n) noexcept
    {
        buffer_.erase(buffer_.begin() + n, buffer_.end());
        return ;
    }

    std::vector<std::uint8_t> const& buffer() const noexcept {return buffer_;}

  private:
    std::vector<std::uint8_t> buffer_;
};

struct test_read_archive
{
    test_read_archive(const test_write_archive& wa)
        : buffer_(wa.buffer()), iter_(buffer_.begin())
    {}

    std::vector<std::uint8_t>::const_iterator
    src() const noexcept {return iter_;}

    bool is_readable(const std::size_t len) const noexcept
    {
        const std::size_t residual = std::distance(iter_, buffer_.end());
        return len <= residual;
    }

    void advance(const std::size_t len) noexcept {iter_ += len; return ;}
    void retrace(const std::size_t len) noexcept {iter_ -= len; return ;}

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

template<typename T>
T save_load(const T& t)
{
    test_write_archive warc;
    save(warc, t);

    T retval;
    test_read_archive rarc(warc);
    load(rarc, retval);

    return retval;
}

template<typename U, typename T>
U save_load_convert(const T& t)
{
    test_write_archive warc;
    save(warc, t);

    U retval;
    test_read_archive rarc(warc);
    load(rarc, retval);

    return retval;
}


} // wad
#endif// WAD_TEST_UTILITY_HPP
