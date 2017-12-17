//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <system_error>.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/detail/hash_specialize.h>
#include <system_error>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::error_category;
using std::error_condition;
using std::errc;
using std::error_code;
using std::system_error;

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

PYCPP_SPECIALIZE_HASH_VALUE(hash, error_code);
#if defined(HAVE_CPP17)
PYCPP_SPECIALIZE_HASH_VALUE(hash, error_condition);
#endif          // HAVE_CPP17

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
