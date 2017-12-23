//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <memory>.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/new.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/detail/polymorphic_allocator.h>
#include <memory>
#include <stdlib.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::make_shared;
using std::allocate_shared;
using std::addressof;
using std::default_delete;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

template <typename T, typename Allocator>
struct uses_allocator: std::uses_allocator<T, Allocator>
{};

// Check if the allocator has `reallocate`, an extension.
template <typename T>
class has_reallocate
{
protected:
    template <typename C> static char &test(decltype(std::declval<C>().reallocate(std::declval<typename C::value_type*>(), std::declval<size_t>(), std::declval<size_t>())));
    template <typename C> static long &test(...);

public:
    enum {
        value = sizeof(test<T>(0)) == sizeof(char)
    };
};

template <typename Allocator>
struct allocator_traits: std::allocator_traits<Allocator>
{
    using typename std::allocator_traits<Allocator>::value_type;
    using typename std::allocator_traits<Allocator>::pointer;
    using typename std::allocator_traits<Allocator>::size_type;

    // Overload if class provides specialized reallocate
    template <typename T = value_type, typename A = Allocator>
    enable_if_t<has_reallocate<A>::value, pointer>
    reallocate(Allocator& allocator, pointer ptr, size_type old_size, size_type new_size)
    {
        return allocator.reallocate(ptr, old_size, new_size);
    }

    // Overload if class does not provide specialized reallocate
    // and can be trivially moved as bytes.
    template <typename T = value_type, typename A = Allocator>
    enable_if_t<!has_reallocate<A>::value && is_relocatable<T>::value, pointer>
    reallocate(Allocator& allocator, pointer ptr, size_type old_size, size_type new_size)
    {
        pointer p = allocator.allocate(new_size);
        memcpy((void*) p, (void*) ptr, old_size * sizeof(value_type));
        allocator.deallocate(ptr, old_size);
        return p;
    }

    // Overload if class does not provide specialized reallocate
    // and cannot be trivially moved as bytes.
    template <typename T = value_type, typename A = Allocator>
    enable_if_t<!has_reallocate<A>::value && !is_relocatable<T>::value, pointer>
    reallocate(Allocator& allocator, pointer ptr, size_type old_size, size_type new_size)
    {
        pointer p = allocator.allocate(new_size);
        // use placement new to construct-in-place
        // Don't use `std::move`, since that move assigns into
        // uninitialized memory.
        for (size_t i = 0; i < old_size; ++i) {
            T& src = ptr[i];
            T* dst = &p[i];
            new (static_cast<void*>(dst)) T(std::move(src));
        }
        allocator.deallocate(ptr, old_size);
        return p;
    }
};

#if USE_POLYMORPHIC_ALLOCATOR           // POLYMOPRHIC

template <typename T>
using allocator = polymorphic_allocator<T>;

#else                                   // !POLYMOPRHIC

template <typename T>
using allocator = std::allocator<T>;

#endif                                  // POLYMOPRHIC

// The following are from cstddef, so include them here
using std::nullptr_t;

#if defined(HAVE_CPP17)     // HAVE_CPP17

using std::byte;

#else                       // !HAVE_CPP17

enum class byte: unsigned char
{};

#endif                      // HAVE_CPP17

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if USE_XXHASH

template <typename T, typename Deleter>
struct hash<std::unique_ptr<T, Deleter>>
{
    using argument_type = std::unique_ptr<T, Deleter>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

template <typename T>
struct hash<std::shared_ptr<T>>
{
    using argument_type = std::shared_ptr<T>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

#endif          // USE_XXHASH

// FUNCTIONS
// ---------

/**
 *  \brief `new` analog for a custom allocator.
 */
template <typename T, typename Allocator, typename ... Ts>
inline T* allocate_and_construct(const Allocator& allocator, Ts&&... ts)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    // the allocator should throw if there's no memory available
    allocator_type alloc(allocator);
    T* t = traits_type::allocate(alloc, sizeof(T));
    traits_type::construct(alloc, t, std::forward<Ts>(ts)...);
    return t;
}


/**
 *  \brief `delete` analog for a custom allocator.
 */
template <typename T, typename Allocator>
inline void destroy_and_deallocate(const Allocator& allocator, T* t, size_t n = 1)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    allocator_type alloc(allocator);
    traits_type::destroy(alloc, t);
    traits_type::deallocate(alloc, t, n);
}

#if defined(HAVE_CPP14)     // CPP14

using std::make_unique;

#else                       // CPP11

template <typename T, typename ... Ts >
unique_ptr<T> make_unique(Ts&&... ts)
{
    return unique_ptr<T>(new T(forward<Ts>(ts)...));
}

template <typename T>
unique_ptr<T> make_unique(size_t size)
{
    using type = remove_extent_t<T>;
    return unique_ptr<T>(new type[size]);
}

#endif                      // HAVE_CPP14

PYCPP_END_NAMESPACE
