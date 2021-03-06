//  :copyright: (c) 2016 Mariano Trebino.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Standard allocator analogous to `std::allocator`.
 *
 *  A shallow wrapper around `new` and `delete`. This allocator
 *  has poor performance, and therefore should be used sparingly.
 *
 *  \synopsis
 *      template <typename T>
 *      struct standard_allocator
 *      {
 *          using value_type = T;
 *
 *          standard_allocator() noexcept;
 *          standard_allocator(const self_t&) noexcept;
 *          template <typename U> standard_allocator(const standard_allocator<U>&) noexcept;
 *          self_t& operator=(const self_t&) noexcept;
 *          template <typename U> self_t& operator=(const standard_allocator<U>&) noexcept;
 *          ~standard_allocator() = default;
 *
 *          value_type* allocate(size_t n, const void* hint = nullptr);
 *          value_type* reallocate(value_type* p, size_t old_size, size_t new_size, const void* hint = nullptr);
 *          void deallocate(value_type* p, size_t n);
 *      };
 *
 *      using standard_resource = resource_adaptor<standard_allocator<byte>>;
 *
 *      template <typename T, typename U>
 *      inline bool operator==(const standard_allocator<T>&, const standard_allocator<U>&) noexcept;
 *
 *      template <typename T, typename U>
 *      inline bool operator!=(const standard_allocator<T>&, const standard_allocator<U>&) noexcept
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T>
struct standard_allocator;

// OBJECTS
// -------

/**
 *  \brief Base for standard memory allocator.
 */
struct standard_allocator_base
{
    static void* allocate(size_t n, size_t size, const void* hint = nullptr);
    static void deallocate(void* p, size_t n);
};


/**
 *  \brief Standard memory allocator.
 */
template <typename T>
struct standard_allocator: private standard_allocator_base
{
    // MEMBER TYPES
    // ------------
    using self_t = standard_allocator<T>;
    using value_type = T;
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    template <typename U> struct rebind { using other = polymorphic_allocator<U>; };
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // MEMBER FUNCTIONS
    // ----------------
    standard_allocator() noexcept = default;
    standard_allocator(const self_t&) noexcept = default;
    self_t& operator=(const self_t&) noexcept = default;
    ~standard_allocator() noexcept = default;

    template <typename U>
    standard_allocator(const standard_allocator<U>&) noexcept
    {}

    template <typename U>
    self_t& operator=(const standard_allocator<U>&) noexcept
    {
        return *this;
    }

    // ALLOCATOR TRAITS

    value_type* allocate(size_t n, const void* hint = nullptr)
    {
        return reinterpret_cast<value_type*>(standard_allocator_base::allocate(n, sizeof(value_type), hint));
    }

    void deallocate(value_type* p, size_t n)
    {
        standard_allocator_base::deallocate(p, sizeof(value_type) * n);
    }

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

using standard_resource = resource_adaptor<standard_allocator<byte>>;

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable<standard_allocator<T>>: true_type
{};

// IMPLEMENTATION
// --------------

template <typename T, typename U>
inline bool operator==(const standard_allocator<T>&, const standard_allocator<U>&) noexcept
{
    return true;
}


template <typename T, typename U>
inline bool operator!=(const standard_allocator<T>& lhs, const standard_allocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
