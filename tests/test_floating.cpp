#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "wad/floating.hpp"

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
    wad::save(t, warc);

    T retval;
    test_read_archive rarc(warc);
    wad::load(retval, rarc);

    return retval;
}

TEST_CASE( "float32 save/load", "[float]" ) {
    REQUIRE(save_load(1.0f    ) == 1.0f    );
    REQUIRE(save_load(3.1415f ) == 3.1415f );
    REQUIRE(save_load(6.02e23f) == 6.02e23f);
    REQUIRE(save_load(1.6e-19f) == 1.6e-19f);
}

TEST_CASE( "float64 save/load", "[double]" ) {
    REQUIRE(save_load(1.0    ) == 1.0    );
    REQUIRE(save_load(3.1415 ) == 3.1415 );
    REQUIRE(save_load(6.02e23) == 6.02e23);
    REQUIRE(save_load(1.6e-19) == 1.6e-19);
}
