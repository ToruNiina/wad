#ifndef WAD_ENDIAN_HPP
#define WAD_ENDIAN_HPP

// Auto-detect endiannness.
//
// If it does not detect the endiannness, manually define `WAD_USE_BIG_ENDIAN`
// or `WAD_USE_LITTLE_ENDIAN` in the following way.
//
// ```console
// $ compiler -DWAD_USE_LITTLE_ENDIAN main.cpp
// ```

#if !defined(WAD_USE_BIG_ENDIAN) && !defined(WAD_USE_LITTLE_ENDIAN)
#  if defined(__GNUC__)
#    if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#      define WAD_USE_BIG_ENDIAN
#    elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#      define WAD_USE_LITTLE_ENDIAN
#    else
#      error "Wad currently supports only big or little endian."
#    endif
#  elif defined(_MSC_VER)
#    pragma message("Compiling on Windows, assuming little endian...")
#    define WAD_USE_LITTLE_ENDIAN
#  else
#    error "Unknown platform. Please define WAD_USE_(BIG|LITTLE)_ENDIAN."
#  endif
#endif

#endif// WAD_ENDIAN_HPP
