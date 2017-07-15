//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Decompressing stream definitions.
 */

#pragma once

#include <pycpp/compression/bzip2.h>
#include <pycpp/stream/filter.h>

PYCPP_BEGIN_NAMESPACE


// MACROS
// ------

/**
 *  \brief Provides wide-path overloads for Windows.
 */
#if defined(OS_WINDOWS)                     // WINDOWS

#   define WIDE_PATH_IFSTREAM(name, base, flags)                                                \
        name##_##base(const std::string &name, std::ios_base::openmode = flags);                \
        void open(const std::string &name, std::ios_base::openmode = flags);

#   define WIDE_PATH_OFSTREAM(name, base, flags)                                                \
        name##_##base(const std::string &name, std::ios_base::openmode = flags);                \
        name##_##base(const std::string &name, int level, std::ios_base::openmode = flags);     \
        void open(const std::string &name, std::ios_base::openmode = flags);

#else                                       // POSIX

#   define WIDE_PATH_IFSTREAM(name, base, flags)
#   define WIDE_PATH_OFSTREAM(name, base, flags)

#endif                                      // WINDOWS


/**
 *  \brief Macro to define a filtering istream base.
 */
#define COMPRESSED_ISTREAM(name)                                                                \
    struct name##_istream: filter_istream                                                       \
    {                                                                                           \
    public:                                                                                     \
        name##_istream();                                                                       \
        name##_istream(const name##_istream&) = delete;                                         \
        name##_istream & operator=(const name##_istream&) = delete;                             \
                                                                                                \
        name##_istream(std::istream& stream);                                                   \
        void open(std::istream& stream);                                                        \
                                                                                                \
    protected:                                                                                  \
        name##_istream(name##_istream&&);                                                       \
        name##_istream & operator=(name##_istream&&);                                           \
                                                                                                \
    private:                                                                                    \
        name##_decompressor ctx;                                                                \
    }


/**
 *  \brief Macro to define a filtering ostream base.
 */
#define COMPRESSED_OSTREAM(name)                                                                \
    struct name##_ostream: filter_ostream                                                       \
    {                                                                                           \
    public:                                                                                     \
        name##_ostream();                                                                       \
        name##_ostream(int level);                                                              \
        name##_ostream(const name##_ostream&) = delete;                                         \
        name##_ostream & operator=(const name##_ostream&) = delete;                             \
                                                                                                \
        name##_ostream(std::ostream& stream);                                                   \
        name##_ostream(std::ostream& stream, int level);                                        \
        void open(std::ostream& stream);                                                        \
                                                                                                \
    protected:                                                                                  \
        name##_ostream(name##_ostream&&);                                                       \
        name##_ostream & operator=(name##_ostream&&);                                           \
                                                                                                \
    private:                                                                                    \
        name##_compressor ctx;                                                                  \
    }


/**
 *  \brief Macro to define a filtering ifstream base.
 */
#define COMPRESSED_IFSTREAM(name)                                                               \
    struct name##_ifstream: filter_ifstream                                                     \
    {                                                                                           \
    public:                                                                                     \
        name##_ifstream();                                                                      \
        name##_ifstream(const name##_ifstream&) = delete;                                       \
        name##_ifstream & operator=(const name##_ifstream&) = delete;                           \
        name##_ifstream(name##_ifstream&&);                                                     \
        name##_ifstream & operator=(name##_ifstream&&);                                         \
                                                                                                \
        name##_ifstream(const std::string &name, std::ios_base::openmode = std::ios_base::in);  \
        void open(const std::string &name, std::ios_base::openmode = std::ios_base::in);        \
        WIDE_PATH_IFSTREAM(name, base, std::ios_base::in)                                       \
                                                                                                \
    private:                                                                                    \
        name##_decompressor ctx;                                                                \
    }


/**
 *  \brief Macro to define a filtering ofstream base.
 */
#define COMPRESSED_OFSTREAM(name)                                                                           \
    struct name##_ofstream: filter_ofstream                                                                 \
    {                                                                                                       \
    public:                                                                                                 \
        name##_ofstream();                                                                                  \
        name##_ofstream(int level);                                                                         \
        name##_ofstream(const name##_ofstream&) = delete;                                                   \
        name##_ofstream & operator=(const name##_ofstream&) = delete;                                       \
        name##_ofstream(name##_ofstream&&);                                                                 \
        name##_ofstream & operator=(name##_ofstream&&);                                                     \
                                                                                                            \
        name##_ofstream(const std::string &name, std::ios_base::openmode = std::ios_base::out);             \
        name##_ofstream(const std::string &name, int level, std::ios_base::openmode = std::ios_base::out);  \
        void open(const std::string &name, std::ios_base::openmode = std::ios_base::out);                   \
        WIDE_PATH_OFSTREAM(name, base, std::ios_base::out)                                                  \
                                                                                                            \
    private:                                                                                                \
        name##_compressor ctx;                                                                              \
    }


/**
 *  \brief Defines the compressed streams.
 */
#define COMPRESSED_STREAM_DEFINITION(name)                              \
    COMPRESSED_ISTREAM(name);                                           \
    COMPRESSED_OSTREAM(name);                                           \
    COMPRESSED_IFSTREAM(name);                                          \
    COMPRESSED_OFSTREAM(name)

// OBJECTS
// -------

COMPRESSED_STREAM_DEFINITION(bz2);

// TODO:
// zlib_stream
// zlib_file
// blosc_stream
// blosc_file
// lzma_stream
// lzma_file
// compressed_stream
// compressed_file

// CLEANUP
// -------

#undef WIDE_PATH_IFSTREAM
#undef WIDE_PATH_OFSTREAM
#undef COMPRESSED_ISTREAM
#undef COMPRESSED_OSTREAM
#undef COMPRESSED_IFSTREAM
#undef COMPRESSED_OFSTREAM
#undef COMPRESSED_STREAM_DEFINITION

PYCPP_END_NAMESPACE
