//  :copyright: (c) 2015 Howard Hinnant.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Allocator that preallocates memory on the stack without deallocating.
 *
 *  An allocator that acheives extremely high performance by preallocating
 *  memory and never deallocating through the use of a fixed-size arena.
 *  Great for short-lived obects, preferrably those that never delete items.
 *  The allocator will throw `bad_alloc` when the initial buffer is
 *  exhausted.
 */

#pragma once

#include <pycpp/misc/compressed_pair.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/mutex.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/type_traits.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseLocks = false
>
class linear_allocator_arena;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseLocks = false
>
class linear_allocator;

// DECLARATIONS
// ------------

/**
 *  \brief Arena to allocate memory from the stack.
 *
 *  Move constructores are disabled since arrays are not pointers,
 *  and therefore would require an O(n) swap. Likewise, copy constructors
 *  are disabled, since it would require copying the entire internal state,
 *  something which is a relatively rare use-case.
 */
template <
    size_t StackSize,
    size_t Alignment,
    bool UseLocks
>
class linear_allocator_arena
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <size_t S1 = StackSize, size_t A1 = Alignment, bool UL1 = UseLocks>
    struct rebind { using other = linear_allocator_arena<S1, A1, UL1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_locks = UseLocks;

    // MEMBER TYPES
    // ------------
    using mutex_type = conditional_t<UseLocks, mutex, dummy_mutex>;

    // MEMBER FUNCTIONS
    // ----------------
    linear_allocator_arena() noexcept;
    linear_allocator_arena(const linear_allocator_arena&) = delete;
    linear_allocator_arena& operator=(const linear_allocator_arena&) = delete;
    linear_allocator_arena(linear_allocator_arena&&) = delete;
    linear_allocator_arena& operator=(linear_allocator_arena&&) = delete;
    ~linear_allocator_arena() noexcept;

    // ALLOCATION
    template <size_t RequiredAlignment> byte* allocate(size_t n);
    void deallocate(byte* p, size_t n) noexcept;

    // PROPERTIES
    static size_t size() noexcept;
    size_t used() const noexcept;
    void reset() noexcept;

private:
    alignas(Alignment) byte buf_[StackSize];
    compressed_pair<byte*, mutex_type> data_;

    byte*& ptr_() noexcept;
    byte* const& ptr_() const noexcept;
    mutex_type& mutex_() noexcept;
    const mutex_type& mutex_() const noexcept;

    static size_t align_up(size_t n) noexcept;
    bool pointer_in_buffer(byte* p) noexcept;
};

// ALLOCATOR

/**
 *  \brief Allocator optimized for stack-based allocation.
 */
template <
    typename T,
    size_t StackSize,
    size_t Alignment,
    bool UseLocks
>
class linear_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1, size_t S1 = StackSize, size_t A1 = Alignment, bool UL1 = UseLocks>
    struct rebind { using other = linear_allocator<T1, S1, A1, UL1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_locks = UseLocks;

    // MEMBER TYPES
    // ------------
    using self_t = linear_allocator<T, StackSize, Alignment, UseLocks>;
    using value_type = T;
    using arena_type = linear_allocator_arena<stack_size, alignment>;
    using mutex_type = typename arena_type::mutex_type;
    using propagate_on_container_move_assignment = true_type;
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // MEMBER FUNCTIONS
    // ----------------
    linear_allocator() noexcept;
    linear_allocator(arena_type& arena) noexcept;
    linear_allocator(const self_t&) noexcept;
    template <typename T1> linear_allocator(const linear_allocator<T1, StackSize, Alignment, UseLocks>&) noexcept;
    self_t& operator=(const self_t&) noexcept;
    template <typename T1> self_t& operator=(const linear_allocator<T1, StackSize, Alignment, UseLocks>&) noexcept;
    linear_allocator(self_t&&) noexcept;
    template <typename T1> linear_allocator(linear_allocator<T1, StackSize, Alignment, UseLocks>&&) noexcept;
    self_t& operator=(self_t&&) noexcept;
    template <typename T1> self_t& operator=(linear_allocator<T1, StackSize, Alignment, UseLocks>&&) noexcept;
    ~linear_allocator() noexcept;

    // ALLOCATOR TRAITS
    value_type* allocate(size_t, const void* = nullptr);
    void deallocate(value_type*, size_t);
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    template <typename ... Ts>
    void construct(T* p, Ts&&... ts) { ::new (static_cast<void*>(p)) T(std::forward<Ts>(ts)...); }
    void destroy(T* p) { p->~T(); }
    size_type max_size() { return std::numeric_limits<size_type>::max(); }
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

private:
    template <typename T1, size_t S, size_t A, bool UL>
    friend class linear_allocator;

    template <typename T1, size_t S1, size_t A1, bool UL1, typename T2, size_t S2, size_t A2, bool UL2>
    friend bool operator==(const linear_allocator<T1, S1, A1, UL1>& lhs, const linear_allocator<T2, S2, A2, UL2>& rhs) noexcept;

    template <typename T1, size_t S1, size_t A1, bool UL1, typename T2, size_t S2, size_t A2, bool UL2>
    friend bool operator!=(const linear_allocator<T1, S1, A1, UL1>& lhs, const linear_allocator<T2, S2, A2, UL2>& rhs) noexcept;

    arena_type* arena_ = nullptr;
};

// ALIAS
// -----

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseLocks = false
>
using linear_resource = resource_adaptor<
    linear_allocator<byte, StackSize, Alignment, UseLocks>
>;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t)
>
using linear_locked_allocator = linear_allocator<T, StackSize, Alignment, true>;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t)
>
using linear_unlocked_allocator = linear_allocator<T, StackSize, Alignment, false>;

// SPECIALIZATION
// --------------

template <size_t S, size_t A, bool UL>
struct is_relocatable<linear_allocator_arena<S, A, UL>>: false_type
{};

template <typename T, size_t S, size_t A, bool UL>
struct is_relocatable<linear_allocator<T, S, A, UL>>: true_type
{};

// IMPLEMENTATION
// --------------

// ARENA

template <size_t S, size_t A, bool UL>
const size_t linear_allocator_arena<S, A, UL>::alignment;

template <size_t S, size_t A, bool UL>
const size_t linear_allocator_arena<S, A, UL>::stack_size;

template <size_t S, size_t A, bool UL>
const bool linear_allocator_arena<S, A, UL>::use_locks;

template <size_t S, size_t A, bool UL>
inline linear_allocator_arena<S, A, UL>::linear_allocator_arena() noexcept:
    data_(buf_)
{}


template <size_t S, size_t A, bool UL>
inline linear_allocator_arena<S, A, UL>::~linear_allocator_arena() noexcept
{
    ptr_() = nullptr;
}


template <size_t S, size_t A, bool UL>
template <size_t RequiredAlignment>
byte* linear_allocator_arena<S, A, UL>::allocate(size_t n)
{
    static_assert(RequiredAlignment <= alignment, "Alignment is too small for this arena");
    assert(pointer_in_buffer(ptr_()) && "Allocator has outlived arena.");

    lock_guard<mutex_type> lock(mutex_());
    size_t aligned_n = align_up(n);
    if (static_cast<size_t>(buf_ + stack_size - ptr_()) >= aligned_n) {
        byte* r = ptr_();
        ptr_() += aligned_n;
        return r;
    }

    static_assert(
        alignment <= alignof(max_align_t),
        "Alignment is larger than alignof(max_align_t), and cannot be guaranteed by new."
    );

    throw bad_alloc();
}


template <size_t S, size_t A, bool UL>
inline void linear_allocator_arena<S, A, UL>::deallocate(byte* p, size_t n) noexcept
{
    assert(pointer_in_buffer(ptr_()) && "Allocator has outlived arena.");
}


template <size_t S, size_t A, bool UL>
inline size_t linear_allocator_arena<S, A, UL>::size() noexcept
{
    return stack_size;
}


template <size_t S, size_t A, bool UL>
inline size_t linear_allocator_arena<S, A, UL>::used() const noexcept
{
    return static_cast<size_t>(ptr_() - buf_);
}


template <size_t S, size_t A, bool UL>
inline void linear_allocator_arena<S, A, UL>::reset() noexcept
{
    lock_guard<mutex_type> lock(mutex_());
    ptr_() = buf_;
}


template <size_t S, size_t A, bool UL>
inline auto linear_allocator_arena<S, A, UL>::ptr_() noexcept -> byte*&
{
    return get<0>(data_);
}


template <size_t S, size_t A, bool UL>
inline auto linear_allocator_arena<S, A, UL>::ptr_() const noexcept -> byte* const&
{
    return get<0>(data_);
}


template <size_t S, size_t A, bool UL>
inline auto linear_allocator_arena<S, A, UL>::mutex_() noexcept -> mutex_type&
{
    return get<1>(data_);
}


template <size_t S, size_t A, bool UL>
inline auto linear_allocator_arena<S, A, UL>::mutex_() const noexcept -> const mutex_type&
{
    return get<1>(data_);
}


template <size_t S, size_t A, bool UL>
inline size_t linear_allocator_arena<S, A, UL>::align_up(size_t n) noexcept
{
    return (n + (alignment-1)) & ~(alignment-1);
}


template <size_t S, size_t A, bool UL>
inline bool linear_allocator_arena<S, A, UL>::pointer_in_buffer(byte* p) noexcept
{
    return (buf_ <= p) && (p <= buf_ + stack_size);
}

// ALLOCATOR

template <typename T, size_t S, size_t A, bool UL>
const size_t linear_allocator<T, S, A, UL>::alignment;

template <typename T, size_t S, size_t A, bool UL>
const size_t linear_allocator<T, S, A, UL>::stack_size;

template <typename T, size_t S, size_t A, bool UL>
const bool linear_allocator<T, S, A, UL>::use_locks;

template <typename T, size_t S, size_t A, bool UL>
inline linear_allocator<T, S, A, UL>::linear_allocator() noexcept:
    arena_(nullptr)
{}


template <typename T, size_t S, size_t A, bool UL>
inline linear_allocator<T, S, A, UL>::linear_allocator(arena_type& arena) noexcept:
    arena_(&arena)
{}


template <typename T, size_t S, size_t A, bool UL>
inline linear_allocator<T, S, A, UL>::linear_allocator(const self_t& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool UL>
template <typename T1>
inline linear_allocator<T, S, A, UL>::linear_allocator(const linear_allocator<T1, S, A, UL>& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool UL>
inline auto linear_allocator<T, S, A, UL>::operator=(const self_t& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool UL>
template <typename T1>
inline auto linear_allocator<T, S, A, UL>::operator=(const linear_allocator<T1, S, A, UL>& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool UL>
inline linear_allocator<T, S, A, UL>::linear_allocator(self_t&& rhs) noexcept
{
    swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool UL>
template <typename T1>
inline linear_allocator<T, S, A, UL>::linear_allocator(linear_allocator<T1, S, A, UL>&& rhs) noexcept
{
    swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool UL>
inline auto linear_allocator<T, S, A, UL>::operator=(self_t&& rhs) noexcept -> self_t&
{
    swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool UL>
template <typename T1>
inline auto linear_allocator<T, S, A, UL>::operator=(linear_allocator<T1, S, A, UL>&& rhs) noexcept -> self_t&
{
    swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool UL>
inline linear_allocator<T, S, A, UL>::~linear_allocator() noexcept
{
    arena_ = nullptr;
}


template <typename T, size_t S, size_t A, bool UL>
inline auto linear_allocator<T, S, A, UL>::allocate(size_t n, const void* hint) -> value_type*
{
    assert(arena_ && "Arena cannot be null.");
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t S, size_t A, bool UL>
inline void linear_allocator<T, S, A, UL>::deallocate(value_type* p, size_t n)
{
    assert(arena_ && "Arena cannot be null.");
    arena_->deallocate(reinterpret_cast<byte*>(p), sizeof(T) * n);
}


template <typename T1, size_t S1, size_t A1, bool UL1, typename T2, size_t S2, size_t A2, bool UL2>
inline bool operator==(const linear_allocator<T1, S1, A1, UL1>& lhs, const linear_allocator<T2, S2, A2, UL2>& rhs) noexcept
{
    return lhs.arena_ == rhs.arena_;
}


template <typename T1, size_t S1, size_t A1, bool UL1, typename T2, size_t S2, size_t A2, bool UL2>
inline bool operator!=(const linear_allocator<T1, S1, A1, UL1>& lhs, const linear_allocator<T2, S2, A2, UL2>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
