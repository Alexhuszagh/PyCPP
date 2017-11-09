//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Key-value database cache.
 */

#pragma once

#include <pycpp/cache/kv_backend.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Flags for key-value database options.
 *
 *  Options:
 *      kv_none:                Use default parameters.
 *      kv_reuse_logs:          Re-use logs to speed up database open times.
 *      kv_zlib_compression:    Use ZLIB compression to encode values.
 *      kv_bzip2_compression:   Use BZIP2 compression to encode values.
 *      kv_xz_compression:      Use LZMA2 compression to encode values.
 *      kv_blosc_compression:   Use BLOSC compression to encode values.
 */
enum kv_options
{
    kv_none               = 0x0000,
    kv_reuse_logs         = 0x0001,
#if BUILD_COMPRESSION
    kv_zlib_compression   = 0x0010,
    kv_bzip2_compression  = 0x0020,
    kv_lzma_compression   = 0x0030,
    kv_blosc_compression  = 0x0040,
#endif  // BUILD_COMPRESSION
};


/**
 *  \brief STL-like wrapper around a key-value database iterator.
 */
template <typename T>
struct kv_iterator: std::iterator<std::input_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, T> base_t;
    typedef kv_iterator self_t;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

// TODO: need everything
};


/**
 *  \brief Map-like wrapper around a key-value database store.
 */
template <typename Key, typename T>
struct kv_cache
{
public:
    // MEMBER TYPES
    // ------------
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = kv_iterator<value_type>;
    using const_iterator = kv_iterator<const value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // MEMBER FUNCTIONS
    // ----------------
    kv_cache(kv_options = kv_none);

// TODO: need everything
};


// IMPLEMENTATION
// --------------

PYCPP_END_NAMESPACE
