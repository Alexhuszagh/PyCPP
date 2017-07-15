//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/stream.h>
#include <iostream>     // TODO: remove

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------


/**
 *  \brief Callback function for compression.
 */
#define COMPRESS_CALLBACK                                               \
    [this](const void*& src, size_t srclen, void*& dst,                 \
           size_t dstlen, size_t char_size)                             \
    {                                                                   \
        std::cout << src << " " << srclen << " " << dst << " " << dstlen << std::endl; \
        if (srclen) {                                                   \
            ctx.compress(src, srclen, dst, dstlen);                     \
        } else {                                                        \
            ctx.flush(dst, dstlen);                                     \
        }                                                               \
    }


/**
 *  \brief Callback function for decompression.
 */
#define DECOMPRESS_CALLBACK                                             \
    [this](const void*& src, size_t srclen, void*& dst,                 \
           size_t dstlen, size_t char_size) {                           \
        ctx.decompress(src, srclen, dst, dstlen);                       \
    }


/**
 *  \brief Macro to define methods for a filtering istream base.
 */
#define COMPRESSED_ISTREAM(name)                                        \
    name##_istream::name##_istream()                                    \
    {}                                                                  \
                                                                        \
    name##_istream::name##_istream(std::istream& stream)                \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_istream::~name##_istream()                                   \
    {                                                                   \
        filter_istream::close();                                        \
    }                                                                   \
                                                                        \
    void name##_istream::open(std::istream& stream)                     \
    {                                                                   \
        filter_istream::open(stream, DECOMPRESS_CALLBACK);              \
    }                                                                   \
                                                                        \
    name##_istream::name##_istream(name##_istream&& rhs):               \
        ctx(std::move(rhs.ctx)),                                        \
        filter_istream(std::move(rhs))                                  \
    {}                                                                  \
                                                                        \
    name##_istream & name##_istream::operator=(name##_istream&& rhs)    \
    {                                                                   \
        ctx = std::move(rhs.ctx);                                       \
        filter_istream::operator=(std::move(rhs));                      \
        return *this;                                                   \
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
    name##_ostream::name##_ostream(std::ostream& stream)                \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_ostream::name##_ostream(std::ostream& stream, int level):    \
        ctx(level)                                                      \
    {                                                                   \
        open(stream);                                                   \
    }                                                                   \
                                                                        \
    name##_ostream::~name##_ostream()                                   \
    {                                                                   \
        filter_ostream::close();                                        \
    }                                                                   \
                                                                        \
    void name##_ostream::open(std::ostream& stream)                     \
    {                                                                   \
        filter_ostream::open(stream, COMPRESS_CALLBACK);                \
    }                                                                   \
                                                                        \
    name##_ostream::name##_ostream(name##_ostream&& rhs):               \
        ctx(std::move(rhs.ctx)),                                        \
        filter_ostream(std::move(rhs))                                  \
    {}                                                                  \
                                                                        \
    name##_ostream & name##_ostream::operator=(name##_ostream&& rhs)    \
    {                                                                   \
        ctx = std::move(rhs.ctx);                                       \
        filter_ostream::operator=(std::move(rhs));                      \
        return *this;                                                   \
    }


/**
 *  \brief Defines the compressed streams.
 */
#define COMPRESSED_STREAM_DEFINITION(name)                              \
    COMPRESSED_ISTREAM(name);                                           \
    COMPRESSED_OSTREAM(name);                                           \

//    COMPRESSED_OSTREAM(name);                                           \
//    COMPRESSED_IFSTREAM(name);                                          \
//    COMPRESSED_OFSTREAM(name)


// OBJECTS
// -------


COMPRESSED_STREAM_DEFINITION(bz2)



//
///**
// *  \brief Filtering BZ2 input stream.
// */
//struct bz2_istream: filter_istream
//{
//public:
//    bz2_istream(const bz2_istream&) = delete;
//    bz2_istream & operator=(const bz2_istream&) = delete;
//
//    bz2_istream(std::istream& stream);
//    void open(std::istream& stream);
//
//protected:
//    bz2_istream(bz2_istream&&);
//    bz2_istream & operator=(bz2_istream&&);
//};
//
//
///**
// *  \brief Filtering BZ2 input file stream.
// */
//struct bz2_ifstream: filter_ifstream
//{
//    bz2_ifstream(const bz2_ifstream&) = delete;
//    bz2_ifstream & operator=(const bz2_ifstream&) = delete;
//    bz2_ifstream(bz2_ifstream&&);
//    bz2_ifstream & operator=(bz2_ifstream&&);
//
//    bz2_ifstream(const std::string &name, std::ios_base::openmode = std::ios_base::in);
//    void open(const std::string &name, std::ios_base::openmode = std::ios_base::in);
//};


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
