//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <bitset>.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <bitset>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename Key>
struct hash;

// ALIAS
// -----

using std::bitset;

// SPECIALIZATION
// --------------

#if USE_XXHASH

template <size_t Size>
struct hash<bitset<Size>>
{
    using argument_type = bitset<Size>;
    using result_type = size_t;

    inline size_t operator()(const bitset<Size>& x) const noexcept
    {
        return std::hash<bitset<Size>>()(x);
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
