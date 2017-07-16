//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/core.h>
#include <pycpp/compression/zlib.h>
#include <pycpp/safe/stdlib.h>
#include <zlib.h>

PYCPP_BEGIN_NAMESPACE

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
struct zlib_compressor_impl: filter_impl<z_stream>
{
    using base = filter_impl<z_stream>;
    zlib_compressor_impl(int level = Z_DEFAULT_COMPRESSION);
    ~zlib_compressor_impl();

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_compressor_impl::zlib_compressor_impl(int level)
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    CHECK(deflateInit(&stream, level));
}


zlib_compressor_impl::~zlib_compressor_impl()
{
    deflateEnd(&stream);
}


void zlib_compressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = deflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


bool zlib_compressor_impl::flush(void*& dst, size_t dstlen)
{
    return base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = deflate(&stream, Z_FINISH);
            return status == Z_STREAM_END || status == Z_OK;
        } else {
            status = deflate(&stream, Z_FULL_FLUSH);
            return status == Z_STREAM_END || status == Z_OK;
        }
    });
}


compression_status zlib_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


/**
 *  \brief Implied base class for the ZLIB decompressor.
 */
struct zlib_decompressor_impl: filter_impl<z_stream>
{
    using base = filter_impl<z_stream>;
    zlib_decompressor_impl();
    ~zlib_decompressor_impl();

    virtual void call();
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_decompressor_impl::zlib_decompressor_impl()
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    CHECK(inflateInit(&stream));
}


zlib_decompressor_impl::~zlib_decompressor_impl()
{
    inflateEnd(&stream);
}


void zlib_decompressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = inflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


compression_status zlib_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
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
