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
#include <pycpp/stl/utility.h>
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
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    template <typename U> struct rebind { using other = crt_allocator<U>; };
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // MEMBER FUNCTIONS
    // ----------------
    crt_allocator() noexcept = default;
    crt_allocator(const self_t&) noexcept = default;
    template <typename U> crt_allocator(const crt_allocator<U>&) noexcept;
    self_t& operator=(const self_t&) noexcept = default;
    template <typename U> self_t& operator=(const crt_allocator<U>&) noexcept;
    ~crt_allocator() = default;

    // ALLOCATOR TRAITS
    value_type* allocate(size_t, const void* = nullptr);
    value_type* reallocate(value_type*, size_t, size_t, const void* = nullptr);
    void deallocate(value_type*, size_t);
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    template <typename ... Ts>
    void construct(T* p, Ts&&... ts) { ::new (static_cast<void*>(p)) T(std::forward<Ts>(ts)...); }
    void destroy(T* p) { p->~T(); }
    size_type max_size() { return std::numeric_limits<size_type>::max(); }
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS
};

// ALIAS
// -----

using crt_resource = resource_adaptor<crt_allocator<byte>>;

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable<crt_allocator<T>>: true_type
{};

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
    // Don't use `move`, since that move assigns into
    // uninitialized memory.
    for (size_t i = 0; i < old_size; ++i) {
        T& src = p[i];
        T* dst = &ptr[i];
        new (static_cast<void*>(dst)) T(move(src));
    }
    crt_allocator_base::deallocate(p, old_size * sizeof(T));
    return ptr;
}

// IMPLEMENTATION
// --------------


template <typename T>
template <typename U>
inline crt_allocator<T>::crt_allocator(const crt_allocator<U>&) noexcept
{}


template <typename T>
template <typename U>
inline auto crt_allocator<T>::operator=(const crt_allocator<U>&) noexcept -> self_t&
{
    return *this;
}


template <typename T>
inline auto crt_allocator<T>::allocate(size_t n, const void* hint) -> value_type*
{
    return reinterpret_cast<value_type*>(crt_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
inline auto crt_allocator<T>::reallocate(value_type* p, size_t old_size, size_t new_size, const void* hint) -> value_type*
{
    return reinterpret_cast<value_type*>(crt_reallocate_impl(p, old_size, new_size, hint, is_relocatable<T> {}));
}


template <typename T>
inline void crt_allocator<T>::deallocate(value_type* p, size_t n)
{
    crt_allocator_base::deallocate(p, sizeof(value_type) * n);
}


template <typename T, typename U>
inline bool operator==(const crt_allocator<T>&, const crt_allocator<U>&) noexcept
{
    return true;
}


template <typename T, typename U>
inline bool operator!=(const crt_allocator<T>& lhs, const crt_allocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
