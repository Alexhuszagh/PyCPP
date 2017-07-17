//  :copyright: (c) 2003-2007 Jonathan Turkanis.
//  :copyright: (c) 2008 CodeRage, LLC (turkanis at coderage dot com).
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

// This module is basically identical to zlib,
// so just include the private error handling.
#include <pycpp/byteorder.h>
#include <pycpp/compression/zlib.cc>
#include <pycpp/compression/gzip.h>
#include <cstring>
#include <ctime>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define WINDOW_BITS 15
#define GZIP_ENCODING 16

// HELPERS
// -------

static std::string gzip_header(int level,
    std::time_t mtime = 0,
    const std::string& filename = "",
    const std::string& comment = "")
{
    std::string header;
    bool has_filename = !filename.empty();
    bool has_comment = !comment.empty();
    size_t length = 10;
    if (has_filename) {
        length += filename.size() + 1;
    }
    if (has_comment) {
        length += comment.size() + 1;
    }
    header.reserve(length);

    header += (char) 0x1f;                  /* magic number */
    header += (char) 0x8b;                  /* magic number */
    header += (char) 0x08;                  /*    deflate   */
    header += (char) 0x00;                  /*     flags    */

    // mtime
    uint32_t mtime_le = htole32(mtime);
    header.append((char*) &mtime_le, sizeof(uint32_t));

    if (level == Z_BEST_COMPRESSION) {
        header += (char) 0x02;              /*  extra flags */
    } else if (level == Z_BEST_SPEED) {
        header += (char) 0x04;              /*  extra flags */
    } else {
        header += (char) 0x00;              /*  extra flags */
    }
    header += (char) 0xff;                  /*  OS unknown  */

    if (has_filename) {
        header += filename;
        header += (char) 0x00;
    }
    if (has_comment) {
        header += comment;
        header += (char) 0x00;
    }

    return header;
}

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

    void write_header();
    void write_footer(void*& dst);
    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


gzip_compressor_impl::gzip_compressor_impl(int level)
{
    header = gzip_header(level);

    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    CHECK(deflateInit2(&stream, level, Z_DEFLATED, -WINDOW_BITS, 8, Z_DEFAULT_STRATEGY));
    // TODO: inflateInit2 will use -WINDOW_BITS
    // inflateInit2(s, window_bits)
}


gzip_compressor_impl::~gzip_compressor_impl()
{
    deflateEnd(&stream);
}


void gzip_compressor_impl::write_header()
{
    // write header on first pass
    if (!header.empty() && header.size() < stream.avail_out) {
        memcpy(stream.next_out, header.data(), header.size());
        stream.next_out += header.size();
        stream.avail_out -= header.size();
        header.clear();
    }
}


void gzip_compressor_impl::write_footer(void*& dst)
{
    if (status == Z_STREAM_END && stream.avail_out >= 8) {
        // write CRC32
        uint32_t crc_le = htole32(crc);
        memcpy(stream.next_out, &crc_le, sizeof(uint32_t));
        stream.next_out += sizeof(uint32_t);

        // write size
        uint32_t size_le = htole32(size & 0xffffffff);
        memcpy(stream.next_out, &size_le, sizeof(uint32_t));
        stream.next_out += sizeof(uint32_t);
        after(dst);
    }
}


void gzip_compressor_impl::call()
{
    // try to write header, and if cannot, return early
    write_header();
    if (!header.empty()) {
        return;
    }

    size += stream.avail_in;
    crc = crc32(crc, stream.next_in, stream.avail_in);
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = deflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


bool gzip_compressor_impl::flush(void*& dst, size_t dstlen)
{
    // try to write header, and if cannot, return early
    write_header();
    if (!header.empty()) {
        return false;
    }

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
    write_footer(dst);

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
