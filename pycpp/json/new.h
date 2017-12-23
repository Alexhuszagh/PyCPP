//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Specialized memory allocation for JSON objects.
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <pycpp/stl/utility.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Global allocator for JSON data.
 *
 *  To change the allocation behavior of this allocator,
 *  change the underlying `resource` so it is copy
 *  constructed into all JSON data.
 */
inline allocator<byte>& json_allocator() noexcept
{
    static allocator<byte> singleton;
    return singleton;
}


template <typename T, typename ... Ts>
T* json_new(Ts&&... ts)
{
    static allocator<byte>& alloc = json_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    byte* p = traits_type::allocate(alloc, sizeof(T));
    new(p) T(forward<Ts>(ts)...);
    return reinterpret_cast<T*>(p);
}


template <typename T>
void json_delete(T* t) noexcept
{
    static allocator<byte>& alloc = json_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    if (t != nullptr) {
        t->~T();
        traits_type::deallocate(alloc, reinterpret_cast<byte*>(t), 1);
    }
}

// OBJECTS
// -------

// Specific allocator adaptor for the JSON back-end.
// This allocator stores the size of the array in the head
// of the allocated array, allowing stack and linear allocators
// to be effectively used.
struct json_backend_allocator
{
    static constexpr bool kNeedFree = true;

    static void* Malloc(size_t size);
    static void* Realloc(void* p, size_t old_size, size_t new_size);
    static void Free(void* p) noexcept;
};


inline void* json_backend_allocator::Malloc(size_t size)
{
    static allocator<byte>& alloc = json_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    // special case for rapidjson compliance
    if (size == 0) {
        return nullptr;
    }

    // allocate and store an extra size_t before the start of the array
    try {
        byte* p = traits_type::allocate(alloc, size + sizeof(size_t));
        size_t* s = reinterpret_cast<size_t*>(p);
        *s++ = size;
        return reinterpret_cast<void*>(s);
    } catch (bad_alloc&) {
        return nullptr;
    }
}


inline void* json_backend_allocator::Realloc(void* pin, size_t old_size, size_t new_size)
{
    // special case for rapidjson compliance
    if (new_size == 0) {
        Free(pin);
        return nullptr;
    }

    // allocate
    void* pout = Malloc(new_size);
    if (pout == nullptr) {
        return nullptr;
    }

    // copy over bytes and free
    memcpy(pout, pin, old_size);
    Free(pin);

    return pout;
}


inline void json_backend_allocator::Free(void* p) noexcept
{
    static allocator<byte>& alloc = json_allocator();
    using traits_type = allocator_traits<allocator<byte>>;

    if (p != nullptr) {
        size_t* s = reinterpret_cast<size_t*>(p);
        size_t size = *(--s) + sizeof(size_t);
        traits_type::deallocate(alloc, reinterpret_cast<byte*>(s), size);
    }
}

PYCPP_END_NAMESPACE
