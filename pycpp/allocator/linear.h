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
 *  The allocator will throw `std::bad_alloc` when the initial buffer is \
 *  exhausted.
 */

#pragma once

#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/stdexcept.h>
#include <cassert>
#include <cstddef>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t)
>
class linear_allocator_arena;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t)
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
    size_t Alignment
>
class linear_allocator_arena
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <size_t S1 = StackSize, size_t A1 = Alignment>
    struct rebind { using other = linear_allocator_arena<S1, A1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;

    // MEMBER FUNCTIONS
    // ----------------
    linear_allocator_arena() noexcept;
    linear_allocator_arena(const linear_allocator_arena&) = delete;
    linear_allocator_arena& operator=(const linear_allocator_arena&) = delete;
    linear_allocator_arena(linear_allocator_arena&&) = delete;
    linear_allocator_arena& operator=(linear_allocator_arena&&) = delete;
    ~linear_allocator_arena();

    // ALLOCATION
    template <size_t RequiredAlignment> char* allocate(size_t n);
    void deallocate(char* p, size_t n) noexcept;

    // PROPERTIES
    static size_t size() noexcept;
    size_t used() const noexcept;
    void reset() noexcept;

private:
    alignas(Alignment) char buf_[StackSize];
    char* ptr_;

    static size_t align_up(size_t n) noexcept;
    bool pointer_in_buffer(char* p) noexcept;
};

// ALLOCATOR

/**
 *  \brief Allocator optimized for stack-based allocation.
 */
template <
    typename T,
    size_t StackSize,
    size_t Alignment
>
class linear_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1 = T, size_t S1 = StackSize, size_t A1 = Alignment>
    struct rebind { using other = linear_allocator<T1, S1, A1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;

    // MEMBER TYPES
    // ------------
    using self_t = linear_allocator<T, StackSize, Alignment>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using arena_type = linear_allocator_arena<stack_size, alignment>;
    using propagate_on_container_move_assignment = std::true_type;

    // MEMBER FUNCTIONS
    // ----------------
    linear_allocator() noexcept;
    linear_allocator(arena_type& arena) noexcept;
    linear_allocator(const self_t&) noexcept;
    template <typename T1> linear_allocator(const linear_allocator<T1, StackSize, Alignment>&) noexcept;
    self_t& operator=(const self_t&) noexcept;
    template <typename T1> self_t& operator=(const linear_allocator<T1, StackSize, Alignment>&) noexcept;
    linear_allocator(self_t&&) noexcept;
    template <typename T1> linear_allocator(linear_allocator<T1, StackSize, Alignment>&&) noexcept;
    self_t& operator=(self_t&&) noexcept;
    template <typename T1> self_t& operator=(linear_allocator<T1, StackSize, Alignment>&&) noexcept;
    ~linear_allocator() noexcept;

    // ALLOCATOR TRAITS
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);

private:
    template <typename T1, size_t S, size_t A>
    friend class linear_allocator;

    template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
    friend bool operator==(const linear_allocator<T1, S1, A1>& lhs, const linear_allocator<T2, S2, A2>& rhs) noexcept;

    template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
    friend bool operator!=(const linear_allocator<T1, S1, A1>& lhs, const linear_allocator<T2, S2, A2>& rhs) noexcept;

    arena_type* arena_ = nullptr;
};

// ALIAS
// -----

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t)
>
using linear_resource = resource_adaptor<
    linear_allocator<char, StackSize, Alignment>
>;

// IMPLEMENTATION
// --------------

// ARENA

template <size_t S, size_t A>
const size_t linear_allocator_arena<S, A>::alignment;

template <size_t S, size_t A>
const size_t linear_allocator_arena<S, A>::stack_size;

template <size_t S, size_t A>
linear_allocator_arena<S, A>::linear_allocator_arena() noexcept:
    ptr_(buf_)
{}


template <size_t S, size_t A>
linear_allocator_arena<S, A>::~linear_allocator_arena()
{
    ptr_ = nullptr;
}


template <size_t S, size_t A>
template <size_t RequiredAlignment>
char* linear_allocator_arena<S, A>::allocate(size_t n)
{
    static_assert(RequiredAlignment <= alignment, "Alignment is too small for this arena");
    assert(pointer_in_buffer(ptr_) && "Allocator has outlived arena.");

    size_t aligned_n = align_up(n);
    if (static_cast<size_t>(buf_ + stack_size - ptr_) >= aligned_n) {
        char* r = ptr_;
        ptr_ += aligned_n;
        return r;
    }

    static_assert(
        alignment <= alignof(max_align_t),
        "Alignment is larger than alignof(max_align_t), and cannot be guaranteed by new."
    );

    throw std::bad_alloc();
}


template <size_t S, size_t A>
void linear_allocator_arena<S, A>::deallocate(char* p, size_t n) noexcept
{
    assert(pointer_in_buffer(ptr_) && "Allocator has outlived arena.");
}


template <size_t S, size_t A>
size_t linear_allocator_arena<S, A>::size() noexcept
{
    return stack_size;
}


template <size_t S, size_t A>
size_t linear_allocator_arena<S, A>::used() const noexcept
{
    return static_cast<size_t>(ptr_ - buf_);
}


template <size_t S, size_t A>
void linear_allocator_arena<S, A>::reset() noexcept
{
    ptr_ = buf_;
}


template <size_t S, size_t A>
size_t linear_allocator_arena<S, A>::align_up(size_t n) noexcept
{
    return (n + (alignment-1)) & ~(alignment-1);
}


template <size_t S, size_t A>
bool linear_allocator_arena<S, A>::pointer_in_buffer(char* p) noexcept
{
    return (buf_ <= p) && (p <= buf_ + stack_size);
}

// ALLOCATOR

template <typename T, size_t S, size_t A>
const size_t linear_allocator<T, S, A>::alignment;

template <typename T, size_t S, size_t A>
const size_t linear_allocator<T, S, A>::stack_size;

template <typename T, size_t S, size_t A>
linear_allocator<T, S, A>::linear_allocator() noexcept:
    arena_(nullptr)
{}


template <typename T, size_t S, size_t A>
linear_allocator<T, S, A>::linear_allocator(arena_type& arena) noexcept:
    arena_(&arena)
{}


template <typename T, size_t S, size_t A>
linear_allocator<T, S, A>::linear_allocator(const self_t& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A>
template <typename T1>
linear_allocator<T, S, A>::linear_allocator(const linear_allocator<T1, S, A>& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A>
auto linear_allocator<T, S, A>::operator=(const self_t& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A>
template <typename T1>
auto linear_allocator<T, S, A>::operator=(const linear_allocator<T1, S, A>& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A>
linear_allocator<T, S, A>::linear_allocator(self_t&& rhs) noexcept
{
    std::swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A>
template <typename T1>
linear_allocator<T, S, A>::linear_allocator(linear_allocator<T1, S, A>&& rhs) noexcept
{
    std::swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A>
auto linear_allocator<T, S, A>::operator=(self_t&& rhs) noexcept -> self_t&
{
    std::swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A>
template <typename T1>
auto linear_allocator<T, S, A>::operator=(linear_allocator<T1, S, A>&& rhs) noexcept -> self_t&
{
    std::swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A>
linear_allocator<T, S, A>::~linear_allocator() noexcept
{
    arena_ = nullptr;
}


template <typename T, size_t S, size_t A>
auto linear_allocator<T, S, A>::allocate(size_type n, const void* hint) -> pointer
{
    assert(arena_ && "Arena cannot be null.");
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t S, size_t A>
void linear_allocator<T, S, A>::deallocate(pointer p, size_type n)
{
    assert(arena_ && "Arena cannot be null.");
    arena_->deallocate(reinterpret_cast<char*>(p), sizeof(T) * n);
}


template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
bool operator==(const linear_allocator<T1, S1, A1>& lhs, const linear_allocator<T2, S2, A2>& rhs) noexcept
{
    return lhs.arena_ == rhs.arena_;
}


template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
bool operator!=(const linear_allocator<T1, S1, A1>& lhs, const linear_allocator<T2, S2, A2>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
