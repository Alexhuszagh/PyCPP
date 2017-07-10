//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure C++-style memory allocator.
 *
 *  Provides wrappers around libsodium to enable secure memory
 *  allocation and deallocation. Memory is...
 */

#pragma once

#include <pycpp/secure/util.h>
#include <limits>
#include <memory>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// DECLARATION
// -----------

/**
 *  \brief Secure memory allocator.
 */
template <typename T>
struct secure_allocator
{
    // MEMBER TEMPLATES
    // ----------------

    template <typename U>
    using other = secure_allocator<U>;

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
    if (n > max_size()) {
        throw std::runtime_error("allocator<T>::allocate(size_t n) exceeds max size.");
    }

    return reinterpret_cast<pointer>(secure_calloc(n, sizeof(T)));
}


template <typename T>
template<typename U, class... Ts>
void secure_allocator<T>::construct(U* p, Ts&&... ts)
{
    new ((void*)p) T(std::forward<Ts>(ts)...);
}


template <typename T>
void secure_allocator<T>::deallocate(pointer p, size_type n)
{
    secure_free(p);
}


template <typename T>
template <typename U>
void secure_allocator<T>::destroy(U* p)
{
    p->~U();
    secure_zero(p, sizeof(U));
}


template <typename T>
auto secure_allocator<T>::max_size() const noexcept -> size_type
{
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
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
