//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <typeindex>.
 */

#pragma once

#include <pycpp/hashlib/specialize.h>
#include <typeindex>

PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

PYCPP_SPECIALIZE_HASH_VALUE(hash, std::type_index);

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
