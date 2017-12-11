//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <system_error>.
 */

#pragma once

#include <pycpp/hashlib/specialize.h>
#include <pycpp/preprocessor/compiler.h>
#include <system_error>

PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

PYCPP_SPECIALIZE_HASH_VALUE(hash, std::error_code);
#if defined(HAVE_CPP17)
PYCPP_SPECIALIZE_HASH_VALUE(hash, std::error_condition);
#endif          // HAVE_CPP17

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
