//  :copyright: (c) 2013 Cosku Acay, http://www.coskuacay.com.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Memory pool allocator.
 *
 *  An allocator that optimizes for types of known size by using
 *  a memory pool to allocate each block.
 */

//// TODO: implement
//// Pool allocators take advantage of fixed-size allocations
//// Array allocations make no sense, since they break this.
//// It will therefore work with list, and not vector,
//// since items must be requested 1x1.
//
//// Boost has a fast pool allocator, which works like this.
//// It all has a pool allocator, which can handle custom shapes.
//
//// I might need a custom mutex for the implementation...
//// This allocator is not thread-safe, which...

#pragma once

#include <pycpp/config.h>
#include <limits.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Allocator optimized for stack-based allocation.
 */
template <
    typename T,
    size_t N,
    size_t Alignment = alignof(max_align_t)
>
class pool_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1, size_t N1 = N, size_t A1 = Alignment>
    struct rebind { using other = pool_allocator<T1, N1, A1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t size = N;

    // MEMBER TYPES
    // ------------
    using value_type = T;
#if defined(CPP11_PARTIAL_COMPATIBILITY)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
#endif      // CPP11_PARTIAL_COMPATIBILITY
// TODO: here

    // MEMBER FUNCTIONS
    // ----------------
//    pool_allocator() noexcept;
//    pool_allocator(const pool_allocator<T, N, Alignment>&) noexcept;
//    ~pool_allocator() noexcept;

//    pool_allocator<T, N, Alignment>& operator=(const pool_allocator<T, N, Alignment>&) noexcept;
//    pool_allocator<T, N, Alignment>& operator=(pool_allocator<T, N, Alignment>&&) noexcept;

private:
    union slot_
    {
        value_type element;
        slot_* next;
    };

    using data_pointer_ = char*;
    using slot_type_ = slot_;
    using slot_pointer_ = slot_*;
};

// TODO: specialize is_relocatable
// TODO: add locked and unlocked variants

// IMPLEMENTATION
// --------------


PYCPP_END_NAMESPACE
