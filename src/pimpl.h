//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Pimpl-like implementation without pointer overhead.
 */

#pragma once

#include <cstdlib>
#include <type_traits>
#include <utility>


// DECLARATION
// -----------


/**
 *  \brief pimpl idiom using aligned storage for direct stack allocation.
 *
 *  Since the size may be unknown at template instantiation, there
 *  are no static checks for
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

private:
    typedef typename std::aligned_storage<S>::type memory_type;
    memory_type mem;
};


// IMPLEMENTATION
// --------------

template <typename T, size_t S>
pimpl<T, S>::pimpl()
{}

template <typename T, size_t S>
pimpl<T, S>::pimpl(const self& other):
    mem(other.mem)
{}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(const self& other) -> self&
{
    mem = other.mem;
    return *this;
}

template <typename T, size_t S>
pimpl<T, S>::pimpl(pimpl&& other):
    mem(std::move(other.mem))
{}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(self&& other) -> self&
{
    mem = std::move(other.mem);
    return *this;
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(const value_type& other):
    mem(reinterpret_cast<const memory_type&>(other))
{}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(const value_type& other) -> self&
{
    **this = other;
    return *this;
}


template <typename T, size_t S>
pimpl<T, S>::pimpl(value_type&& other):
    mem(std::move(reinterpret_cast<memory_type&&>(other)))
{}


template <typename T, size_t S>
auto pimpl<T, S>::operator=(value_type&& other) -> self&
{
    **this = std::move(other);
    return *this;
}


template <typename T, size_t S>
auto pimpl<T, S>::operator*() -> reference
{
    return reinterpret_cast<reference>(mem);
}

template <typename T, size_t S>
auto pimpl<T, S>::operator*() const -> const_reference
{
    return reinterpret_cast<const_reference>(mem);
}

template <typename T, size_t S>
auto pimpl<T, S>::operator->() -> pointer
{
    return reinterpret_cast<pointer>(&mem);
}

template <typename T, size_t S>
auto pimpl<T, S>::operator->() const -> const_pointer
{
    return reinterpret_cast<const_pointer>(&mem);
}


template <typename T, size_t S>
pimpl<T, S>::operator const_reference() const
{
    return reinterpret_cast<const_reference>(mem);
}


template <typename T, size_t S>
void pimpl<T, S>::swap(self& other)
{
    std::swap(mem, other.mem);
}
