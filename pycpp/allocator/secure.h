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
#include <memory>

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
    size_t max_size(size_t size) const noexcept;
};


/**
 *  \brief Secure memory allocator.
 */
template <typename T>
struct secure_allocator: private secure_allocator_base
{
    // MEMBER TEMPLATES
    // ----------------

    template <typename U>
    using other = secure_allocator<U>;

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
    secure_allocator() noexcept;
    secure_allocator(const secure_allocator<T>&) noexcept;
    template <typename U> secure_allocator(const secure_allocator<U>&) noexcept;
    ~secure_allocator();

    pointer address(reference) const noexcept;
    const_pointer address(const_reference) const noexcept;
    pointer allocate(size_type, const void* = nullptr);
    template <typename U, class... Ts> void construct(U*, Ts&&...);
    void deallocate(pointer, size_type);
    template <typename U> void destroy(U*);
    size_type max_size() const noexcept;

    // STATIC
    // ------
    static pointer allocate(const secure_allocator<T>&, size_type, const void* = nullptr);
    template<typename U, class... Ts> static void construct(const secure_allocator<U>&, U*, Ts&&...);
    static void deallocate(const secure_allocator<T>&, pointer p, size_type);
    template<typename U> static void destroy(const secure_allocator<U>&, U*);
    static size_type max_size(const secure_allocator<T>&) noexcept;
};


// IMPLEMENTATION
// --------------


template <typename T>
secure_allocator<T>::secure_allocator() noexcept
{}


template <typename T>
secure_allocator<T>::secure_allocator(const secure_allocator<T>&) noexcept
{}


template <typename T>
template <typename U>
secure_allocator<T>::secure_allocator(const secure_allocator<U>&) noexcept
{}


template <typename T>
secure_allocator<T>::~secure_allocator()
{}


template <typename T>
auto secure_allocator<T>::address(reference t) const noexcept -> pointer
{
    return std::addressof(t);
}


template <typename T>
auto secure_allocator<T>::address(const_reference t) const noexcept -> const_pointer
{
    return std::addressof(t);
}


template <typename T>
auto secure_allocator<T>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(secure_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
template<typename U, class... Ts>
void secure_allocator<T>::construct(U* p, Ts&&... ts)
{
    new ((void*)p) value_type(std::forward<Ts>(ts)...);
}


template <typename T>
void secure_allocator<T>::deallocate(pointer p, size_type n)
{
    secure_allocator_base::deallocate(p, n);
}


template <typename T>
template <typename U>
void secure_allocator<T>::destroy(U* p)
{
    p->~U();
    secure_allocator_base::zero(p, sizeof(U));
}


template <typename T>
auto secure_allocator<T>::max_size() const noexcept -> size_type
{
    return secure_allocator_base::max_size(sizeof(value_type));
}


template <typename T>
auto secure_allocator<T>::allocate(const secure_allocator<T>& a, size_type n, const void* p) -> pointer
{
    return a.allocate(n, p);
}


template <typename T>
template <typename U, class... Ts>
void secure_allocator<T>::construct(const secure_allocator<U>& a, U* p, Ts&&... ts)
{
    a.construct(p, std::forward<Ts>(ts)...);
}


template <typename T>
void secure_allocator<T>::deallocate(const secure_allocator<T>& a, pointer p, size_type n)
{
    a.deallocate(p, n);
}


template <typename T>
template <typename U>
void secure_allocator<T>::destroy(const secure_allocator<U>& a, U* p)
{
    a.destroy(p);
}


template <typename T>
auto secure_allocator<T>::max_size(const secure_allocator<T>& other) noexcept -> size_type
{
    return other.max_size();
}

PYCPP_END_NAMESPACE
