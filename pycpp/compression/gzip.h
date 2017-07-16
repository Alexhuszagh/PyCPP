//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief GZIP compression and decompression.
 */

#pragma once

#include <pycpp/compression/exception.h>
#include <memory>
#include <string>

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
    gzip_compressor(int compress_level = 6);
    gzip_compressor(gzip_compressor&&);
    gzip_compressor & operator=(gzip_compressor&&);
    ~gzip_compressor();
    compression_status compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);

private:
    std::unique_ptr<gzip_compressor_impl> ptr_;
};


/**
 *  \brief Wrapper for a GZIP decompressor.
 */
struct gzip_decompressor
{
public:
    gzip_decompressor();
    gzip_decompressor(gzip_decompressor&&);
    gzip_decompressor & operator=(gzip_decompressor&&);
    ~gzip_decompressor();
    compression_status decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

private:
    std::unique_ptr<gzip_decompressor_impl> ptr_;
};

// FUNCTIONS
// ---------

/** \brief GZIP-compress data. Returns number of bytes converted.
 */
size_t gzip_compress(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief GZIP-compress data.
 */
std::string gzip_compress(const std::string &str);

/** \brief GZIP-decompress data.
 */
std::string gzip_decompress(const std::string &str);

/** \brief GZIP-decompress data. Returns number of bytes converted.
 *
 *  \param bound            Known size of decompressed buffer.
 */
size_t gzip_decompress(const void *src, size_t srclen, void* dst, size_t dstlen, size_t bound);

/** \brief GZIP-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
std::string gzip_decompress(const std::string &str, size_t bound);


PYCPP_END_NAMESPACE
