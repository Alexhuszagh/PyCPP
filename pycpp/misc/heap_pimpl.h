//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Traditional heap-based PIMPL idiom.
 *
 *  Traditional PIMPL idiom using pointer indirection to avoid
 *  requiring knowledge of the type layout or size, reducing
 *  compile-time dependencies and allowing ABI (Application
 *  Binary Interface) compatiblity even if the original type's
 *  size or layout changes.
 *
 *  The heap PIMPL contains two classes: one for shared memory
 *  ownership, and one for unique memory ownership. With shared
 *  semantics, copying the wrapper shares the a reference to the
 *  implied member with the original class. With unique semantics,
 *  copying the wrapper creates a shallow copy of the implied
 *  member.
 *
 *  The class should be used as a private member variable encapsulating
 *  the implied class in the public class. For example:
 *
 *  \code
 *      #include <pycpp/misc/heap_pimpl.h>
 *
 *      struct file_impl;
 *      struct file
 *      {
 *      public:
 *      private:
 *          unique_heap_pimpl<file_impl> impl_;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <cstddef>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// UNIQUE

/**
 *  \brief PIMPL idiom using pointer indirection and unique semantics.
 */
template <typename T>
class unique_heap_pimpl
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = unique_heap_pimpl<T>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // MEMBER FUNCTIONS
    // ----------------
    unique_heap_pimpl();
    unique_heap_pimpl(const self_t&);
    self_t& operator=(const self_t&);
    unique_heap_pimpl(self_t&&);
    self_t& operator=(self_t&&);
    unique_heap_pimpl(const value_type&);
    self_t& operator=(const value_type&);
    unique_heap_pimpl(value_type&&);
    self_t& operator=(value_type&&);

    // CONVERSIONS
    reference operator*() noexcept;
    const_reference operator*() const noexcept;
    pointer operator->() noexcept;
    const_pointer operator->() const noexcept;
    operator reference() noexcept;
    operator const_reference() const noexcept;
    reference get() noexcept;
    const_reference get() const noexcept;

    // MODIFIERS
    void swap(self_t&);

private:
    std::unique_ptr<T> t_ = nullptr;
};

// SHARED

/**
 *  \brief PIMPL idiom using pointer indirection and shared semantics.
 */
template <typename T>
class shared_heap_pimpl
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = shared_heap_pimpl<T>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // MEMBER FUNCTIONS
    // ----------------
    shared_heap_pimpl();
    shared_heap_pimpl(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    shared_heap_pimpl(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
    shared_heap_pimpl(const value_type&);
    self_t& operator=(const value_type&);
    shared_heap_pimpl(value_type&&);
    self_t& operator=(value_type&&);

    // CONVERSIONS
    reference operator*() noexcept;
    const_reference operator*() const noexcept;
    pointer operator->() noexcept;
    const_pointer operator->() const noexcept;
    operator reference() noexcept;
    operator const_reference() const noexcept;
    reference get() noexcept;
    const_reference get() const noexcept;

    // MODIFIERS
    void swap(self_t&);

private:
    std::shared_ptr<T> t_ = nullptr;
};

// IMPLEMENTATION
// --------------

// UNIQUE

template <typename T>
unique_heap_pimpl<T>::unique_heap_pimpl():
    t_(std::make_unique<value_type>())
{}


template <typename T>
unique_heap_pimpl<T>::unique_heap_pimpl(const self_t& rhs):
    t_(std::make_unique<value_type>(rhs.get()))
{}


template <typename T>
auto unique_heap_pimpl<T>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        t_ = std::make_unique<value_type>(rhs.get());
    }
    return *this;
}


template <typename T>
unique_heap_pimpl<T>::unique_heap_pimpl(self_t&& rhs):
    t_(std::move(rhs.t_))
{}


template <typename T>
auto unique_heap_pimpl<T>::operator=(self_t&& rhs) -> self_t&
{
    if (this != &rhs) {
        t_ = std::move(rhs.t_);
    }
    return *this;
}


template <typename T>
unique_heap_pimpl<T>::unique_heap_pimpl(const value_type& rhs):
    t_(std::make_unique<value_type>(rhs))
{}


template <typename T>
auto unique_heap_pimpl<T>::operator=(const value_type& rhs) -> self_t&
{
    *t_ = rhs;
    return *this;
}


template <typename T>
unique_heap_pimpl<T>::unique_heap_pimpl(value_type&& rhs):
    t_(std::make_unique<value_type>(std::move(rhs)))
{}


template <typename T>
auto unique_heap_pimpl<T>::operator=(value_type&& rhs) -> self_t&
{
    *t_ = std::move(rhs);
    return *this;
}


template <typename T>
auto unique_heap_pimpl<T>::operator*() noexcept -> reference
{
    return get();
}


template <typename T>
auto unique_heap_pimpl<T>::operator*() const noexcept -> const_reference
{
    return get();
}


template <typename T>
auto unique_heap_pimpl<T>::operator->() noexcept -> pointer
{
    return &get();
}

template <typename T>
auto unique_heap_pimpl<T>::operator->() const noexcept -> const_pointer
{
    return &get();
}


template <typename T>
unique_heap_pimpl<T>::operator reference() noexcept
{
    return get();
}


template <typename T>
unique_heap_pimpl<T>::operator const_reference() const noexcept
{
    return get();
}


template <typename T>
auto unique_heap_pimpl<T>::get() noexcept -> reference
{
    return *t_;
}


template <typename T>
auto unique_heap_pimpl<T>::get() const noexcept -> const_reference
{
    return *t_;
}


template <typename T>
void unique_heap_pimpl<T>::swap(self_t& rhs)
{
    std::swap(t_, rhs.t_);
}

// SHARED

template <typename T>
shared_heap_pimpl<T>::shared_heap_pimpl():
    t_(std::make_shared<value_type>())
{}


template <typename T>
shared_heap_pimpl<T>::shared_heap_pimpl(const value_type& rhs):
    t_(std::make_shared<value_type>(rhs))
{}


template <typename T>
auto shared_heap_pimpl<T>::operator=(const value_type& rhs) -> self_t&
{
    *t_ = rhs;
    return *this;
}


template <typename T>
shared_heap_pimpl<T>::shared_heap_pimpl(value_type&& rhs):
    t_(std::make_shared<value_type>(std::move(rhs)))
{}


template <typename T>
auto shared_heap_pimpl<T>::operator=(value_type&& rhs) -> self_t&
{
    *t_ = std::move(rhs);
    return *this;
}


template <typename T>
auto shared_heap_pimpl<T>::operator*() noexcept -> reference
{
    return get();
}


template <typename T>
auto shared_heap_pimpl<T>::operator*() const noexcept -> const_reference
{
    return get();
}


template <typename T>
auto shared_heap_pimpl<T>::operator->() noexcept -> pointer
{
    return &get();
}

template <typename T>
auto shared_heap_pimpl<T>::operator->() const noexcept -> const_pointer
{
    return &get();
}


template <typename T>
shared_heap_pimpl<T>::operator reference() noexcept
{
    return get();
}


template <typename T>
shared_heap_pimpl<T>::operator const_reference() const noexcept
{
    return get();
}


template <typename T>
auto shared_heap_pimpl<T>::get() noexcept -> reference
{
    return *t_;
}


template <typename T>
auto shared_heap_pimpl<T>::get() const noexcept -> const_reference
{
    return *t_;
}


template <typename T>
void shared_heap_pimpl<T>::swap(self_t& rhs)
{
    std::swap(t_, rhs.t_);
}

PYCPP_END_NAMESPACE
