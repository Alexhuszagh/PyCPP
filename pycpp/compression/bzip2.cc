//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/architecture.h>
#include <pycpp/compression/bzip2.h>
#include <pycpp/safe/stdlib.h>
#include <bzlib.h>
#include <string.h>
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
static constexpr int BUFFER_SIZE = 4096;

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
struct bz2_compressor_impl
{
    int small = BZ2_SMALL;
    int verbosity = BZ2_VERBOSITY;
    int status = BZ_OK;
    bz_stream stream;

    bz2_compressor_impl(int block_size = BZ2_BLOCK_SIZE);
    ~bz2_compressor_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void compress();
    bool flush(void*& dst, size_t dstlen);
    compression_status check_status(const void* src, void* dst) const;
    void after(void*& dst);
    void after(const void*& src, void*& dst);

    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


bz2_compressor_impl::bz2_compressor_impl(int block_size)
{
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
    BZ2_CHECK(BZ2_bzCompressInit(&stream, block_size, verbosity, small));
}


bz2_compressor_impl::~bz2_compressor_impl()
{
    BZ2_bzCompressEnd(&stream);
}


void bz2_compressor_impl::before(void* dst, size_t dstlen)
{
    stream.next_out = (char*) dst;
    stream.avail_out = dstlen;
}


void bz2_compressor_impl::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (char*) src;
    stream.avail_in = srclen;
    stream.next_out = (char*) dst;
    stream.avail_out = dstlen;
}


void bz2_compressor_impl::compress()
{
    while (stream.avail_in && stream.avail_out && status != BZ_STREAM_END) {
        status = BZ2_bzCompress(&stream, BZ_RUN);
        check_bzstatus(status);
    }
}


bool bz2_compressor_impl::flush(void*& dst, size_t dstlen)
{
    if (dst == nullptr) {
        return false;
    }
    before(dst, dstlen);
    bool code;
    if (dstlen) {
        status = BZ2_bzCompress(&stream, BZ_FINISH);
        code = status == BZ_FINISH_OK;
    } else {
        status = BZ2_bzCompress(&stream, BZ_FLUSH);
        code = status == BZ_FLUSH_OK;
    }
    after(dst);

    return status;
}


compression_status bz2_compressor_impl::check_status(const void* src, void* dst) const
{
    if (status == BZ_STREAM_END) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
}


void bz2_compressor_impl::after(void*& dst)
{
    dst = stream.next_out;
}


void bz2_compressor_impl::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


compression_status bz2_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    // no input data, or already reached stream end
    if (status == BZ_STREAM_END) {
        return compression_eof;
    } else if (srclen == 0 && stream.avail_in == 0) {
        return compression_need_input;
    } else if (dst == nullptr || dstlen == 0) {
        return compression_need_output;
    }

    bool use_src = (stream.next_in == nullptr || stream.avail_in == 0);
    if (use_src) {
        before(src, srclen, dst, dstlen);
    } else {
        // have remaining input data
        before(dst, dstlen);
    }
    compress();
    compression_status code = check_status(src, dst);
    if (use_src) {
        after(src, dst);
    } else {
        after(dst);
    }

    return code;
}


/**
 *  \brief Implied base class for the BZ2 decompressor.
 */
struct bz2_decompressor_impl
{
    int small = BZ2_SMALL;
    int verbosity = BZ2_VERBOSITY;
    int status = BZ_OK;
    bz_stream stream;

    bz2_decompressor_impl();
    ~bz2_decompressor_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void decompress();
    compression_status check_status(const void* src, void* dst) const;
    void after(void*& dst);
    void after(const void*& src, void*& dst);

    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


bz2_decompressor_impl::bz2_decompressor_impl()
{
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
    BZ2_CHECK(BZ2_bzDecompressInit(&stream, verbosity, small));
}


bz2_decompressor_impl::~bz2_decompressor_impl()
{
    BZ2_bzDecompressEnd(&stream);
}


void bz2_decompressor_impl::before(void* dst, size_t dstlen)
{
    stream.next_out = (char*) dst;
    stream.avail_out = dstlen;
}


void bz2_decompressor_impl::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (char*) src;
    stream.avail_in = srclen;
    stream.next_out = (char*) dst;
    stream.avail_out = dstlen;
}


void bz2_decompressor_impl::decompress()
{
    while (stream.avail_in && stream.avail_out && status != BZ_STREAM_END) {
        status = BZ2_bzDecompress(&stream);
        check_bzstatus(status);
    }
}


compression_status bz2_decompressor_impl::check_status(const void* src, void* dst) const
{
    if (status == BZ_STREAM_END) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
}


void bz2_decompressor_impl::after(void*& dst)
{
    dst = stream.next_out;
}


void bz2_decompressor_impl::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


compression_status bz2_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    // no input data, or already reached stream end
    if (status == BZ_STREAM_END) {
        return compression_eof;
    } else if (srclen == 0 && stream.avail_in == 0) {
        return compression_need_input;
    } else if (dstlen == 0) {
        return compression_need_output;
    }

    bool use_src = (stream.next_in == nullptr || stream.avail_in == 0);
    if (use_src) {
        before(src, srclen, dst, dstlen);
    } else {
        // have remaining input data
        before(dst, dstlen);
    }
    decompress();
    compression_status code = check_status(src, dst);
    if (use_src) {
        after(src, dst);
    } else {
        after(dst);
    }

    return code;
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


compression_status bz2_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}

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
    return dstlen_;
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


std::string bzip2_decompress(const std::string &str)
{
    // configurations
    size_t dstlen = BUFFER_SIZE;
    size_t srclen = str.size();
    size_t dst_pos = 0;
    size_t src_pos = 0;
    char* buffer = (char*) safe_malloc(dstlen);
    void* dst = (void*) buffer;
    const void* src = (const void*) str.data();

    // initialize our decompression
    compression_status status = compression_ok;
    try {
        bz2_decompressor ctx;
        while (status != compression_eof) {
            dstlen *= 2;
            buffer = (char*) safe_realloc(buffer, dstlen);
            dst = (void*) (buffer + dst_pos);
            status = ctx.decompress(src, srclen - src_pos, dst, dstlen - dst_pos);
            dst_pos = std::distance(buffer, (char*) dst);
            src_pos = std::distance(str.data(), (const char*) src);
        }
    } catch (...) {
        safe_free(dst);
        throw;
    }

    // create our output string
    size_t out = std::distance(buffer, (char*) dst);
    std::string output(buffer, out);
    safe_free(buffer);

    return output;
}


size_t bzip2_decompress(const void *src, size_t srclen, void* dst, size_t dstlen, size_t bound)
{
    // configurations
    auto small = BZ2_SMALL;
    auto verbosity = BZ2_VERBOSITY;

    unsigned int srclen_ = srclen;
    unsigned int dstlen_ = dstlen;
    if (srclen) {
        BZ2_CHECK(BZ2_bzBuffToBuffDecompress((char*) dst, &dstlen_, (char*) src, srclen_, small, verbosity));
    } else {
        // compression no bytes
        char c = 0;
        BZ2_CHECK(BZ2_bzBuffToBuffDecompress((char*) dst, &dstlen_, (char*) &c, 0, small, verbosity));
    }
    return dstlen_;
}


std::string bzip2_decompress(const std::string &str, size_t bound)
{
    auto *dst = safe_malloc(bound);

    size_t out;
    try {
        out = bzip2_decompress(str.data(), str.size(), dst, bound, bound);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    std::string output(reinterpret_cast<const char*>(dst), out);
    safe_free(dst);

    return output;
}


PYCPP_END_NAMESPACE
