//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/architecture.h>
#include <pycpp/compression/bzip2.h>
#include <pycpp/safe/stdlib.h>
#include <bzlib.h>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define BZ2_CHECK(EX) (void)((EX) >= 0 || (throw std::runtime_error(#EX), 0))

// CONSTANTS
// ---------

static constexpr int BZ2_SMALL = 0;
static constexpr int BZ2_BLOCK_SIZE = 9;
static constexpr int BZ2_VERBOSITY = 0;
static constexpr int BZ2_WORK_FACTOR = 30;

#if SYSTEM_ARCHITECTURE == 16
    static const uint16_t UNCOMPRESSED_MAX = 0xFB24ULL;
#elif SYSTEM_ARCHITECTURE == 32
    static const uint32_t UNCOMPRESSED_MAX = 0xFD771EA0ULL;
#elif SYSTEM_ARCHITECTURE == 64
    static const uint64_t UNCOMPRESSED_MAX = 0xFD7720F353A4C000ULL;
#else
#   error Unrecognized system architecture.
#endif          // SYSTEM_ARCHITECTURE

// HELPERS
// -------

/** The maximum buffer size can be calculated by padding the buffer by
 *  1%, + 600 bytes.
 *
 *  \reference
 *      http://www.bzip.org/1.0.3/html/util-fns.html
 */
static size_t bzip2_compress_bound(size_t size)
{
    if (size > UNCOMPRESSED_MAX) {
        throw std::overflow_error("Maximum compressed size would overflow size_t.");
    }

    return (1.01 * size) + 600;
}

// OBJECTS
// -------

/**
 *  \brief Implied base class for the BZ2 compressor.
 */
struct bz2_compressor_impl
{};


/**
 *  \brief Implied base class for the BZ2 decompressor.
 */
struct bz2_decompressor_impl
{};


bz2_compressor::bz2_compressor(int compress_level)
{}


bz2_compressor::bz2_compressor(bz2_compressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


bz2_compressor & bz2_compressor::operator=(bz2_compressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


bz2_compressor::~bz2_compressor()
{}


bz2_decompressor::bz2_decompressor()
{}


bz2_decompressor::bz2_decompressor(bz2_decompressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


bz2_decompressor & bz2_decompressor::operator=(bz2_decompressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


bz2_decompressor::~bz2_decompressor()
{}

// TODO: need to do the object definitions.
// The 1-shot kinda shit should probably be handled too...

// FUNCTIONS
// ---------


size_t bzip2_compress(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    // configurations
    auto block_size = BZ2_BLOCK_SIZE;
    auto verbosity = BZ2_VERBOSITY;
    auto work_factor = BZ2_WORK_FACTOR;

    unsigned int srclen_ = srclen;
    unsigned int dstlen_ = dstlen;
    if (srclen) {
        BZ2_CHECK(BZ2_bzBuffToBuffCompress((char*) dst, &dstlen_, (char*) src, srclen_, block_size, verbosity, work_factor));
    } else {
        // compression no bytes
        char c = 0;
        BZ2_CHECK(BZ2_bzBuffToBuffCompress((char*) dst, &dstlen_, (char*) &c, 0, block_size, verbosity, work_factor));
    }
    return dstlen;
}


std::string bzip2_compress(const std::string &str)
{
    size_t dstlen = bzip2_compress_bound(str.size());
    auto *dst = safe_malloc(dstlen);

    size_t out;
    try {
        out = bzip2_compress(str.data(), str.size(), dst, dstlen);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    std::string output(reinterpret_cast<const char*>(dst), out);
    safe_free(dst);

    return output;
}


// TODO: implement...
///** \brief BZIP2-decompress data. Returns number of bytes converted.
// */
//size_t bzip2_decompress(const void *src, size_t srclen, void* dst, size_t dstlen);
//
///** \brief BZIP2-decompress data.
// */
//std::string bzip2_decompress(const std::string &str);
//
///** \brief BZIP2-decompress data. Returns number of bytes converted.
// *
// *  \param bound            Known size of decompressed buffer.
// */
//size_t bzip2_decompress(const void *src, size_t srclen, void* dst, size_t dstlen, size_t bound);
//
///** \brief BZIP2-decompress data.
// *
// *  \param bound            Known size of decompressed buffer.
// */
//std::string bzip2_decompress(const std::string &str, size_t bound);


PYCPP_END_NAMESPACE
