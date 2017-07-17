//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#if defined(HAVE_BLOSC)

#include <pycpp/architecture.h>
#include <pycpp/compression/blosc.h>
#include <pycpp/compression/core.h>
#include <blosc.h>
#include <algorithm>
#include <cstring>
#include <thread>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// CONSTANTS
// ---------

static const int THREADS = std::min<int>(4, std::max<int>(1, std::thread::hardware_concurrency()));
static const int PADDING = BLOSC_MAX_OVERHEAD + 4 * THREADS;
static const int CLEVEL = 5;
static const int DOSHUFFLE = BLOSC_DOSHUFFLE;
static const char* COMPNAME = BLOSC_BLOSCLZ_COMPNAME;
static const size_t TYPESIZE = 8;
static const size_t BLOCKSIZE = 0;

#if SYSTEM_ARCHITECTURE == 16
    static const uint16_t UNCOMPRESSED_MAX = std::numeric_limits<uint16_t>::max() - PADDING;
#elif SYSTEM_ARCHITECTURE == 32
    static const uint32_t UNCOMPRESSED_MAX = std::numeric_limits<uint32_t>::max() - PADDING;
#elif SYSTEM_ARCHITECTURE == 64
    static const uint64_t UNCOMPRESSED_MAX = std::numeric_limits<uint64_t>::max() - PADDING;
#else
#   error Unrecognized system architecture.
#endif          // SYSTEM_ARCHITECTURE

// HELPERS
// -------

static size_t blosc_compress_bound(size_t size)
{
    if (size > UNCOMPRESSED_MAX) {
        throw std::overflow_error("Maximum compressed size would overflow size_t.");
    }

    return size + PADDING;
}

// FUNCTIONS
// ---------

void blosc_compress(const void*& src, size_t srclen, void* &dst, size_t dstlen)
{
    // configurations
    static const int clevel = CLEVEL;
    static const int doshuffle = DOSHUFFLE;
    static const int typesize = TYPESIZE;
    static const char* compressor = COMPNAME;
    static const int blocksize = BLOCKSIZE;
    static const int threads = THREADS;

    // compress bytes
    int dstlen_ = dstlen;
    if (srclen) {
        dstlen_ = blosc_compress_ctx(clevel, doshuffle, typesize, srclen, src, dst, dstlen, compressor, blocksize, threads);
    } else {
        char c = 0;
        dstlen_ = blosc_compress_ctx(clevel, doshuffle, typesize, 0, (void*) &c, dst, dstlen, compressor, blocksize, threads);
    }
    CHECK(dstlen_);

    // update pointers
    src = ((char*) src) + srclen;
    dst = ((char*) dst) + dstlen_;
}


std::string blosc_compress(const std::string &str)
{
    size_t dstlen = blosc_compress_bound(str.size());
    return compress_bound(str, dstlen, [](const void*& src, size_t srclen, void* &dst, size_t dstlen) {
        return blosc_compress(src, srclen, dst, dstlen);
    });
}


std::string blosc_decompress(const std::string &str)
{
    if (str.size() < BLOSC_MIN_HEADER_LENGTH) {
        return "";
    }

    // extract number of bytes from the header
    size_t nbytes, cbytes, blocksize;
    blosc_cbuffer_sizes((void*) str.data(), &nbytes, &cbytes, &blocksize);

    // check if the header data doesn't match experimental
    if (str.size() != cbytes) {
        return "";
    }

    return blosc_decompress(str, nbytes);
}


void blosc_decompress(const void*& src, size_t srclen, void* &dst, size_t dstlen, size_t bound)
{
    // configurations
    static const int threads = THREADS;

    // decompress bytes
    int dstlen_ = dstlen;
    if (srclen) {
        dstlen_ = blosc_decompress_ctx(src, dst, dstlen_, threads);
    } else {
        dstlen_ = 0;
    }
    CHECK(dstlen_);

    // update pointers
    src = ((char*) src) + srclen;
    dst = ((char*) dst) + dstlen_;
}


std::string blosc_decompress(const std::string &str, size_t bound)
{
    return decompress_bound(str, bound, [](const void*& src, size_t srclen, void* &dst, size_t dstlen, size_t bound) {
        blosc_decompress(src, srclen, dst, dstlen, bound);
    });
}

PYCPP_END_NAMESPACE

#endif                  // HAVE_BLOSC
