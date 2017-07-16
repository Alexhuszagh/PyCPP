//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/core.h>
#include <pycpp/compression/gzip.h>
#include <zlib.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static size_t zlib_compress_bound(size_t size)
{
    return compressBound(size);
}

// TODO: here...



PYCPP_END_NAMESPACE
