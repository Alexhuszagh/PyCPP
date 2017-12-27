//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Null memory allocator.
 *
 *  An allocator that throws if memory is allocated or deallocated.
 *  `null_allocator` is useful for chaining allocators, where a
 *  condition is raised that should not occur: rather than use an
 *  inefficient fallback (such as malloc), the logic may be
 *  reworked later for a more efficient allocator.
 *
 *  \synopsis
 *      template <typename T>
 *      struct null_allocator
 *      {
 *          using value_type = T;
 *
 *          null_allocator() noexcept;
 *          null_allocator(const self_t&) noexcept;
 *          template <typename U> null_allocator(const null_allocator<U>&) noexcept;
 *          self_t& operator=(const self_t&) noexcept;
 *          template <typename U> self_t& operator=(const null_allocator<U>&) noexcept;
 *          ~null_allocator() = default;
 *
 *          value_type* allocate(size_t n, const void* hint = nullptr);
 *          value_type* reallocate(value_type* p, size_t old_size, size_t new_size, const void* hint = nullptr);
 *          void deallocate(value_type* p, size_t n);
 *      };
 *
 *      using null_resource = resource_adaptor<null_allocator<byte>>;
 *
 *      template <typename T, typename U>
 *      inline bool operator==(const null_allocator<T>&, const null_allocator<U>&) noexcept;
 *
 *      template <typename T, typename U>
 *      inline bool operator!=(const null_allocator<T>&, const null_allocator<U>&) noexcept
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <pycpp/stl/utility.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T>
struct null_allocator;

// OBJECTS
// -------

/**
 *  \brief Standard memory allocator.
 */
template <typename T>
struct null_allocator
{
    // MEMBER TYPES
    // ------------
    using self_t = null_allocator<T>;
    using value_type = T;
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    template <typename U> struct rebind { using other = null_allocator<U>; };
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS

    null_allocator() noexcept = default;
    null_allocator(const self_t&) noexcept = default;
    self_t& operator=(const self_t&) noexcept = default;
    ~null_allocator() = default;

    template <typename U>
    null_allocator(const null_allocator<U>&) noexcept
    {}

    template <typename U>
    self_t& operator=(const null_allocator<U>&) noexcept
    {
        return *this;
    }

    // ALLOCATOR TRAITS

    value_type* allocate(size_t n, const void* hint = nullptr)
    {
        throw bad_alloc();
    }

    void deallocate(value_type* p, size_t n)
    {}

#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)

    template <typename ... Ts>
    void construct(T* p, Ts&&... ts)
    {
        ::new (static_cast<void*>(p)) T(std::forward<Ts>(ts)...);
    }

    void destroy(T* p)
    {
        p->~T();
    }

    size_type max_size()
    {
        return std::numeric_limits<size_type>::max();
    }

#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS
};

// ALIAS
// -----

using null_resource = resource_adaptor<null_allocator<byte>>;

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable<null_allocator<T>>: true_type
{};

// NON-MEMBER FUNCTIONS
// --------------------

template <typename T, typename U>
inline bool operator==(const null_allocator<T>&, const null_allocator<U>&) noexcept
{
    return true;
}


template <typename T, typename U>
inline bool operator!=(const null_allocator<T>& lhs, const null_allocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
