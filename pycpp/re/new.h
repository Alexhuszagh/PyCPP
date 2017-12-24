//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Specialized memory allocation for RE objects.
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <pycpp/stl/utility.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Global allocator for RE data.
 *
 *  To change the allocation behavior of this allocator,
 *  change the underlying `resource` so it is copy
 *  constructed into all RE data.
 *
 *  This can only change the memory allocated internally by the RE2
 *  wrappers, as RE2 uses the global memory allocator.
 */
inline allocator<byte>& re_allocator() noexcept
{
    static allocator<byte> singleton;
    return singleton;
}


template <typename T, typename ... Ts>
T* re_new(Ts&&... ts)
{
    static allocator<byte>& alloc = re_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    byte* p = traits_type::allocate(alloc, sizeof(T));
    new(p) T(forward<Ts>(ts)...);
    return reinterpret_cast<T*>(p);
}


template <typename T>
void re_delete(T* t) noexcept
{
    static allocator<byte>& alloc = re_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    if (t != nullptr) {
        t->~T();
        traits_type::deallocate(alloc, reinterpret_cast<byte*>(t), 1);
    }
}

PYCPP_END_NAMESPACE
