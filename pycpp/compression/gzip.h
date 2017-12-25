//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief GZIP compression and decompression.
 */

#pragma once

#if defined(HAVE_ZLIB)

#include <pycpp/compression/exception.h>
#include <pycpp/stl/memory.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct gzip_compressor;
struct gzip_compressor_impl;
struct gzip_decompressor;
struct gzip_decompressor_impl;

// OBJECTS
// -------

/**
 *  \brief Wrapper for a GZIP compressor.
 */
struct gzip_compressor
{
public:
    gzip_compressor(int compress_level = 9);
    gzip_compressor(gzip_compressor&&) noexcept;
    gzip_compressor & operator=(gzip_compressor&&) noexcept;
    ~gzip_compressor() noexcept;

    compression_status compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);
    void close() noexcept;
    void swap(gzip_compressor&) noexcept;

private:
    unique_ptr<gzip_compressor_impl> ptr_;
};


/**
 *  \brief Wrapper for a GZIP decompressor.
 */
struct gzip_decompressor
{
public:
    gzip_decompressor();
    gzip_decompressor(gzip_decompressor&&) noexcept;
    gzip_decompressor & operator=(gzip_decompressor&&) noexcept;
    ~gzip_decompressor() noexcept;

    compression_status decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);
    void close() noexcept;
    void swap(gzip_decompressor&) noexcept;

private:
    unique_ptr<gzip_decompressor_impl> ptr_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<gzip_compressor>: is_relocatable<unique_ptr<gzip_compressor_impl>>
{};

template <>
struct is_relocatable<gzip_decompressor>: is_relocatable<unique_ptr<gzip_decompressor_impl>>
{};

// FUNCTIONS
// ---------

/**
 *  \brief GZIP-compress data. Returns number of bytes converted.
 */
void gzip_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief GZIP-compress data.
 */
string gzip_compress(const string_wrapper& str);

/**
 *  \brief GZIP-decompress data.
 */
string gzip_decompress(const string_wrapper& str);

/**
 *  \brief GZIP-decompress data. Returns number of bytes converted.
 *
 *  \param bound            Known size of decompressed buffer.
 */
void gzip_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound);

/**
 *  \brief GZIP-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
string gzip_decompress(const string_wrapper& str, size_t bound);

PYCPP_END_NAMESPACE

#endif                  // HAVE_ZLIB
