//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/stream.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Callback function for compression.
 */
#define COMPRESS_CALLBACK                                                                                       \
    [this](const void*& src, size_t srclen, void*& dst,                                                         \
           size_t dstlen, size_t char_size)                                                                     \
    {                                                                                                           \
        if (srclen) {                                                                                           \
            ctx.compress(src, srclen, dst, dstlen);                                                             \
        } else {                                                                                                \
            ctx.flush(dst, dstlen);                                                                             \
        }                                                                                                       \
    }


/**
 *  \brief Callback function for decompression.
 */
#define DECOMPRESS_CALLBACK                                                                                     \
    [this](const void*& src, size_t srclen, void*& dst,                                                         \
           size_t dstlen, size_t char_size)                                                                     \
    {                                                                                                           \
        if (srclen) {                                                                                           \
            ctx.decompress(src, srclen, dst, dstlen);                                                           \
        } else {                                                                                                \
            ctx.flush(dst, dstlen);                                                                             \
        }                                                                                                       \
    }


/**
 *  \brief Provides wide-path overloads for Windows.
 */
#if defined(PYCPP_HAVE_WFOPEN)              // WINDOWS

#   define WIDE_PATH_IFSTREAM(name)                                                                             \
                                                                                                                \
        name##_ifstream::name##_ifstream(const std::wstring &name, std::ios_base::openmode mode)                \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ifstream::open(const std::wstring &name, std::ios_base::openmode mode)                      \
        {                                                                                                       \
            filter_ifstream::open(name, mode, DECOMPRESS_CALLBACK);                                             \
        }

#   define WIDE_PATH_OFSTREAM(name)                                                                             \
                                                                                                                \
        name##_ofstream::name##_ofstream(const std::wstring &name, std::ios_base::openmode mode)                \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        name##_ofstream::name##_ofstream(const std::wstring &name, int level, std::ios_base::openmode mode):    \
            ctx(level)                                                                                          \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ofstream::open(const std::wstring &name, std::ios_base::openmode mode)                      \
        {                                                                                                       \
            filter_ofstream::open(name, mode, COMPRESS_CALLBACK);                                               \
        }

#else                                       // POSIX

#   define WIDE_PATH_IFSTREAM(name)
#   define WIDE_PATH_OFSTREAM(name)

#endif                                      // WINDOWS


/**
 *  \brief Macro to define methods for a filtering istream base.
 */
#define COMPRESSED_ISTREAM(name)                                                                                \
    name##_istream::name##_istream()                                                                            \
    {}                                                                                                          \
                                                                                                                \
    name##_istream::name##_istream(std::istream& stream)                                                        \
    {                                                                                                           \
        open(stream);                                                                                           \
    }                                                                                                           \
                                                                                                                \
    name##_istream::~name##_istream()                                                                           \
    {                                                                                                           \
        filter_istream::close();                                                                                \
        rdbuf()->set_callback(nullptr);                                                                         \
        ctx.close();                                                                                            \
    }                                                                                                           \
                                                                                                                \
    void name##_istream::open(std::istream& stream)                                                             \
    {                                                                                                           \
        filter_istream::open(stream, DECOMPRESS_CALLBACK);                                                      \
    }                                                                                                           \
                                                                                                                \
    name##_istream::name##_istream(name##_istream&& rhs):                                                       \
        ctx(std::move(rhs.ctx)),                                                                                \
        filter_istream(std::move(rhs))                                                                          \
    {}                                                                                                          \
                                                                                                                \
    name##_istream & name##_istream::operator=(name##_istream&& rhs)                                            \
    {                                                                                                           \
        ctx = std::move(rhs.ctx);                                                                               \
        filter_istream::operator=(std::move(rhs));                                                              \
        return *this;                                                                                           \
    }


/**
 *  \brief Macro to define methods for a filtering ostream base.
 */
#define COMPRESSED_OSTREAM(name)                                                                                \
    name##_ostream::name##_ostream()                                                                            \
    {}                                                                                                          \
                                                                                                                \
    name##_ostream::name##_ostream(int level):                                                                  \
        ctx(level)                                                                                              \
    {}                                                                                                          \
                                                                                                                \
    name##_ostream::name##_ostream(std::ostream& stream)                                                        \
    {                                                                                                           \
        open(stream);                                                                                           \
    }                                                                                                           \
                                                                                                                \
    name##_ostream::name##_ostream(std::ostream& stream, int level):                                            \
        ctx(level)                                                                                              \
    {                                                                                                           \
        open(stream);                                                                                           \
    }                                                                                                           \
                                                                                                                \
    name##_ostream::~name##_ostream()                                                                           \
    {                                                                                                           \
        filter_ostream::close();                                                                                \
        rdbuf()->set_callback(nullptr);                                                                         \
        ctx.close();                                                                                            \
    }                                                                                                           \
                                                                                                                \
    void name##_ostream::open(std::ostream& stream)                                                             \
    {                                                                                                           \
        filter_ostream::open(stream, COMPRESS_CALLBACK);                                                        \
    }                                                                                                           \
                                                                                                                \
    name##_ostream::name##_ostream(name##_ostream&& rhs):                                                       \
        ctx(std::move(rhs.ctx)),                                                                                \
        filter_ostream(std::move(rhs))                                                                          \
    {}                                                                                                          \
                                                                                                                \
    name##_ostream & name##_ostream::operator=(name##_ostream&& rhs)                                            \
    {                                                                                                           \
        ctx = std::move(rhs.ctx);                                                                               \
        filter_ostream::operator=(std::move(rhs));                                                              \
        return *this;                                                                                           \
    }


/**
 *  \brief Macro to define methods for a filtering ifstream base.
 */
#define COMPRESSED_IFSTREAM(name)                                                                               \
    name##_ifstream::name##_ifstream()                                                                          \
    {}                                                                                                          \
                                                                                                                \
    name##_ifstream::name##_ifstream(name##_ifstream&& rhs):                                                    \
        ctx(std::move(rhs.ctx)),                                                                                \
        filter_ifstream(std::move(rhs))                                                                         \
    {}                                                                                                          \
                                                                                                                \
    name##_ifstream & name##_ifstream::operator=(name##_ifstream&& rhs)                                         \
    {                                                                                                           \
        ctx = std::move(rhs.ctx);                                                                               \
        filter_ifstream::operator=(std::move(rhs));                                                             \
        return *this;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    name##_ifstream::~name##_ifstream()                                                                         \
    {                                                                                                           \
        filter_ifstream::close();                                                                               \
        rdbuf()->set_callback(nullptr);                                                                         \
        ctx.close();                                                                                            \
    }                                                                                                           \
                                                                                                                \
    name##_ifstream::name##_ifstream(const std::string &name, std::ios_base::openmode mode)                     \
    {                                                                                                           \
        open(name, mode);                                                                                       \
    }                                                                                                           \
                                                                                                                \
    void name##_ifstream::open(const std::string &name, std::ios_base::openmode mode)                           \
    {                                                                                                           \
        filter_ifstream::open(name, mode, DECOMPRESS_CALLBACK);                                                 \
    }                                                                                                           \
                                                                                                                \
    WIDE_PATH_IFSTREAM(name)


/**
 *  \brief Macro to define methods for a filtering ofstream base.
 */
#define COMPRESSED_OFSTREAM(name)                                                                               \
    name##_ofstream::name##_ofstream()                                                                          \
    {}                                                                                                          \
                                                                                                                \
    name##_ofstream::name##_ofstream(int level):                                                                \
        ctx(level)                                                                                              \
    {}                                                                                                          \
                                                                                                                \
    name##_ofstream::name##_ofstream(name##_ofstream&& rhs):                                                    \
        ctx(std::move(rhs.ctx)),                                                                                \
        filter_ofstream(std::move(rhs))                                                                         \
    {}                                                                                                          \
                                                                                                                \
    name##_ofstream & name##_ofstream::operator=(name##_ofstream&& rhs)                                         \
    {                                                                                                           \
        ctx = std::move(rhs.ctx);                                                                               \
        filter_ofstream::operator=(std::move(rhs));                                                             \
        return *this;                                                                                           \
    }                                                                                                           \
                                                                                                                \
    name##_ofstream::~name##_ofstream()                                                                         \
    {                                                                                                           \
        filter_ofstream::close();                                                                               \
        rdbuf()->set_callback(nullptr);                                                                         \
        ctx.close();                                                                                            \
    }                                                                                                           \
                                                                                                                \
    name##_ofstream::name##_ofstream(const std::string &name, std::ios_base::openmode mode)                     \
    {                                                                                                           \
        open(name, mode);                                                                                       \
    }                                                                                                           \
                                                                                                                \
    name##_ofstream::name##_ofstream(const std::string &name, int level, std::ios_base::openmode mode):         \
        ctx(level)                                                                                              \
    {                                                                                                           \
        open(name, mode);                                                                                       \
    }                                                                                                           \
                                                                                                                \
    void name##_ofstream::open(const std::string &name, std::ios_base::openmode mode)                           \
    {                                                                                                           \
        filter_ofstream::open(name, mode, COMPRESS_CALLBACK);                                                   \
    }                                                                                                           \
                                                                                                                \
    WIDE_PATH_OFSTREAM(name)


/**
 *  \brief Defines the compressed streams.
 */
#define COMPRESSED_STREAM_DEFINITION(name)                                                                      \
    COMPRESSED_ISTREAM(name)                                                                                    \
    COMPRESSED_OSTREAM(name)                                                                                    \
    COMPRESSED_IFSTREAM(name)                                                                                   \
    COMPRESSED_OFSTREAM(name)

// HELPERS
// -------

#if defined(HAVE_BZIP2)

template <typename Stream>
static void new_bz2_decompressor(Stream& stream, compression_format& format, void*& ctx)
{
    format = compression_bz2;
    ctx = (void*) new bz2_decompressor;
    stream.rdbuf()->set_callback([&ctx] (const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size) {
        ((bz2_decompressor*) ctx)->decompress(src, srclen, dst, dstlen);
    });
}

#endif              // HAVE_BZIP2

#if defined(HAVE_ZLIB)

template <typename Stream>
static void new_zlib_decompressor(Stream& stream, compression_format& format, void*& ctx)
{
    format = compression_zlib;
    ctx = (void*) new zlib_decompressor;
    stream.rdbuf()->set_callback([&ctx] (const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size) {
        ((zlib_decompressor*) ctx)->decompress(src, srclen, dst, dstlen);
    });
}


template <typename Stream>
static void new_gzip_decompressor(Stream& stream, compression_format& format, void*& ctx)
{
    format = compression_gzip;
    ctx = (void*) new gzip_decompressor;
    stream.rdbuf()->set_callback([&ctx] (const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size) {
        ((gzip_decompressor*) ctx)->decompress(src, srclen, dst, dstlen);
    });
}

#endif              // HAVE_ZLIB

#if defined(HAVE_LZMA)

template <typename Stream>
static void new_lzma_decompressor(Stream& stream, compression_format& format, void*& ctx)
{
    format = compression_lzma;
    ctx = (void*) new lzma_decompressor;
    stream.rdbuf()->set_callback([&ctx] (const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size) {
        ((lzma_decompressor*) ctx)->decompress(src, srclen, dst, dstlen);
    });
}

#endif              // HAVE_LZMA


template <typename Stream>
static void new_decompressor(Stream& stream, char c, compression_format& format, void*& ctx)
{
    // detect format based on the first character
    // It may be wrong, but then we have a corrupt stream
    // Also, since all these bytes are binary, and not
    // **too** common, we should be pretty safe.
    switch (c) {

#if defined(HAVE_BZIP2)
        case '\x42':            /* bzip2 */
            new_bz2_decompressor(stream, format, ctx);
            break;
#endif              // HAVE_BZIP2

#if defined(HAVE_ZLIB)
        case '\x78':            /* zlib */
            new_zlib_decompressor(stream, format, ctx);
            break;

        case '\x1f':            /* gzip */
            new_gzip_decompressor(stream, format, ctx);
            break;
#endif              // HAVE_ZLIB

#if defined(HAVE_LZMA)
        case '\xFD':            /* lzma */
            new_lzma_decompressor(stream, format, ctx);
            break;
#endif              // HAVE_LZMA

        default:
            break;
    }
}


template <typename Stream, typename Path>
typename std::enable_if<!std::is_arithmetic<Path>::value, void>::type
static new_decompressor(Stream& stream, const Path& path, compression_format& format, void*& ctx)
{
#if defined(HAVE_BZIP2)
    if (is_bz2::path(path)) {
        new_bz2_decompressor(stream, format, ctx);
        return;
    }
#endif              // HAVE_BZIP2

#if defined(HAVE_ZLIB)
    if (is_zlib::path(path)) {
        new_zlib_decompressor(stream, format, ctx);
        return;
    } else if (is_gzip::path(path)) {
        new_gzip_decompressor(stream, format, ctx);
        return;
    }
#endif              // HAVE_ZLIB

#if defined(HAVE_LZMA)
    if (is_lzma::path(path)) {
        new_lzma_decompressor(stream, format, ctx);
    }
#endif              // HAVE_LZMA
}


static void delete_decompressor(compression_format format, void* ctx)
{
    switch (format) {
#if defined(HAVE_BZIP2)
        case compression_bz2:
            delete (bz2_decompressor*) ctx;
            break;
#endif              // HAVE_BZIP2

#if defined(HAVE_ZLIB)
        case compression_zlib:
            delete (zlib_decompressor*) ctx;
            break;

        case compression_gzip:
            delete (gzip_decompressor*) ctx;
            break;
#endif              // HAVE_ZLIB

#if defined(HAVE_LZMA)
        case compression_lzma:
            delete (lzma_decompressor*) ctx;
            break;
#endif              // HAVE_LZMA

        default:
            break;
    }
}

// OBJECTS
// -------

#if defined(HAVE_BZIP2)
    COMPRESSED_STREAM_DEFINITION(bz2);
#endif

#if defined(HAVE_ZLIB)
    COMPRESSED_STREAM_DEFINITION(zlib);
    COMPRESSED_STREAM_DEFINITION(gzip);
#endif

#if defined(HAVE_LZMA)
    COMPRESSED_STREAM_DEFINITION(lzma);
#endif


decompressing_istream::decompressing_istream()
{}


decompressing_istream::~decompressing_istream()
{
    filter_istream::close();
    rdbuf()->set_callback(nullptr);
    delete_decompressor(format, ctx);
}


decompressing_istream::decompressing_istream(std::istream& stream)
{
    open(stream);
}


void decompressing_istream::open(std::istream& stream)
{
    char c = stream.peek();
    filter_istream::open(stream);
    new_decompressor(*this, c, format, ctx);
}


decompressing_istream::decompressing_istream(decompressing_istream&& rhs)
{
    std::swap(format, rhs.format);
    std::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
}


decompressing_istream & decompressing_istream::operator=(decompressing_istream&& rhs)
{
    std::swap(format, rhs.format);
    std::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
    return *this;
}


decompressing_ifstream::decompressing_ifstream()
{}


decompressing_ifstream::decompressing_ifstream(decompressing_ifstream&& rhs)
{
    std::swap(format, rhs.format);
    std::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
}


decompressing_ifstream & decompressing_ifstream::operator=(decompressing_ifstream&& rhs)
{
    std::swap(format, rhs.format);
    std::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
    return *this;
}


decompressing_ifstream::~decompressing_ifstream()
{
    filter_ifstream::close();
    rdbuf()->set_callback(nullptr);
    delete_decompressor(format, ctx);
}


decompressing_ifstream::decompressing_ifstream(const std::string &name, std::ios_base::openmode mode)
{
    open(name, mode);
}

void decompressing_ifstream::open(const std::string &name, std::ios_base::openmode mode)
{
    filter_ifstream::open(name, mode);
    new_decompressor(*this, name, format, ctx);
}

#ifdef PYCPP_HAVE_WFOPEN

decompressing_ifstream::decompressing_ifstream(const std::wstring &name, std::ios_base::openmode mode)
{
    open(name, mode);
}

void decompressing_ifstream::open(const std::wstring &name, std::ios_base::openmode mode)
{
    filter_ifstream::open(name, mode);
    new_decompressor(*this, name, format, ctx);
}

#endif

// CLEANUP
// -------

#undef COMPRESS_CALLBACK
#undef DECOMPRESS_CALLBACK
#undef WIDE_PATH_IFSTREAM
#undef WIDE_PATH_OFSTREAM
#undef COMPRESSED_ISTREAM
#undef COMPRESSED_OSTREAM
#undef COMPRESSED_IFSTREAM
#undef COMPRESSED_OFSTREAM
#undef COMPRESSED_STREAM_DEFINITION

PYCPP_END_NAMESPACE
