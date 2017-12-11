//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect compressed files, streams, and paths.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/istream.h>
#include <pycpp/stl/vector_view.h>
#include <pycpp/stream/filter.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using magic_bytes = vector_view<string_wrapper>;

// ENUMS
// -----

/**
 *  \brief Enumerated values for all supported compression formats.
 */
enum compression_format
{
    compression_none = 0,
    compression_bz2,
    compression_zlib,
    compression_gzip,
    compression_lzma,
    compression_blosc,
};

// MACROS
// ------

/**
 *  \brief Provides path expansion for Windows.
 */
#if defined(HAVE_WFOPEN)                    // WINDOWS
#   define DETECT_PATH                                      \
        static bool path(const wstring_view& path);         \
        static bool path(const u16string_view& path);       \
        static bool path(const string_view& path);
#else                                       // POSIX
#   define DETECT_PATH                                      \
        static bool path(const string_view& path);
#endif                                      // WINDOWS

/**
 *  \brief Macro to define a compression-detection object.
 */
#define IS_COMPRESSED(name)                                             \
    struct is_##name                                                    \
    {                                                                   \
        static const magic_bytes& magic();                              \
        static bool header(const string_wrapper& header);               \
        static bool stream(std::istream& stream);                       \
        DETECT_PATH                                                     \
    }

// OBJECTS
// -------

IS_COMPRESSED(bz2);
IS_COMPRESSED(zlib);
IS_COMPRESSED(gzip);
IS_COMPRESSED(lzma);
IS_COMPRESSED(blosc);

// CLEANUP
// -------

#undef DETECT_PATH
#undef IS_COMPRESSED

PYCPP_END_NAMESPACE
