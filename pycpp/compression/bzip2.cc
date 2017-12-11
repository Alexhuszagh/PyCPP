//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#if defined(HAVE_BZIP2)

#include <pycpp/compression/bzip2.h>
#include <pycpp/compression/core.h>
#include <pycpp/preprocessor/architecture.h>
#include <pycpp/stl/stdexcept.h>
#include <bzlib.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int BZ2_SMALL = 0;
static constexpr int BZ2_BLOCK_SIZE = 9;
static constexpr int BZ2_VERBOSITY = 0;
static constexpr int BZ2_WORK_FACTOR = 30;

// Calculated using `(std::numeric_limits<T>::max() / 1.01) - 600`.
#if SYSTEM_ARCHITECTURE == 16
    static const uint16_t UNCOMPRESSED_MAX = 0xFB24ULL;
#elif SYSTEM_ARCHITECTURE == 32
    static const uint32_t UNCOMPRESSED_MAX = 0xFD771EA0ULL;
#elif SYSTEM_ARCHITECTURE == 64
    static const uint64_t UNCOMPRESSED_MAX = 0xFD7720F353A4C000ULL;
#elif SYSTEM_ARCHITECTURE == 128
    // Currently an error, assume ULLL is for 128-bit integers.
    static const uint128_t UNCOMPRESSED_MAX = 0XFD7720F353A4C0000000000000000000ULLL;
#else
#   error "Unrecognized system architecture."
#endif          // SYSTEM_ARCHITECTURE

// HELPERS
// -------

/**
 *  The maximum buffer size can be calculated by padding the buffer by
 *  1%, + 600 bytes.
 *
 *  \reference
 *      http://www.bzip.org/1.0.3/html/util-fns.html
 */
static size_t bz2_compress_bound(size_t size)
{
    if (size > UNCOMPRESSED_MAX) {
        throw std::overflow_error("Maximum compressed size would overflow size_t.");
    }

    return static_cast<size_t>((1.01 * size) + 600);
}


void check_bzstatus(int error)
{
    switch (error) {
        case BZ_OK:
        case BZ_RUN_OK:
        case BZ_FLUSH_OK:
        case BZ_FINISH_OK:
        case BZ_STREAM_END:
            return;
        case BZ_CONFIG_ERROR:
            throw compression_error(compression_config_error);
        case BZ_PARAM_ERROR:
            throw compression_error(compression_invalid_parameter);
        case BZ_MEM_ERROR:
            throw compression_error(compression_out_of_memory);
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
            throw compression_error(compression_data_error);
        case BZ_IO_ERROR:
            throw compression_error(compression_io_error);
        case BZ_UNEXPECTED_EOF:
            throw compression_error(compression_unexpected_eof);
        case BZ_SEQUENCE_ERROR:
            throw compression_error(compression_internal_error);
        default:
            throw compression_error(compression_unexpected_error);
    }
}


// OBJECTS
// -------

/**
 *  \brief Implied base class for the BZ2 compressor.
 */
struct bz2_compressor_impl: filter_impl<bz_stream>
{
    using base = filter_impl<bz_stream>;
    static const int small = BZ2_SMALL;
    static const int verbosity = BZ2_VERBOSITY;

    bz2_compressor_impl(int block_size = BZ2_BLOCK_SIZE);
    ~bz2_compressor_impl();

    virtual void call() override;
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


bz2_compressor_impl::bz2_compressor_impl(int block_size)
{
    status = BZ_OK;
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    CHECK(BZ2_bzCompressInit(&stream, block_size, verbosity, small));
}


bz2_compressor_impl::~bz2_compressor_impl()
{
    BZ2_bzCompressEnd(&stream);
}


void bz2_compressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != BZ_STREAM_END) {
        status = BZ2_bzCompress(&stream, BZ_RUN);
        check_bzstatus(status);
    }
}


bool bz2_compressor_impl::flush(void*& dst, size_t dstlen)
{
    return base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = BZ2_bzCompress(&stream, BZ_FINISH);
            return status == BZ_FINISH_OK || status == BZ_STREAM_END;
        } else {
            status = BZ2_bzCompress(&stream, BZ_FLUSH);
            return status == BZ_FLUSH_OK || status == BZ_STREAM_END;
        }
    });
}


compression_status bz2_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, BZ_STREAM_END);
}


/**
 *  \brief Implied base class for the BZ2 decompressor.
 */
struct bz2_decompressor_impl: filter_impl<bz_stream>
{
    using base = filter_impl<bz_stream>;
    static const int small = BZ2_SMALL;
    static const int verbosity = BZ2_VERBOSITY;

    bz2_decompressor_impl();
    ~bz2_decompressor_impl();

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


bz2_decompressor_impl::bz2_decompressor_impl()
{
    status = BZ_OK;
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    CHECK(BZ2_bzDecompressInit(&stream, verbosity, small));
}


bz2_decompressor_impl::~bz2_decompressor_impl()
{
    BZ2_bzDecompressEnd(&stream);
}


void bz2_decompressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != BZ_STREAM_END) {
        status = BZ2_bzDecompress(&stream);
        check_bzstatus(status);
    }
}


bool bz2_decompressor_impl::flush(void*& dst, size_t dstlen)
{
    // null-op, always flushed
    return true;
}


compression_status bz2_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, BZ_STREAM_END);
}


bz2_compressor::bz2_compressor(int compress_level):
    ptr_(new bz2_compressor_impl(compress_level))
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


void bz2_compressor::close()
{
    ptr_.reset();
}


compression_status bz2_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool bz2_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


bz2_decompressor::bz2_decompressor():
    ptr_(new bz2_decompressor_impl)
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


void bz2_decompressor::close()
{
    ptr_.reset();
}


compression_status bz2_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool bz2_decompressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}

// FUNCTIONS
// ---------


void bz2_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    // configurations
    auto block_size = BZ2_BLOCK_SIZE;
    auto verbosity = BZ2_VERBOSITY;
    auto work_factor = BZ2_WORK_FACTOR;

    unsigned int srclen_ = srclen;
    unsigned int dstlen_ = dstlen;
    if (srclen) {
        CHECK(BZ2_bzBuffToBuffCompress((char*) dst, &dstlen_, (char*) src, srclen_, block_size, verbosity, work_factor));
    } else {
        // compression no bytes
        char c = 0;
        CHECK(BZ2_bzBuffToBuffCompress((char*) dst, &dstlen_, (char*) &c, 0, block_size, verbosity, work_factor));
    }

    // update pointers
    src = ((char*) src) + srclen_;
    dst = ((char*) dst) + dstlen_;
}


std::string bz2_compress(const string_wrapper& str)
{
    size_t dstlen = bz2_compress_bound(str.size());
    return compress_bound(str, dstlen, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        bz2_compress(src, srclen, dst, dstlen);
    });
}


std::string bz2_decompress(const string_wrapper& str)
{
    return ctx_decompress<bz2_decompressor>(str);
}


void bz2_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound)
{
    // configurations
    auto small = BZ2_SMALL;
    auto verbosity = BZ2_VERBOSITY;

    unsigned int srclen_ = srclen;
    unsigned int dstlen_ = dstlen;
    if (srclen) {
        CHECK(BZ2_bzBuffToBuffDecompress((char*) dst, &dstlen_, (char*) src, srclen_, small, verbosity));
    } else {
        // compression no bytes
        char c = 0;
        CHECK(BZ2_bzBuffToBuffDecompress((char*) dst, &dstlen_, (char*) &c, 0, small, verbosity));
    }

    // update pointers
    src = ((char*) src) + srclen_;
    dst = ((char*) dst) + dstlen_;
}


std::string bz2_decompress(const string_wrapper& str, size_t bound)
{
    return decompress_bound(str, bound, [](const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound) {
        bz2_decompress(src, srclen, dst, dstlen, bound);
    });
}

PYCPP_END_NAMESPACE

#endif                  // HAVE_BZIP2
