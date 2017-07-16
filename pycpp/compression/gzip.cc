//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

// This module is basically identical to zlib,
// so just include the private error handling.
#include <pycpp/byteorder.h>
#include <pycpp/compression/zlib.cc>
#include <pycpp/compression/gzip.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// TODO: there's a lot wrong with this module, need to fix it
// https://github.com/boostorg/iostreams/blob/develop/include/boost/iostreams/filter/gzip.hpp

// MACROS
// ------

#define WINDOW_BITS 15
#define GZIP_ENCODING 16

// OBJECTS
// -------

/**
 *  \brief Implied base class for the GZIP compressor.
 */
struct gzip_compressor_impl: filter_impl<z_stream>
{
    std::string header;
    uLong crc = 0;
    size_t size = 0;
    using base = filter_impl<z_stream>;
    gzip_compressor_impl(int level = 9);
    ~gzip_compressor_impl();

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


gzip_compressor_impl::gzip_compressor_impl(int level)
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    CHECK(deflateInit2(&stream, level, Z_DEFLATED, WINDOW_BITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY));
}


gzip_compressor_impl::~gzip_compressor_impl()
{
    deflateEnd(&stream);
}


void gzip_compressor_impl::call()
{
    size += stream.avail_in;
    crc = crc32(crc, stream.next_in, stream.avail_in);
    printf("New CRC is %u\n", crc);
    printf("New size is %zu\n", size);
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = deflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


bool gzip_compressor_impl::flush(void*& dst, size_t dstlen)
{
    bool code = base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = deflate(&stream, Z_FINISH);
            return status == Z_STREAM_END || status == Z_OK;
        } else {
            status = deflate(&stream, Z_FULL_FLUSH);
            return status == Z_STREAM_END || status == Z_OK;
        }
    });

    if (code && stream.avail_out >= 8) {
        char* buf = (char*) dst;
        // write CRC32
        uint32_t crc_le = htole32(crc);
        memcpy(buf, &crc_le, sizeof(uint32_t));
        buf += sizeof(uint32_t);

        // write size
        uint32_t size_le = htole32(size & 0xffffffff);
        memcpy(buf, &size_le, sizeof(uint32_t));
        buf += sizeof(uint32_t);
        dst = buf;
    }

    return code;
}


compression_status gzip_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


// TODO: need to implement the gzip decompress_impl


gzip_compressor::gzip_compressor(int level):
    ptr_(new gzip_compressor_impl(level))
{}


gzip_compressor::gzip_compressor(gzip_compressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


gzip_compressor & gzip_compressor::operator=(gzip_compressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


gzip_compressor::~gzip_compressor()
{}


compression_status gzip_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool gzip_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}

// TODO: need to implement the gzip decompressor

PYCPP_END_NAMESPACE
