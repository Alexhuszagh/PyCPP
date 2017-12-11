//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Helpers to wrap the key-value backend.
 */

#pragma once

#include <pycpp/filesystem/path.h>
#include <pycpp/stl/functional.h>

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
//    kv_reuse_logs         = 0x0001,
//    kv_logs_mask          = 0x000F,
//#if BUILD_COMPRESSION
//    kv_zlib_compression   = 0x0010,
//    kv_bzip2_compression  = 0x0020,
//    kv_lzma_compression   = 0x0030,
//    kv_blosc_compression  = 0x0040,
//    kv_compression_mask   = 0x00F0,
//#endif  // BUILD_COMPRESSION
};

// ALIAS
// -----

using comparator_callback = std::function<void(
    const void*& s1, size_t s1len,
    const void*& s2, size_t s2len)
>;

// FUNCTIONS
// ---------

/**
 *  \brief Open key-value database.
 */
void kv_open(void* db, const path_view_t& path, kv_options options, comparator_callback cb);

/**
 *  \brief Close key-value database.
 */
void kv_close(void* db);

///**
// *  \brief Wrapper
// */
//template <typename K, typename Compare>
//void kv_comparator(const void*& s1, size_t s1len, void*& s2, size_t s2len)
//{
//    // TODO: here...
//}

PYCPP_END_NAMESPACE
