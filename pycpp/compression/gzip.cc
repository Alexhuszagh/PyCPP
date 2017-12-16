//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#if defined(HAVE_ZLIB)

// This module is basically identical to zlib,
// so just include the private error handling.
#include <pycpp/compression/zlib.cc>
#include <pycpp/compression/gzip.h>
#include <pycpp/preprocessor/byteorder.h>
#include <string.h>
#include <time.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define WINDOW_BITS 15

// HELPERS
// -------


static size_t gzip_compress_bound(size_t size)
{
    // need an extra 10 bytes for the header
    return zlib_compress_bound(size) + 10;
}

/**
 *  Create the GZIP header. Use a default filetime, filename,
 *  and comment by default, since these features aren't useful,
 *  and just mess up reproducibility of the produced bytes
 *  for testing purposes.
 */
static std::string gzip_header(int level,
    time_t mtime = 0,
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

    // flags
    int flags = 0;
    if (has_filename) {
        flags += 8;
    }
    if (has_comment) {
        flags += 16;
    }
    header += (char) flags;

    // mtime
    uint32_t mtime_le = htole32(static_cast<uint32_t>(mtime));
    header.append((char*) &mtime_le, sizeof(uint32_t));

    // write remaining header
    if (level == Z_BEST_COMPRESSION) {
        header += (char) 0x02;              /*  extra flags */
    } else if (level == Z_BEST_SPEED) {
        header += (char) 0x04;              /*  extra flags */
    } else {
        header += (char) 0x00;              /*  extra flags */
    }
    header += (char) 0xff;                  /*  OS unknown  */

    // write filename and comments
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
    using base = filter_impl<z_stream>;

    std::string header;
    uLong crc = 0;
    size_t size = 0;

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
    if (code) {
        write_footer(dst);
    }

    return code;
}


compression_status gzip_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


/**
 *  \brief Implied base class for the GZIP decompressor.
 */
struct gzip_decompressor_impl: filter_impl<z_stream>
{
    using base = filter_impl<z_stream>;

    bool header_done = false;
    uLong crc = 0;
    size_t size = 0;

    gzip_decompressor_impl();
    ~gzip_decompressor_impl();

    void read_header();
    void read_footer();
    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


gzip_decompressor_impl::gzip_decompressor_impl()
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    CHECK(inflateInit2(&stream, -WINDOW_BITS));
}


gzip_decompressor_impl::~gzip_decompressor_impl()
{
    inflateEnd(&stream);
}


static void read_string(z_stream& stream)
{
    // no filename nor comment
    void* tmp = memchr(stream.next_in, 0, stream.avail_in);
    stream.avail_in -= distance(stream.next_in, (Bytef*) tmp);
    if (!stream.avail_in) {
        throw runtime_error("Unable to read header.");
    }
    --stream.avail_in;
    ++stream.next_in;
}


void gzip_decompressor_impl::read_header()
{
    if (!header_done && stream.avail_in >= 10) {
        // read our header
        char flags = stream.next_in[3];
        stream.next_in += 10;
        stream.avail_in -= 10;
        if (flags == 8 || flags == 16) {
            // has flag or comment but not both
            read_string(stream);
        } else if (flags == 24) {
            // has both filename and comment
            read_string(stream);
            read_string(stream);
        }

        header_done = true;
    }
}


void gzip_decompressor_impl::read_footer()
{
    if (status == Z_STREAM_END && stream.avail_in >= 8) {
        uint32_t* buf = (uint32_t*) stream.next_in;
        uint32_t crc_ = le32toh(*buf++);
        uint32_t size_ = le32toh(*buf++);

        if (crc_ != crc) {
            throw runtime_error("CRC mismatch in GZIP decompression.");
        }
        if (size_ != (size & 0xffffffff)) {
            throw runtime_error("Size mismatch in GZIP decompression.");
        }
    }
}


void gzip_decompressor_impl::call()
{
    read_header();
    if (!header_done) {
        return;
    }

    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        Bytef* dst = stream.next_out;
        status = inflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);

        // store out CRC and length information
        size_t length = distance(dst, stream.next_out);
        size += length;
        crc = crc32(crc, dst, length);
    }

    read_footer();
}


bool gzip_decompressor_impl::flush(void*& dst, size_t dstlen)
{
    // null-op, always flushed
    return true;
}


compression_status gzip_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


gzip_compressor::gzip_compressor(int level):
    ptr_(new gzip_compressor_impl(level))
{}


gzip_compressor::gzip_compressor(gzip_compressor&& rhs):
    ptr_(move(rhs.ptr_))
{}


gzip_compressor & gzip_compressor::operator=(gzip_compressor&& rhs)
{
    swap(ptr_, rhs.ptr_);
    return *this;
}


gzip_compressor::~gzip_compressor()
{}


void gzip_compressor::close()
{
    ptr_.reset();
}


compression_status gzip_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool gzip_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


gzip_decompressor::gzip_decompressor():
    ptr_(new gzip_decompressor_impl)
{}


gzip_decompressor::gzip_decompressor(gzip_decompressor&& rhs):
    ptr_(move(rhs.ptr_))
{}


gzip_decompressor & gzip_decompressor::operator=(gzip_decompressor&& rhs)
{
    swap(ptr_, rhs.ptr_);
    return *this;
}


gzip_decompressor::~gzip_decompressor()
{}


void gzip_decompressor::close()
{
    ptr_.reset();
}


compression_status gzip_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool gzip_decompressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}

// FUNCTIONS
// ---------


void gzip_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    gzip_compressor ctx;
    ctx.compress(src, srclen, dst, dstlen);
    ctx.flush(dst, dstlen);
}


std::string gzip_compress(const string_wrapper& str)
{
    size_t dstlen = gzip_compress_bound(str.size());
    return compress_bound(str, dstlen, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        gzip_compress(src, srclen, dst, dstlen);
    });
}


std::string gzip_decompress(const string_wrapper& str)
{
    return ctx_decompress<gzip_decompressor>(str);
}


void gzip_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound)
{
    gzip_decompressor ctx;
    ctx.decompress(src, srclen, dst, dstlen);
}


std::string gzip_decompress(const string_wrapper& str, size_t bound)
{
    return decompress_bound(str, bound, [](const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound) {
        gzip_decompress(src, srclen, dst, dstlen, bound);
    });
}

PYCPP_END_NAMESPACE

#endif                  // HAVE_ZLIB
