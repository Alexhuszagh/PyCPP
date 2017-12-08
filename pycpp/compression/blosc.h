//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief BLOSC compression and decompression.
 */

#pragma once

#if defined(HAVE_BLOSC)

#include <pycpp/compression/exception.h>
#include <pycpp/view/string.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief BLOSC-compress data. Returns number of bytes converted.
 */
void blosc_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief BLOSC-compress data.
 */
std::string blosc_compress(const string_view& str);

/**
 *  \brief BLOSC-decompress data.
 */
std::string blosc_decompress(const string_view& str);

/**
 *  \brief BLOSC-decompress data. Returns number of bytes converted.
 *
 *  \param bound            Known size of decompressed buffer.
 */
void blosc_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound);

/**
 *  \brief BLOSC-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
std::string blosc_decompress(const string_view& str, size_t bound);

PYCPP_END_NAMESPACE

#endif                  // HAVE_BLOSC
