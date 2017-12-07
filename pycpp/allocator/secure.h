//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure C++-style memory allocator.
 *
 *  Provides wrappers around libsodium to enable secure memory
 *  allocation and deallocation.
 */

#pragma once

#include <pycpp/config.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Base for secure memory allocator.
 */
struct secure_allocator_base
{
    void* allocate(size_t n, size_t size, const void* hint = nullptr);
    void deallocate(void* p, size_t n);
    void zero(void* p, size_t size);
};


/**
 *  \brief Secure memory allocator.
 */
template <typename T>
struct secure_allocator: private secure_allocator_base
{
    // MEMBER TYPES
    // ------------
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    // MEMBER FUNCTIONS
    // ----------------
    secure_allocator() noexcept = default;
    secure_allocator(const secure_allocator<T>&) noexcept = default;
    ~secure_allocator() = default;

    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
};

// IMPLEMENTATION
// --------------


template <typename T>
auto secure_allocator<T>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(secure_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
void secure_allocator<T>::deallocate(pointer p, size_type n)
{
    secure_allocator_base::zero(p, n * sizeof(T));
    secure_allocator_base::deallocate(p, n);
}


template <typename T, typename U>
bool operator==(const secure_allocator<T>&, const secure_allocator<U>&) noexcept
{
    return true;
}


template <typename T, typename U>
bool operator!=(const secure_allocator<T>& lhs, const secure_allocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
