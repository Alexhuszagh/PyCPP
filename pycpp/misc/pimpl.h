//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Pimpl-like implementation without pointer overhead.
 */

#pragma once

#include <pycpp/config.h>
#include <cstdlib>
#include <type_traits>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief pimpl idiom using aligned storage for direct stack allocation.
 *
 *  \warning Since the size may be unknown at template instantiation,
 *  there are no static checks for the type-size. It is highly
 *  recommend you include static_asserts for the type size in compilation
 *  units and use fixed-size types.
 */
template <typename T, size_t S = sizeof(T)>
class pimpl
{
public:
    // MEMBER TYPES
    // ------------
    typedef pimpl<T, S> self;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------
    pimpl();
    ~pimpl();
    pimpl(const self&);
    self& operator=(const self&);
    pimpl(pimpl&&);
    self& operator=(self&&);
    pimpl(const value_type&);
    self& operator=(const value_type&);
    pimpl(value_type&&);
    self& operator=(value_type&&);

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
    operator const_reference() const;

    void swap(self&);
    void reset();

private:
    typedef typename std::aligned_storage<S>::type memory_type;

    memory_type mem_;
    bool destroy_;
};


// IMPLEMENTATION
// --------------

template <typename T, size_t S>
pimpl<T, S>::pimpl():
    destroy_(false)
{}


template <typename T, size_t S>
pimpl<T, S>::~pimpl()
{
    reset();
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(const self& other):
    destroy_(other.destroy_)
{
    if (destroy_) {
        new(reinterpret_cast<pointer>(&mem_)) value_type(*other);
    }
}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(const self& other) -> self&
{
    reset();
    destroy_ = other.destroy_;
    if (destroy_) {
        new(reinterpret_cast<pointer>(&mem_)) value_type(*other);
    }
    return *this;
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(self&& other)
{
    destroy_ = std::move(other.destroy_);
    if (destroy_) {
        new(reinterpret_cast<pointer>(&mem_)) value_type(std::move(*other));
    }
}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(self&& other) -> self&
{
    reset();
    destroy_ = std::move(other.destroy_);
    if (destroy_) {
        new(reinterpret_cast<pointer>(&mem_)) value_type(std::move(*other));
    }
    return *this;
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(const value_type& other)
{
    destroy_ = true;
    new(reinterpret_cast<pointer>(&mem_)) value_type(other);
}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(const value_type& other) -> self&
{
    reset();
    destroy_ = true;
    new(reinterpret_cast<pointer>(&mem_)) value_type(other);
    return *this;
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(value_type&& other)
{
    destroy_ = true;
    new(reinterpret_cast<pointer>(&mem_)) value_type(std::move(other));
}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(value_type&& other) -> self&
{
    reset();
    destroy_ = true;
    new(reinterpret_cast<pointer>(&mem_)) value_type(std::move(other));
    return *this;
}


template <typename T, size_t S>
auto pimpl<T, S>::operator*() -> reference
{
    return reinterpret_cast<reference>(mem_);
}


template <typename T, size_t S>
auto pimpl<T, S>::operator*() const -> const_reference
{
    return reinterpret_cast<const_reference>(mem_);
}


template <typename T, size_t S>
auto pimpl<T, S>::operator->() -> pointer
{
    return reinterpret_cast<pointer>(&mem_);
}

template <typename T, size_t S>
auto pimpl<T, S>::operator->() const -> const_pointer
{
    return reinterpret_cast<const_pointer>(&mem_);
}


template <typename T, size_t S>
pimpl<T, S>::operator const_reference() const
{
    return reinterpret_cast<const_reference>(mem_);
}


template <typename T, size_t S>
void pimpl<T, S>::swap(self& other)
{
    std::swap(destroy_, other.destroy_);
    std::swap(**this, *other);
}


template <typename T, size_t S>
void pimpl<T, S>::reset()
{
    if (destroy_) {
        reinterpret_cast<pointer>(&mem_)->~T();
    }
    destroy_ = false;
}

PYCPP_END_NAMESPACE
