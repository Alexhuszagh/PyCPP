//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Polymorphic allocator as described in N3525.
 *
 *  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3525.pdf
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#if defined(HAVE_CPP17)
#   include <memory_resource>
#else
#   include <atomic>
#   include <memory>
#   include <new>
#   include <stdexcept>
#   include <type_traits>
#endif

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// Right now we depend on some non-standard extensions to
// polymorphic allocator, specifically,
// `propagate_on_container_move_assignment`.
// The conditions to reproduce this are, using
// GCC on Linux:
//      using pair = std::pair<const int, int>;
//      using list = std::list<pair, polymorphic_allocator<pair>>;
//      list x_, y_;
//      y_ = std::move(x_);
//
// This calls `list::operator=(list&&); which then
// checks if `propagate_on_container_move_assignment`
// or `is_always_equal` for the container allocator,
// if not, it uses move assignment to move nodes.
//
// Since the allocator is not `is_always_equal`, since
// it wraps an internal resource, simply manage the
// allocator move assignment so it is **well formed**.
//
// Forcing a custom implementation of polymorphic allocator
// is controlled by `PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR`, and
// allowing nonstandard behavior in polymorphic allocator is
// controlled by `PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR`.
// Revert this whenever possible.
#define PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR 1        // ! defined(HAVE_CPP17)
#define PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR 1  // 0

#if !defined(PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR)    // HAVE_CPP17

// ALIAS
// -----

using memory_resource = std::pmr::memory_resource;
template <typename T> using polymorphic_allocator = std::pmr::polymorphic_allocator;
template <typename T> using resource_adaptor_imp = std::pmr::resource_adaptor_imp<T>;
template <typename T> using resource_adaptor = std::pmr::resource_adaptor<T>;
using pool_options = std::pmr::pool_options;
using synchronized_pool_resource = std::pmr::synchronized_pool_resource;
using unsynchronized_pool_resource = std::pmr::unsynchronized_pool_resource;
using monotonic_buffer_resource = std::pmr::monotonic_buffer_resource;

// FUNCTIONS
// ---------

/**
 *  \brief Return a pointer to a global resource that uses `new` and `delete`.
 */
memory_resource* new_delete_resource() noexcept;

/**
 *  \brief Return a pointer to a global resource that does no allocation.
 */
memory_resource* null_memory_resource() noexcept;

/**
 *  \brief Get the current default resource.
 */
memory_resource* get_default_resource() noexcept;

/**
 *  \brief Set the default resource.
 */
memory_resource* set_default_resource(memory_resource* r) noexcept;

#else                               // !HAVE_CPP17

namespace polymorphic_detail
{
// DETAIL
// ------

/**
 *  \brief Round up to nearest multiple of `alignment`.
 */
size_t aligned_allocation_size(size_t n, size_t alignment) noexcept;

/**
 *  \brief Get memory max size.
 */
size_t max_size(size_t align);

}   /* polymorphic_detail */

// FORWARD
// -------

struct memory_resource;
template <typename T> struct polymorphic_allocator;
template <typename Allocator> struct resource_adaptor_imp;

// ALIAS
// -----

template <typename Allocator>
using resource_adaptor = resource_adaptor_imp<
    typename std::allocator_traits<Allocator>::template rebind_alloc<char>
>;

// FUNCTIONS
// ---------

/**
 *  \brief Return a pointer to a global resource that uses `new` and `delete`.
 */
memory_resource* new_delete_resource() noexcept;

/**
 *  \brief Return a pointer to a global resource that does no allocation.
 */
memory_resource* null_memory_resource() noexcept;

/**
 *  \brief Get the current default resource.
 */
memory_resource* get_default_resource() noexcept;

/**
 *  \brief Set the default resource.
 */
memory_resource* set_default_resource(memory_resource* r) noexcept;

// OBJECTS
// -------

// MEMORY RESOURCE

/**
 *  \brief Abstract base class for polymorphic resources.
 */
struct memory_resource
{
public:
    virtual ~memory_resource();

    // ALLOCATOR TRAITS
    void* allocate(size_t n, size_t alignment = alignof(max_align_t));
    void deallocate(void* p, size_t n, size_t alignment = alignof(max_align_t));
    bool is_equal(const memory_resource&) const noexcept;

protected:
    // MEMORY TRAITS
    virtual void* do_allocate(size_t n, size_t alignment) = 0;
    virtual void do_deallocate(void* p, size_t n, size_t alignment) = 0;
    virtual bool do_is_equal(const memory_resource&) const noexcept = 0;

private:
    static std::atomic<memory_resource*> default_resource_;

    friend memory_resource* set_default_resource(memory_resource*) noexcept;
    friend memory_resource* get_default_resource() noexcept;
};

// RESOURCE ADAPTOR

/**
 *  \brief Adapts an STL allocator to a polymorphic resource.
 */
template <typename Allocator>
struct resource_adaptor_imp: memory_resource
{
    // MEMBER TYPES
    // ------------
    using allocator_type = Allocator;

    // MEMBER FUNCTIONS
    // ----------------
    resource_adaptor_imp() = default;
    resource_adaptor_imp(const resource_adaptor_imp&) = default;
    resource_adaptor_imp(resource_adaptor_imp&&) = default;
    resource_adaptor_imp& operator=(const resource_adaptor_imp&) = default;
    explicit resource_adaptor_imp(const allocator_type&);
    explicit resource_adaptor_imp(allocator_type&&);

    // ALLOCATOR TRAITS
    allocator_type get_allocator() const;

protected:
    // MEMORY TRAITS
    virtual void* do_allocate(size_t n, size_t alignment);
    virtual void do_deallocate(void* p, size_t n, size_t alignment);
    virtual bool do_is_equal(const memory_resource&) const noexcept;

private:
    static constexpr size_t max_align = alignof(max_align_t);
    using traits_type = std::allocator_traits<Allocator>;
    using storage = typename std::aligned_storage<max_align, max_align>::type;
    using alloc = typename traits_type::template rebind_alloc<storage>;
    static_assert(std::is_same<typename traits_type::value_type, char>::value, "");

    alloc alloc_;
};

// POLYMORPHIC ALLOCATOR

/**
 *  \brief STL allocator wrapping the polymorphic resource.
 */
template <typename T>
struct polymorphic_allocator
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = T;

    // MEMBER FUNCTIONS
    // ----------------
    polymorphic_allocator() noexcept;
    polymorphic_allocator(memory_resource *r);
    polymorphic_allocator(const polymorphic_allocator<T>&) = default;
    template <typename U> polymorphic_allocator(const polymorphic_allocator<U>&) noexcept;
    polymorphic_allocator<T>& operator=(const polymorphic_allocator<T>&) = delete;

    // ALLOCATOR TRAITS
    T* allocate(size_t n);
    void deallocate(T* p, size_t n);

    // PROPERTIES
    polymorphic_allocator select_on_container_copy_construction() const;
    memory_resource* resource() const;

#if PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR
    using propagate_on_container_move_assignment = std::true_type;

    polymorphic_allocator(polymorphic_allocator<T>&&) noexcept;
    template <typename U> polymorphic_allocator(polymorphic_allocator<U>&&) noexcept;
    polymorphic_allocator<T>& operator=(polymorphic_allocator<T>&&) noexcept;

    template <typename U>
    friend class polymorphic_allocator;
#endif      // PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR

private:
    memory_resource* resource_;
};

// TODO: implement pool_options
// TODO: implement synchronized_pool_resource
// TODO: implement unsynchronized_pool_resource
// TODO: implement monotonic_buffer_resource

// IMPLEMENTATION
// --------------

// MEMORY RESOURCE

bool operator==(const memory_resource& lhs, const memory_resource& rhs);
bool operator!=(const memory_resource& lhs, const memory_resource& rhs);

// RESOURCE ADAPTOR


template <typename Allocator>
resource_adaptor_imp<Allocator>::resource_adaptor_imp(const allocator_type& alloc):
    alloc_(alloc)
{}


template <typename Allocator>
resource_adaptor_imp<Allocator>::resource_adaptor_imp(allocator_type&& alloc):
    alloc_(std::move(alloc))
{}


template <typename Allocator>
void* resource_adaptor_imp<Allocator>::do_allocate(size_t n, size_t alignment)
{
    if (n > polymorphic_detail::max_size(max_align)) {
            throw std::bad_alloc();
        }

    size_t s = polymorphic_detail::aligned_allocation_size(n, max_align) / max_align;
    return alloc_.allocate(s);
}


template <typename Allocator>
void resource_adaptor_imp<Allocator>::do_deallocate(void* p, size_t n, size_t alignment)
{
    using value_type = typename std::allocator_traits<alloc>::value_type;
    size_t s = polymorphic_detail::aligned_allocation_size(n, max_align) / max_align;
    alloc_.deallocate(reinterpret_cast<value_type*>(p), s);
}


template <typename Allocator>
bool resource_adaptor_imp<Allocator>::do_is_equal(const memory_resource& other) const noexcept
{
    using self = resource_adaptor_imp<Allocator>;
    const self* p = dynamic_cast<const self*>(&other);
    return p ? alloc_ == p->alloc_ : false;
}


template <typename Allocator>
auto resource_adaptor_imp<Allocator>::get_allocator() const -> allocator_type
{
    return alloc_;
}

// POLYMORPHIC ALLOCATOR

template <typename T>
polymorphic_allocator<T>::polymorphic_allocator() noexcept:
    resource_(get_default_resource())
{}


template <typename T>
polymorphic_allocator<T>::polymorphic_allocator(memory_resource* r):
    resource_(r ? r : get_default_resource())
{}


template <typename T>
template <typename U>
polymorphic_allocator<T>::polymorphic_allocator(const polymorphic_allocator<U>& rhs) noexcept:
    resource_(rhs.resource())
{}


template <typename T>
T* polymorphic_allocator<T>::allocate(size_t n)
{
    return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T)));
}


template <typename T>
void polymorphic_allocator<T>::deallocate(T *p, size_t n)
{
    resource_->deallocate(p, n * sizeof(T), alignof(T));
}


template <typename T>
polymorphic_allocator<T> polymorphic_allocator<T>::select_on_container_copy_construction() const
{
    return polymorphic_allocator<T>();
}


template <typename T>
memory_resource* polymorphic_allocator<T>::resource() const
{
    return resource_;
}


#if PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR

template <typename T>
polymorphic_allocator<T>::polymorphic_allocator(polymorphic_allocator<T>&& rhs) noexcept:
    polymorphic_allocator()
{
    // let resource use the default constructor
    std::swap(resource_, rhs.resource_);
}


template <typename T>
template <typename U>
polymorphic_allocator<T>::polymorphic_allocator(polymorphic_allocator<U>&& rhs) noexcept:
    polymorphic_allocator()
{
    // let resource use the default constructor
    std::swap(resource_, rhs.resource_);
}


template <typename T>
polymorphic_allocator<T>& polymorphic_allocator<T>::operator=(polymorphic_allocator<T>&& rhs) noexcept
{
    std::swap(resource_, rhs.resource_);
    return *this;
}

#endif              // PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR


template <typename T1, typename T2>
bool operator==(const polymorphic_allocator<T1>& lhs, const polymorphic_allocator<T2>& rhs)
{
    return *lhs.resource() == *rhs.resource();
}

template <typename T1, typename T2>
bool operator!=(const polymorphic_allocator<T1>& lhs, const polymorphic_allocator<T2>& rhs)
{
    return !(lhs == rhs);
}

#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE
