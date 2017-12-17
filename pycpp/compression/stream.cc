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
    [this](const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size)                        \
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
    [this](const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t char_size)                        \
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
#if defined(HAVE_WFOPEN)                    // WINDOWS

#   define WIDE_PATH_IFSTREAM(name)                                                                             \
                                                                                                                \
        name##_ifstream::name##_ifstream(const wstring_view& name, ios_base::openmode mode)                     \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ifstream::open(const wstring_view& name, ios_base::openmode mode)                           \
        {                                                                                                       \
            filter_ifstream::open(name, mode, DECOMPRESS_CALLBACK);                                             \
        }                                                                                                       \
                                                                                                                \
        name##_ifstream::name##_ifstream(const u16string_view& name, ios_base::openmode mode)                   \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ifstream::open(const u16string_view& name, ios_base::openmode mode)                         \
        {                                                                                                       \
            filter_ifstream::open(name, mode, DECOMPRESS_CALLBACK);                                             \
        }

#   define WIDE_PATH_OFSTREAM(name)                                                                             \
                                                                                                                \
        name##_ofstream::name##_ofstream(const wstring_view& name, ios_base::openmode mode)                     \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        name##_ofstream::name##_ofstream(const wstring_view& name, int level, ios_base::openmode mode):         \
            ctx(level)                                                                                          \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ofstream::open(const wstring_view& name, ios_base::openmode mode)                           \
        {                                                                                                       \
            filter_ofstream::open(name, mode, COMPRESS_CALLBACK);                                               \
        }                                                                                                       \
                                                                                                                \
        name##_ofstream::name##_ofstream(const u16string_view& name, ios_base::openmode mode)                   \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        name##_ofstream::name##_ofstream(const u16string_view& name, int level, ios_base::openmode mode):       \
            ctx(level)                                                                                          \
        {                                                                                                       \
            open(name, mode);                                                                                   \
        }                                                                                                       \
                                                                                                                \
        void name##_ofstream::open(const u16string_view& name, ios_base::openmode mode)                         \
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
#define COMPRESSED_ISTREAM(name)                                        \
    name##_istream::name##_istream()                                    \
    {}                                                                  \
                                                                        \
    name##_istream::name##_istream(istream& stream)                     \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_istream::~name##_istream()                                   \
    {                                                                   \
        filter_istream::close();                                        \
        rdbuf()->set_callback(nullptr);                                 \
        ctx.close();                                                    \
    }                                                                   \
                                                                        \
    void name##_istream::open(istream& stream)                          \
    {                                                                   \
        filter_istream::open(stream, DECOMPRESS_CALLBACK);              \
    }                                                                   \
                                                                        \
    name##_istream::name##_istream(name##_istream&& rhs):               \
        ctx(PYCPP_NAMESPACE::move(rhs.ctx)),                            \
        filter_istream(PYCPP_NAMESPACE::move(rhs))                      \
    {}                                                                  \
                                                                        \
    name##_istream & name##_istream::operator=(name##_istream&& rhs)    \
    {                                                                   \
        ctx = PYCPP_NAMESPACE::move(rhs.ctx);                           \
        filter_istream::operator=(PYCPP_NAMESPACE::move(rhs));          \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    void name##_istream::swap(name##_istream& rhs)                      \
    {                                                                   \
        static_assert(false, "");                                       \
    }


/**
 *  \brief Macro to define methods for a filtering ostream base.
 */
#define COMPRESSED_OSTREAM(name)                                        \
    name##_ostream::name##_ostream()                                    \
    {}                                                                  \
                                                                        \
    name##_ostream::name##_ostream(int level):                          \
        ctx(level)                                                      \
    {}                                                                  \
                                                                        \
    name##_ostream::name##_ostream(ostream& stream)                     \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_ostream::name##_ostream(ostream& stream, int level):         \
        ctx(level)                                                      \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_ostream::~name##_ostream()                                   \
    {                                                                   \
        filter_ostream::close();                                        \
        rdbuf()->set_callback(nullptr);                                 \
        ctx.close();                                                    \
    }                                                                   \
                                                                        \
    void name##_ostream::open(ostream& stream)                          \
    {                                                                   \
        filter_ostream::open(stream, COMPRESS_CALLBACK);                \
    }                                                                   \
                                                                        \
    name##_ostream::name##_ostream(name##_ostream&& rhs):               \
        ctx(PYCPP_NAMESPACE::move(rhs.ctx)),                            \
        filter_ostream(PYCPP_NAMESPACE::move(rhs))                      \
    {}                                                                  \
                                                                        \
    name##_ostream & name##_ostream::operator=(name##_ostream&& rhs)    \
    {                                                                   \
        ctx = PYCPP_NAMESPACE::move(rhs.ctx);                           \
        filter_ostream::operator=(PYCPP_NAMESPACE::move(rhs));          \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    void name##_ostream::swap(name##_ostream& rhs)                      \
    {                                                                   \
        static_assert(false, "");                                       \
    }

/**
 *  \brief Macro to define methods for a filtering ifstream base.
 */
#define COMPRESSED_IFSTREAM(name)                                                           \
    name##_ifstream::name##_ifstream()                                                      \
    {}                                                                                      \
                                                                                            \
    name##_ifstream::name##_ifstream(name##_ifstream&& rhs):                                \
        ctx(PYCPP_NAMESPACE::move(rhs.ctx)),                                                \
        filter_ifstream(PYCPP_NAMESPACE::move(rhs))                                         \
    {}                                                                                      \
                                                                                            \
    name##_ifstream & name##_ifstream::operator=(name##_ifstream&& rhs)                     \
    {                                                                                       \
        ctx = PYCPP_NAMESPACE::move(rhs.ctx);                                               \
        filter_ifstream::operator=(PYCPP_NAMESPACE::move(rhs));                             \
        return *this;                                                                       \
    }                                                                                       \
                                                                                            \
    name##_ifstream::~name##_ifstream()                                                     \
    {                                                                                       \
        filter_ifstream::close();                                                           \
        rdbuf()->set_callback(nullptr);                                                     \
        ctx.close();                                                                        \
    }                                                                                       \
                                                                                            \
    name##_ifstream::name##_ifstream(const string_view& name, ios_base::openmode mode)      \
    {                                                                                       \
        open(name, mode);                                                                   \
    }                                                                                       \
                                                                                            \
    void name##_ifstream::open(const string_view& name, ios_base::openmode mode)            \
    {                                                                                       \
        filter_ifstream::open(name, mode, DECOMPRESS_CALLBACK);                             \
    }                                                                                       \
                                                                                            \
    WIDE_PATH_IFSTREAM(name)                                                                \
                                                                                            \
    void name##_ifstream::swap(name##_ifstream& rhs)                                        \
    {                                                                                       \
        static_assert(false, "");                                                           \
    }


/**
 *  \brief Macro to define methods for a filtering ofstream base.
 */
#define COMPRESSED_OFSTREAM(name)                                                                   \
    name##_ofstream::name##_ofstream()                                                              \
    {}                                                                                              \
                                                                                                    \
    name##_ofstream::name##_ofstream(int level):                                                    \
        ctx(level)                                                                                  \
    {}                                                                                              \
                                                                                                    \
    name##_ofstream::name##_ofstream(name##_ofstream&& rhs):                                        \
        ctx(PYCPP_NAMESPACE::move(rhs.ctx)),                                                        \
        filter_ofstream(PYCPP_NAMESPACE::move(rhs))                                                 \
    {}                                                                                              \
                                                                                                    \
    name##_ofstream & name##_ofstream::operator=(name##_ofstream&& rhs)                             \
    {                                                                                               \
        ctx = PYCPP_NAMESPACE::move(rhs.ctx);                                                       \
        filter_ofstream::operator=(PYCPP_NAMESPACE::move(rhs));                                     \
        return *this;                                                                               \
    }                                                                                               \
                                                                                                    \
    name##_ofstream::~name##_ofstream()                                                             \
    {                                                                                               \
        filter_ofstream::close();                                                                   \
        rdbuf()->set_callback(nullptr);                                                             \
        ctx.close();                                                                                \
    }                                                                                               \
                                                                                                    \
    name##_ofstream::name##_ofstream(const string_view& name, ios_base::openmode mode)              \
    {                                                                                               \
        open(name, mode);                                                                           \
    }                                                                                               \
                                                                                                    \
    name##_ofstream::name##_ofstream(const string_view& name, int level, ios_base::openmode mode):  \
        ctx(level)                                                                                  \
    {                                                                                               \
        open(name, mode);                                                                           \
    }                                                                                               \
                                                                                                    \
    void name##_ofstream::open(const string_view& name, ios_base::openmode mode)                    \
    {                                                                                               \
        filter_ofstream::open(name, mode, COMPRESS_CALLBACK);                                       \
    }                                                                                               \
                                                                                                    \
    WIDE_PATH_OFSTREAM(name)                                                                        \
                                                                                                    \
    void name##_ofstream::swap(name##_ofstream& rhs)                                                \
    {                                                                                               \
        static_assert(false, "");                                                                   \
    }


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
enable_if_t<!is_arithmetic<Path>::value, void>
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

#if defined(HAVE_BZIP2)                     // HAVE_BZIP2
    COMPRESSED_STREAM_DEFINITION(bz2);
#endif                                      // HAVE_BZIP2

#if defined(HAVE_ZLIB)                      // HAVE_ZLIB
    COMPRESSED_STREAM_DEFINITION(zlib);
    COMPRESSED_STREAM_DEFINITION(gzip);
#endif                                      // HAVE_ZLIB

#if defined(HAVE_LZMA)                      // HAVE_LZMA
    COMPRESSED_STREAM_DEFINITION(lzma);
#endif                                      // HAVE_LZMA


decompressing_istream::decompressing_istream()
{}


decompressing_istream::~decompressing_istream()
{
    filter_istream::close();
    rdbuf()->set_callback(nullptr);
    delete_decompressor(format, ctx);
}


decompressing_istream::decompressing_istream(istream& stream)
{
    open(stream);
}


void decompressing_istream::open(istream& stream)
{
    char c = stream.peek();
    filter_istream::open(stream);
    new_decompressor(*this, c, format, ctx);
}


decompressing_istream::decompressing_istream(decompressing_istream&& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(format, rhs.format);
    swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
}


decompressing_istream & decompressing_istream::operator=(decompressing_istream&& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(format, rhs.format);
    swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
    return *this;
}


void decompressing_istream::swap(decompressing_istream& rhs)
{
    // TODO: implement...
    static_assert(false, "");
}


decompressing_ifstream::decompressing_ifstream()
{}


decompressing_ifstream::decompressing_ifstream(decompressing_ifstream&& rhs)
{
    PYCPP_NAMESPACE::swap(format, rhs.format);
    PYCPP_NAMESPACE::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
}


decompressing_ifstream& decompressing_ifstream::operator=(decompressing_ifstream&& rhs)
{
    PYCPP_NAMESPACE::swap(format, rhs.format);
    PYCPP_NAMESPACE::swap(ctx, rhs.ctx);
    filter_istream::swap(rhs);
    return *this;
}


decompressing_ifstream::~decompressing_ifstream()
{
    filter_ifstream::close();
    rdbuf()->set_callback(nullptr);
    delete_decompressor(format, ctx);
}


decompressing_ifstream::decompressing_ifstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}

void decompressing_ifstream::open(const string_view& name, ios_base::openmode mode)
{
    filter_ifstream::open(name, mode);
    new_decompressor(*this, name, format, ctx);
}

#if defined(HAVE_WFOPEN)                    // WINDOWS

decompressing_ifstream::decompressing_ifstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}

void decompressing_ifstream::open(const wstring_view& name, ios_base::openmode mode)
{
    filter_ifstream::open(name, mode);
    new_decompressor(*this, name, format, ctx);
}


decompressing_ifstream::decompressing_ifstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void decompressing_ifstream::open(const u16string_view& name, ios_base::openmode mode)
{
    filter_ifstream::open(name, mode);
    new_decompressor(*this, name, format, ctx);
}

#endif                                      // WINDOWS


void decompressing_ifstream::swap(decompressing_ifstream& rhs)
{
    // TODO: implement...
    static_assert(false, "");
}

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
