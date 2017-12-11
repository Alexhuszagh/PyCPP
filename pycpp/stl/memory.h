//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <memory>.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/detail/polymorphic_allocator.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if USE_POLYMORPHIC_ALLOCATOR           // POLYMOPRHIC

template <typename T>
using allocator = polymorphic_allocator<T>;

#else                                   // !POLYMOPRHIC

template <typename T>
using allocator = std::allocator<T>;

#endif                                  // POLYMOPRHIC

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if USE_XXHASH

template <typename T, typename Deleter>
struct hash<std::unique_ptr<T, Deleter>>
{
    using argument_type = std::unique_ptr<T, Deleter>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

template <typename T>
struct hash<std::shared_ptr<T>>
{
    using argument_type = std::shared_ptr<T>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
