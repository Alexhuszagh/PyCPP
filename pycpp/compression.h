//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Compression encoders and decoders.
 */

#pragma once

#include <pycpp/compression/blosc.h>
#include <pycpp/compression/bzip2.h>
#include <pycpp/compression/xz.h>
#include <pycpp/compression/zlib.h>
#if defined(BUILD_STREAM)
#   include <pycpp/compression/stream.h>
#endif
