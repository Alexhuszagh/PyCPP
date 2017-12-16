//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast PIMPL idiom without pointer overhead.
 *
 *  Fast PIMPL idiom using a pre-defined stack buffer of a custom
 *  size to avoid dynamic allocation. This avoids additional
 *  allocation required for PIMPL, enhancing performance.
 *  However, the type size **must** be known during template
 *  instantiation, and therefore loses the ABI (Application
 *  Binary Interface) compatibility PIMPL is frequently used
 *  for.
 *
 *  To avoid changes in class layout or size leading to undefined
 *  behavior, the destructor includes a static assert to ensure
 *  the alignment and type-size are compatible.
 *
 *  The class should be used as a private member variable encapsulating
 *  the implied class in the public class. For example:
 *
 *  \code
 *      #include <pycpp/misc/stack_pimpl.h>
 *
 *      struct file_impl;
 *      struct file
 *      {
 *      public:
 *      private:
 *          stack_pimpl<file_impl> impl_;
 *      };
 *
 *  The design is based off of Herb Sutter's GotW:
 *      http://www.gotw.ca/gotw/028.htm
 *
 *  The implementation is refined off of Malte Skarupke type-safe
 *  PIMPL implementation:
 *      https://probablydance.com/2013/10/05/type-safe-pimpl-implementation-without-overhead/
 *
 *  For the pitfalls on `aligned_storage`, read:
 *      https://whereswalden.com/tag/stdaligned_storage/
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

namespace pimp_detail
{
// DETAIL
// ------


// Static check to ensure the type is properly sized and
// aligned to avoid any undefined behavior. Ensure
// the size is exactly equal, and that the alignment
// is at least as strict as the type alignment.
// Larger alignments are stricter on the memory locations
// they can be placed, and any stricter alignment can
// be used in place of a weaker one, according to the C standard.

template <typename T, size_t Size, size_t Alignment>
inline void assert_storage()
{
    static_assert(sizeof(T) == Size, "");
    static_assert(alignof(T) <= Alignment, "");
}


template <typename T, size_t Size, size_t Alignment>
struct storage_asserter
{
    inline storage_asserter()
    {
        assert_storage<T, Size, Alignment>();
    }
};

}   /* pimp_detail */

// DECLARATION
// -----------

/**
 *  \brief PIMPL idiom using aligned storage to avoid dynamic allocation.
 */
template <
    typename T,
    size_t Size = sizeof(T),
    size_t Alignment = alignof(max_align_t)
>
class stack_pimpl
{
public:
    // MEMBER VARIABLES
    // ----------------
    static constexpr size_t size = Size;
    static constexpr size_t alignment = Alignment;

    // MEMBER TYPES
    // ------------
    using self_t = stack_pimpl<T, size, alignment>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // MEMBER FUNCTIONS
    // ----------------
    stack_pimpl();
    stack_pimpl(const self_t&);
    self_t& operator=(const self_t&);
    stack_pimpl(self_t&&);
    self_t& operator=(self_t&&);
    stack_pimpl(const value_type&);
    self_t& operator=(const value_type&);
    stack_pimpl(value_type&&);
    self_t& operator=(value_type&&);
    ~stack_pimpl();

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
    using memory_type = aligned_storage_t<Size, Alignment>;

    memory_type mem_;
};


// IMPLEMENTATION
// --------------

template <typename T, size_t Size, size_t Alignment>
const size_t stack_pimpl<T, Size, Alignment>::size;

template <typename T, size_t Size, size_t Alignment>
const size_t stack_pimpl<T, Size, Alignment>::alignment;

template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::stack_pimpl()
{
    new (&get()) value_type();
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::stack_pimpl(const self_t& rhs)
{
    new (&get()) value_type(rhs.get());
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        get() = rhs.get();
    }
    return *this;
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::stack_pimpl(self_t&& rhs)
{
    new (&get()) value_type(move(rhs.get()));
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator=(self_t&& rhs) -> self_t&
{
    if (this != &rhs) {
        get() = move(rhs.get());
    }
    return *this;
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::stack_pimpl(const value_type& rhs)
{
    new (&get()) value_type(rhs);
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator=(const value_type& rhs) -> self_t&
{
    get() = rhs;
    return *this;
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::stack_pimpl(value_type&& rhs)
{
    new (&get()) value_type(move(rhs));
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator=(value_type&& rhs) -> self_t&
{
    get() = move(rhs);
    return *this;
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::~stack_pimpl()
{
    pimp_detail::storage_asserter<T, Size, Alignment> {};
    get().~T();
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator*() noexcept -> reference
{
    return get();
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator*() const noexcept -> const_reference
{
    return get();
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator->() noexcept -> pointer
{
    return &get();
}

template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::operator->() const noexcept -> const_pointer
{
    return &get();
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::operator reference() noexcept
{
    return get();
}


template <typename T, size_t Size, size_t Alignment>
stack_pimpl<T, Size, Alignment>::operator const_reference() const noexcept
{
    return get();
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::get() noexcept -> reference
{
    return reinterpret_cast<reference>(mem_);
}


template <typename T, size_t Size, size_t Alignment>
auto stack_pimpl<T, Size, Alignment>::get() const noexcept -> const_reference
{
    return reinterpret_cast<const_reference>(mem_);
}


template <typename T, size_t Size, size_t Alignment>
void stack_pimpl<T, Size, Alignment>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(get(), rhs.get());
}

PYCPP_END_NAMESPACE
