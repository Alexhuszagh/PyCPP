//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/zlib.h>
#include <pycpp/safe/stdlib.h>
#include <zlib.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int BUFFER_SIZE = 4096;

// MACROS
// ------

#define CHECK(EX) (void)((EX) >= 0 || (throw std::runtime_error(#EX), 0))

// HELPERS
// -------

static size_t zlib_compress_bound(size_t size)
{
    return compressBound(size);
}


void check_zstatus(int error)
{
    switch (error) {
        case Z_OK:
        case Z_STREAM_END:
        case Z_NEED_DICT:
        case Z_BUF_ERROR:
            return;
        case Z_DATA_ERROR:
            throw compression_error(compression_data_error);
        case Z_MEM_ERROR:
            throw compression_error(compression_out_of_memory);
        case Z_VERSION_ERROR:
            throw compression_error(compression_config_error);
        case Z_STREAM_ERROR:
            throw compression_error(compression_invalid_parameter);
            case Z_ERRNO:
                throw compression_error(compression_io_error);
            default:
                throw compression_error(compression_unexpected_error);
    }
}


// OBJECTS
// -------

/**
 *  \brief Implied base class for the ZLIB compressor.
 */
struct zlib_compressor_impl
{
    int status = Z_OK;
    z_stream stream;

    zlib_compressor_impl(int level = Z_DEFAULT_COMPRESSION);
    ~zlib_compressor_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void compress();
    bool flush(void*& dst, size_t dstlen);
    compression_status check_status(const void* src, void* dst) const;
    void after(void*& dst);
    void after(const void*& src, void*& dst);

    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_compressor_impl::zlib_compressor_impl(int level)
{
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
    CHECK(deflateInit(&stream, level));
}


zlib_compressor_impl::~zlib_compressor_impl()
{
    deflateEnd(&stream);
}


void zlib_compressor_impl::before(void* dst, size_t dstlen)
{
    stream.next_out = (Bytef*) dst;
    stream.avail_out = dstlen;
}


void zlib_compressor_impl::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (Bytef*) src;
    stream.avail_in = srclen;
    stream.next_out = (Bytef*) dst;
    stream.avail_out = dstlen;
}


void zlib_compressor_impl::compress()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = deflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


bool zlib_compressor_impl::flush(void*& dst, size_t dstlen)
{
    if (dst == nullptr) {
        return false;
    }
    before(dst, dstlen);
    bool code;
    if (dstlen) {
        status = deflate(&stream, Z_FINISH);
        code = status == Z_OK;
    } else {
        status = deflate(&stream, Z_FULL_FLUSH);
        code = status == Z_OK;
    }
    after(dst);

    return status;
}


compression_status zlib_compressor_impl::check_status(const void* src, void* dst) const
{
    if (status == Z_STREAM_END) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
}


void zlib_compressor_impl::after(void*& dst)
{
    dst = stream.next_out;
}


void zlib_compressor_impl::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


compression_status zlib_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    // no input data, or already reached stream end
    if (status == Z_STREAM_END) {
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
 *  \brief Implied base class for the ZLIB decompressor.
 */
struct zlib_decompressor_impl
{
    int status = Z_OK;
    z_stream stream;

    zlib_decompressor_impl();
    ~zlib_decompressor_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void decompress();
    compression_status check_status(const void* src, void* dst) const;
    void after(void*& dst);
    void after(const void*& src, void*& dst);

    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_decompressor_impl::zlib_decompressor_impl()
{
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
    CHECK(inflateInit(&stream));
}


zlib_decompressor_impl::~zlib_decompressor_impl()
{
    inflateEnd(&stream);
}


void zlib_decompressor_impl::before(void* dst, size_t dstlen)
{
    stream.next_out = (Bytef*) dst;
    stream.avail_out = dstlen;
}


void zlib_decompressor_impl::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (Bytef*) src;
    stream.avail_in = srclen;
    stream.next_out = (Bytef*) dst;
    stream.avail_out = dstlen;
}


void zlib_decompressor_impl::decompress()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = inflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


compression_status zlib_decompressor_impl::check_status(const void* src, void* dst) const
{
    if (status == Z_STREAM_END) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
}


void zlib_decompressor_impl::after(void*& dst)
{
    dst = stream.next_out;
}


void zlib_decompressor_impl::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


compression_status zlib_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    // no input data, or already reached stream end
    if (status == Z_STREAM_END) {
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


zlib_compressor::zlib_compressor(int level):
    ptr_(new zlib_compressor_impl(level))
{}


zlib_compressor::zlib_compressor(zlib_compressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


zlib_compressor & zlib_compressor::operator=(zlib_compressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


zlib_compressor::~zlib_compressor()
{}


compression_status zlib_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool zlib_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


zlib_decompressor::zlib_decompressor():
    ptr_(new zlib_decompressor_impl)
{}


zlib_decompressor::zlib_decompressor(zlib_decompressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


zlib_decompressor & zlib_decompressor::operator=(zlib_decompressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


zlib_decompressor::~zlib_decompressor()
{}


compression_status zlib_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}

// FUNCTIONS
// ---------


size_t zlib_compress(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    uLong srclen_ = srclen;
    uLong dstlen_ = dstlen;
    if (srclen) {
        CHECK(compress((Bytef*) dst, &dstlen_, (Bytef*) src, srclen_));
    } else {
        // compression no bytes
        Bytef c = 0;
        CHECK(compress((Bytef*) dst, &dstlen_, &c, 0));
    }
    return dstlen_;
}


std::string zlib_compress(const std::string &str)
{
    size_t dstlen = zlib_compress_bound(str.size());
    auto *dst = safe_malloc(dstlen);

    size_t out;
    try {
        out = zlib_compress(str.data(), str.size(), dst, dstlen);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    std::string output(reinterpret_cast<const char*>(dst), out);
    safe_free(dst);

    return output;
}

std::string zlib_decompress(const std::string &str)
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
        zlib_decompressor ctx;
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


size_t zlib_decompress(const void *src, size_t srclen, void* dst, size_t dstlen, size_t bound)
{
    uLong srclen_ = srclen;
    uLong dstlen_ = dstlen;
    if (srclen) {
        CHECK(uncompress((Bytef*) dst, &dstlen_, (Bytef*) src, srclen_));
    } else {
        // compression no bytes
        Bytef c = 0;
        CHECK(uncompress((Bytef*) dst, &dstlen_, &c, 0));
    }
    return dstlen_;
}


std::string zlib_decompress(const std::string &str, size_t bound)
{
    auto *dst = safe_malloc(bound);

    size_t out;
    try {
        out = zlib_decompress(str.data(), str.size(), dst, bound, bound);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    std::string output(reinterpret_cast<const char*>(dst), out);
    safe_free(dst);

    return output;
}


PYCPP_END_NAMESPACE
