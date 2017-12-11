//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief ZLIB compression and decompression.
 */

#pragma once

#if defined(HAVE_ZLIB)

#include <pycpp/compression/exception.h>
#include <pycpp/string/string.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct zlib_compressor;
struct zlib_compressor_impl;
struct zlib_decompressor;
struct zlib_decompressor_impl;

// OBJECTS
// -------

/**
 *  \brief Wrapper for a ZLIB compressor.
 */
struct zlib_compressor
{
public:
    zlib_compressor(int compress_level = 6);
    zlib_compressor(zlib_compressor&&);
    zlib_compressor & operator=(zlib_compressor&&);
    ~zlib_compressor();
    void close();
    compression_status compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);

private:
    std::unique_ptr<zlib_compressor_impl> ptr_;
};


/**
 *  \brief Wrapper for a ZLIB decompressor.
 */
struct zlib_decompressor
{
public:
    zlib_decompressor();
    zlib_decompressor(zlib_decompressor&&);
    zlib_decompressor & operator=(zlib_decompressor&&);
    ~zlib_decompressor();
    void close();
    compression_status decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);

private:
    std::unique_ptr<zlib_decompressor_impl> ptr_;
};

// FUNCTIONS
// ---------

/**
 *  \brief ZLIB-compress data.
 */
void zlib_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief ZLIB-compress data.
 */
std::string zlib_compress(const string_wrapper& str);

/**
 *  \brief ZLIB-decompress data.
 */
std::string zlib_decompress(const string_wrapper& str);

/**
 *  \brief ZLIB-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
void zlib_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound);

/**
 *  \brief ZLIB-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
std::string zlib_decompress(const string_wrapper& str, size_t bound);

PYCPP_END_NAMESPACE

#endif                  // HAVE_ZLIB
