//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief BZIP2 compression and decompression.
 */

#pragma once

#if defined(HAVE_BZIP2)

#include <pycpp/compression/exception.h>
#include <memory>
#include <string>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct bz2_compressor;
struct bz2_compressor_impl;
struct bz2_decompressor;
struct bz2_decompressor_impl;

// OBJECTS
// -------

/**
 *  \brief Wrapper for a BZIP2 compressor.
 */
struct bz2_compressor
{
public:
    bz2_compressor(int compress_level = 9);
    bz2_compressor(bz2_compressor&&);
    bz2_compressor & operator=(bz2_compressor&&);
    ~bz2_compressor();
    compression_status compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);

private:
    std::unique_ptr<bz2_compressor_impl> ptr_;
};


/**
 *  \brief Wrapper for a BZIP2 decompressor.
 */
struct bz2_decompressor
{
public:
    bz2_decompressor();
    bz2_decompressor(bz2_decompressor&&);
    bz2_decompressor & operator=(bz2_decompressor&&);
    ~bz2_decompressor();
    compression_status decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);

private:
    std::unique_ptr<bz2_decompressor_impl> ptr_;
};

// FUNCTIONS
// ---------

/** \brief BZIP2-compress data.
 */
void bz2_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/** \brief BZIP2-compress data.
 */
std::string bz2_compress(const std::string &str);

/** \brief BZIP2-decompress data.
 */
std::string bz2_decompress(const std::string &str);

/** \brief BZIP2-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
void bz2_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound);

/** \brief BZIP2-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
std::string bz2_decompress(const std::string &str, size_t bound);

PYCPP_END_NAMESPACE

#endif                  // HAVE_BZIP2
