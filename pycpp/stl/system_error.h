//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <system_error>.
 */

#pragma once

#include <pycpp/stl/hash.h>
#include <system_error>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if USE_XXHASH

template <>
struct hash<std::error_code>
{
    inline size_t operator()(const std::error_code& x) const noexcept
    {
        return hash<int>()(x.value());
    }
};

template <>
struct hash<std::error_condition>
{
    inline size_t operator()(const std::error_condition& x) const noexcept
    {
        return hash<int>()(x.value());
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
