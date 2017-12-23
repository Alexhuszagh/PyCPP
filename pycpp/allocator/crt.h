//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief C-runtime allocator.
 *
 *  A shallow wrapper around `malloc` and `free`. This allocator
 *  has poor performance, and therefore should be used sparingly.
 */

#pragma once

#include <pycpp/misc/safe_stdlib.h>
#include <pycpp/stl/memory.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T>
struct crt_allocator;

// OBJECTS
// -------

/**
 *  \brief Base for crt memory allocator.
 */
struct crt_allocator_base
{
    static void* allocate(size_t n, size_t size, const void* hint = nullptr);
    static void deallocate(void* p, size_t n);
};


/**
 *  \brief Standard memory allocator.
 */
template <typename T>
struct crt_allocator: private crt_allocator_base
{
    // MEMBER TYPES
    // ------------
    using self_t = crt_allocator<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // MEMBER FUNCTIONS
    // ----------------
    crt_allocator() noexcept = default;
    crt_allocator(const self_t&) noexcept = default;
    template <typename U> crt_allocator(const crt_allocator<U>&) noexcept;
    self_t& operator=(const self_t&) noexcept = default;
    template <typename U> self_t& operator=(const crt_allocator<U>&) noexcept;
    ~crt_allocator() = default;

    // ALLOCATOR TRAITS
    pointer allocate(size_type, const void* = nullptr);
    pointer reallocate(pointer, size_type, size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
};

// ALIAS
// -----

using crt_resource = resource_adaptor<crt_allocator<char>>;

// DETAIL
// ------

// is_relocatable<T>::value
template <typename T>
T* crt_reallocate_impl(T* p, size_t old_size, size_t new_size, const void* hint, true_type)
{
    return reinterpret_cast<T*>(safe_realloc(p, new_size * sizeof(T)));
}


// !is_relocatable<T>::value
template <typename T>
T* crt_reallocate_impl(T* p, size_t old_size, size_t new_size, const void* hint, false_type)
{
    T* ptr = reinterpret_cast<T*>(crt_allocator_base::allocate(new_size, sizeof(T), hint));
    // use placement new to construct-in-place
    // Don't use `std::move`, since that move assigns into
    // uninitialized memory.
    for (size_t i = 0; i < old_size; ++i) {
        T& src = p[i];
        T* dst = &ptr[i];
        new (static_cast<void*>(dst)) T(std::move(src));
    }
    crt_allocator_base::deallocate(p, old_size * sizeof(T));
    return ptr;
}

// IMPLEMENTATION
// --------------


template <typename T>
template <typename U>
crt_allocator<T>::crt_allocator(const crt_allocator<U>&) noexcept
{}


template <typename T>
template <typename U>
auto crt_allocator<T>::operator=(const crt_allocator<U>&) noexcept -> self_t&
{
    return *this;
}


template <typename T>
auto crt_allocator<T>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(crt_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
auto crt_allocator<T>::reallocate(pointer p, size_type old_size, size_type new_size, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(crt_reallocate_impl(p, old_size, new_size, hint, is_relocatable<T> {}));
}


template <typename T>
void crt_allocator<T>::deallocate(pointer p, size_type n)
{
    crt_allocator_base::deallocate(p, sizeof(value_type) * n);
}


template <typename T, typename U>
bool operator==(const crt_allocator<T>&, const crt_allocator<U>&) noexcept
{
    return true;
}


template <typename T, typename U>
bool operator!=(const crt_allocator<T>& lhs, const crt_allocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
