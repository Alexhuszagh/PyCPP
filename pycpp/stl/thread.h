//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <thread>.
 */

#pragma once

#include <pycpp/config.h>
#include <functional>
#include <thread>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

// Thread ID has no public interface: let the standard hash
// take care of it. It can be a long (Linux), a pointer
// (Darwin), or another platform-specific type.
template <>
struct hash<std::thread::id>
{
    using argument_type = std::thread::id;
    using result_type = size_t;

    inline size_t operator()(argument_type x) const noexcept
    {
        return std::hash<argument_type>()(x);
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
