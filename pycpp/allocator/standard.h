//  :copyright: (c) 2016 Mariano Trebino.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Standard allocator analogous to `std::allocator`.
 *
 *  A shallow wrapper around `malloc` and `free`. This allocator
 *  has poor performance, and therefore should be used sparingly.
 */

#pragma once

#include <pycpp/config.h>
#include <cstddef>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Base for standard memory allocator.
 */
struct standard_allocator_base
{
    void* allocate(size_t n, size_t size, const void* hint = nullptr);
    void deallocate(void* p, size_t n);
    size_t max_size(size_t size) const noexcept;
};


/**
 *  \brief Standard memory allocator.
 */
template <typename T>
struct standard_allocator: private standard_allocator_base
{
    // MEMBER TEMPLATES
    // ----------------

    template <typename U>
    using other = standard_allocator<U>;

    // MEMBER TYPES
    // ------------
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    standard_allocator() noexcept;
    standard_allocator(const standard_allocator<T>&) noexcept;
    template <typename U> standard_allocator(const standard_allocator<U>&) noexcept;
    ~standard_allocator();

    pointer address(reference) const noexcept;
    const_pointer address(const_reference) const noexcept;
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
    template <typename U, class... Ts> void construct(U*, Ts&&...);
    template <typename U> void destroy(U*);
    size_type max_size() const noexcept;

    // STATIC
    // ------
    static pointer allocate(const standard_allocator<T>&, size_type, const void* = nullptr);
    static void deallocate(const standard_allocator<T>&, pointer p, size_type);
    template<typename U, class... Ts> static void construct(const standard_allocator<U>&, U*, Ts&&...);
    template<typename U> static void destroy(const standard_allocator<U>&, U*);
    static size_type max_size(const standard_allocator<T>&) noexcept;
};


// IMPLEMENTATION
// --------------


template <typename T>
standard_allocator<T>::standard_allocator() noexcept
{}


template <typename T>
standard_allocator<T>::standard_allocator(const standard_allocator<T>&) noexcept
{}


template <typename T>
template <typename U>
standard_allocator<T>::standard_allocator(const standard_allocator<U>&) noexcept
{}


template <typename T>
standard_allocator<T>::~standard_allocator()
{}


template <typename T>
auto standard_allocator<T>::address(reference t) const noexcept -> pointer
{
    return std::addressof(t);
}


template <typename T>
auto standard_allocator<T>::address(const_reference t) const noexcept -> const_pointer
{
    return std::addressof(t);
}


template <typename T>
auto standard_allocator<T>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(standard_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
void standard_allocator<T>::deallocate(pointer p, size_type n)
{
    standard_allocator_base::deallocate(p, n);
}


template <typename T>
template<typename U, class... Ts>
void standard_allocator<T>::construct(U* p, Ts&&... ts)
{
    new ((void*)p) value_type(std::forward<Ts>(ts)...);
}


template <typename T>
template <typename U>
void standard_allocator<T>::destroy(U* p)
{
    p->~U();
}


template <typename T>
auto standard_allocator<T>::max_size() const noexcept -> size_type
{
    return standard_allocator_base::max_size(sizeof(value_type));
}


template <typename T>
auto standard_allocator<T>::allocate(const standard_allocator<T>& a, size_type n, const void* p) -> pointer
{
    return a.allocate(n, p);
}


template <typename T>
void standard_allocator<T>::deallocate(const standard_allocator<T>& a, pointer p, size_type n)
{
    a.deallocate(p, n);
}


template <typename T>
template <typename U, class... Ts>
void standard_allocator<T>::construct(const standard_allocator<U>& a, U* p, Ts&&... ts)
{
    a.construct(p, std::forward<Ts>(ts)...);
}


template <typename T>
template <typename U>
void standard_allocator<T>::destroy(const standard_allocator<U>& a, U* p)
{
    a.destroy(p);
}


template <typename T>
auto standard_allocator<T>::max_size(const standard_allocator<T>& other) noexcept -> size_type
{
    return other.max_size();
}

PYCPP_END_NAMESPACE
