//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <thread>.
 */

#pragma once

#include <pycpp/stl/detail/hash_specialize.h>
#include <thread>

PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

// Thread ID has no public interface: let the standard hash
// take care of it. It can be a long (Linux), a pointer
// (Darwin), or another platform-specific type.
PYCPP_SPECIALIZE_HASH_VALUE(hash, std::thread::id);

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
