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
 *  By default, the heap PIMPL wrappers use `allocator` for object
 *  construction and deletion, which adds overhead relative to
 *  `std::allocator`, when `USE_POLYMORPHIC_ALLOCATOR` is defined.
 *  Providing an empty allocator makes these classes as efficient
 *  as the underlying smart pointers.
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

#include <pycpp/stl/memory.h>
#include <pycpp/stl/utility.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// UNIQUE


/**
 *  \brief Specialize memory manager for the unique heap PIMPL wrapper.
 *  Use inheritance for empty-base class optimization.
 */
template <
    typename T,
    typename Allocator = allocator<T>,
    bool CheckNull = false
>
struct unique_heap_pimpl_manager: Allocator
{
    // MEMBER TEMPLATES
    // ----------------
    template <
        typename T1,
        typename A1 = typename allocator_traits<Allocator>::template rebind_alloc<T1>,
        bool C1 = CheckNull
    >
    struct rebind { using other = unique_heap_pimpl_manager<T1, A1, C1>; };

    // MEMBER TYPES
    // ------------
    using value_type = T;
    using allocator_type = Allocator;

    // MEMBER VARIABLES
    // ----------------
    static constexpr bool check_null = CheckNull;

    // MEMBER FUNCTIONS
    // ----------------

    unique_heap_pimpl_manager(const Allocator& alloc):
        Allocator(alloc)
    {}

    // WARNING: Naming static functions the same as non-static
    // functions is prohibited by the standard.
    template <typename ... Ts>
    static inline T* create_static(const Allocator& alloc, Ts&&... ts)
    {
        return allocate_and_construct<T>(alloc, forward<Ts>(ts)...);
    }

    template <typename ... Ts>
    inline T* create(Ts&&... ts) const
    {
        return unique_heap_pimpl_manager::create_static(static_cast<const Allocator&>(*this), forward<Ts>(ts)...);
    }

    template <bool B = CheckNull>
    enable_if_t<B, void>
    static inline destroy_static(const Allocator& alloc, T* p)
    {
        if (p) {
            destroy_and_deallocate(alloc, p);
        }
    }

    template <bool B = CheckNull>
    enable_if_t<!B, void>
    static inline destroy_static(const Allocator& alloc, T* p)
    {
        destroy_and_deallocate(alloc, p);
    }

    inline void destroy(T* p) const
    {
        unique_heap_pimpl_manager::destroy_static(static_cast<const Allocator&>(*this), p);
    }

    inline void operator()(T* p) const
    {
        destroy(p);
    }
};


/**
 *  \brief PIMPL idiom using pointer indirection and unique semantics.
 */
template <
    typename T,
    typename Allocator = allocator<T>
>
class unique_heap_pimpl
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = unique_heap_pimpl<T, Allocator>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using allocator_type = Allocator;
    using deleter_type = unique_heap_pimpl_manager<T, Allocator>;

    // MEMBER FUNCTIONS
    // ----------------
    unique_heap_pimpl(const allocator_type& = allocator_type());
    unique_heap_pimpl(const self_t&, const allocator_type& = allocator_type());
    self_t& operator=(const self_t&);
    unique_heap_pimpl(self_t&&, const allocator_type& = allocator_type()) noexcept;
    self_t& operator=(self_t&&) noexcept;
    unique_heap_pimpl(const value_type&, const allocator_type& = allocator_type());
    self_t& operator=(const value_type&);
    unique_heap_pimpl(value_type&&, const allocator_type& = allocator_type());
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
    void swap(self_t&) noexcept;

private:
    unique_ptr<value_type, deleter_type> t_ = nullptr;
};

// SHARED

/**
 *  \brief PIMPL idiom using pointer indirection and shared semantics.
 */
template <
    typename T,
    typename Allocator = allocator<T>
>
class shared_heap_pimpl
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = shared_heap_pimpl<T, Allocator>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using allocator_type = Allocator;

    // MEMBER FUNCTIONS
    // ----------------
    shared_heap_pimpl(const allocator_type& allocator = allocator_type());
    shared_heap_pimpl(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    shared_heap_pimpl(self_t&&) noexcept = default;
    self_t& operator=(self_t&&) noexcept = default;
    shared_heap_pimpl(const value_type&, const allocator_type& allocator = allocator_type());
    self_t& operator=(const value_type&);
    shared_heap_pimpl(value_type&&, const allocator_type& allocator = allocator_type());
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
    void swap(self_t&) noexcept;

private:
    shared_ptr<T> t_ = nullptr;
};

// SPECIALIZATION
// --------------

template <typename T, typename A>
struct is_relocatable<unique_heap_pimpl<T, A>>:
    is_relocatable<unique_ptr<T, unique_heap_pimpl_manager<T, A>>>
{};

template <typename T>
struct is_relocatable<shared_heap_pimpl<T>>: is_relocatable<shared_ptr<T>>
{};

// IMPLEMENTATION
// --------------

// MANAGER

template <typename T, typename A, bool C>
const bool unique_heap_pimpl_manager<T, A, C>::check_null;

// UNIQUE

template <typename T, typename A>
unique_heap_pimpl<T, A>::unique_heap_pimpl(const allocator_type& allocator):
    t_(deleter_type::create_static(allocator), deleter_type(allocator))
{}


template <typename T, typename A>
unique_heap_pimpl<T, A>::unique_heap_pimpl(const self_t& rhs, const allocator_type& allocator):
    t_(deleter_type::create_static(allocator, rhs.get()), deleter_type(allocator))
{}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        t_.reset(t_.get_deleter().create(rhs.get()));
    }
    return *this;
}


template <typename T, typename A>
unique_heap_pimpl<T, A>::unique_heap_pimpl(self_t&& rhs, const allocator_type&) noexcept:
    t_(move(rhs.t_))
{}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator=(self_t&& rhs) noexcept -> self_t&
{
    if (this != &rhs) {
        t_ = move(rhs.t_);
    }
    return *this;
}


template <typename T, typename A>
unique_heap_pimpl<T, A>::unique_heap_pimpl(const value_type& rhs, const allocator_type& allocator):
    t_(deleter_type::create_static(allocator, rhs), deleter_type(allocator))
{}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator=(const value_type& rhs) -> self_t&
{
    *t_ = rhs;
    return *this;
}


template <typename T, typename A>
unique_heap_pimpl<T, A>::unique_heap_pimpl(value_type&& rhs, const allocator_type& allocator):
    t_(deleter_type::create_static(allocator, move(rhs)), deleter_type(allocator))
{}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator=(value_type&& rhs) -> self_t&
{
    *t_ = move(rhs);
    return *this;
}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator*() noexcept -> reference
{
    return get();
}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator*() const noexcept -> const_reference
{
    return get();
}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator->() noexcept -> pointer
{
    return &get();
}

template <typename T, typename A>
auto unique_heap_pimpl<T, A>::operator->() const noexcept -> const_pointer
{
    return &get();
}


template <typename T, typename A>
unique_heap_pimpl<T, A>::operator reference() noexcept
{
    return get();
}


template <typename T, typename A>
unique_heap_pimpl<T, A>::operator const_reference() const noexcept
{
    return get();
}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::get() noexcept -> reference
{
    return *t_;
}


template <typename T, typename A>
auto unique_heap_pimpl<T, A>::get() const noexcept -> const_reference
{
    return *t_;
}


template <typename T, typename A>
void unique_heap_pimpl<T, A>::swap(self_t& rhs) noexcept
{
    swap(t_, rhs.t_);
}

// SHARED

template <typename T, typename A>
shared_heap_pimpl<T, A>::shared_heap_pimpl(const allocator_type& allocator):
    t_(allocate_shared<value_type>(allocator))
{}


template <typename T, typename A>
shared_heap_pimpl<T, A>::shared_heap_pimpl(const value_type& rhs, const allocator_type& allocator):
    t_(allocate_shared<value_type>(allocator, rhs))
{}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator=(const value_type& rhs) -> self_t&
{
    *t_ = rhs;
    return *this;
}


template <typename T, typename A>
shared_heap_pimpl<T, A>::shared_heap_pimpl(value_type&& rhs, const allocator_type& allocator):
    t_(allocate_shared<value_type>(allocator, move(rhs)))
{}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator=(value_type&& rhs) -> self_t&
{
    *t_ = move(rhs);
    return *this;
}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator*() noexcept -> reference
{
    return get();
}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator*() const noexcept -> const_reference
{
    return get();
}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator->() noexcept -> pointer
{
    return &get();
}

template <typename T, typename A>
auto shared_heap_pimpl<T, A>::operator->() const noexcept -> const_pointer
{
    return &get();
}


template <typename T, typename A>
shared_heap_pimpl<T, A>::operator reference() noexcept
{
    return get();
}


template <typename T, typename A>
shared_heap_pimpl<T, A>::operator const_reference() const noexcept
{
    return get();
}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::get() noexcept -> reference
{
    return *t_;
}


template <typename T, typename A>
auto shared_heap_pimpl<T, A>::get() const noexcept -> const_reference
{
    return *t_;
}


template <typename T, typename A>
void shared_heap_pimpl<T, A>::swap(self_t& rhs) noexcept
{
    swap(t_, rhs.t_);
}

PYCPP_END_NAMESPACE
