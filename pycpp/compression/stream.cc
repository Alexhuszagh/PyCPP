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
           size_t dstlen, size_t char_size) {                                                                   \
        ctx.decompress(src, srclen, dst, dstlen);                                                               \
    }


/**
 *  \brief Provides wide-path overloads for Windows.
 */
#if defined(OS_WINDOWS)                     // WINDOWS

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
    COMPRESSED_ISTREAM(name);                                                                                   \
    COMPRESSED_OSTREAM(name);                                                                                   \
    COMPRESSED_IFSTREAM(name);                                                                                  \
    COMPRESSED_OFSTREAM(name);

// OBJECTS
// -------

COMPRESSED_STREAM_DEFINITION(bz2)
COMPRESSED_STREAM_DEFINITION(zlib)
COMPRESSED_STREAM_DEFINITION(lzma)
// TODO:
// blosc
// gzip
// general compressed stream

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
