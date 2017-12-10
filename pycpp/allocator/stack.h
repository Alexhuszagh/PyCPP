//  :copyright: (c) 2015 Howard Hinnant.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Allocator that preallocates memory on the stack.
 *
 *  An allocator that optimizes for short containers by preallocating
 *  memory on the stack, avoiding dynamic allocation until the
 *  initial buffer is exceeded.
 */

#pragma once

#include <pycpp/allocator/polymorphic.h>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    size_t StackSize,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator_arena;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator;

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
class stack_allocator_arena
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <size_t S1 = StackSize, size_t A1 = Alignment>
    struct rebind { using other = stack_allocator_arena<S1, A1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator_arena() noexcept;
    stack_allocator_arena(const stack_allocator_arena&) = delete;
    stack_allocator_arena& operator=(const stack_allocator_arena&) = delete;
    stack_allocator_arena(stack_allocator_arena&&) = delete;
    stack_allocator_arena& operator=(stack_allocator_arena&&) = delete;
    ~stack_allocator_arena();

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

// TODO: change this...
// Get rid of the delete_
// Use only an existing arena.
// Remove NodeSize...

// Basically, the allocator is getting duplicated without the
// reference member and they're CROSSING FUCKING
// STREAMS.

/**
 *  \brief Allocator optimized for stack-based allocation.
 */
template <
    typename T,
    size_t StackSize,
    size_t Alignment
>
class stack_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename U = T, size_t S = StackSize, size_t A = Alignment>
    struct rebind { using other = stack_allocator<U, S, A>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;

    // MEMBER TYPES
    // ------------
    using self_t = stack_allocator<T, StackSize, Alignment>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using arena_type = stack_allocator_arena<stack_size, alignment>;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator();
    stack_allocator(arena_type& arena) noexcept;
    stack_allocator(const self_t&);
    template <typename U> stack_allocator(const stack_allocator<U, StackSize, Alignment>&);
    self_t& operator=(const self_t&);
    template <typename U> self_t& operator=(const stack_allocator<U, StackSize, Alignment>&);
    ~stack_allocator() noexcept;

    // ALLOCATOR TRAITS
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);

private:
    template <typename U, size_t S, size_t A>
    friend class stack_allocator;

    template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
    friend bool operator==(const stack_allocator<T1, S1, A1>& lhs, const stack_allocator<T2, S2, A2>& rhs) noexcept;

    template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
    friend bool operator!=(const stack_allocator<T1, S1, A1>& lhs, const stack_allocator<T2, S2, A2>& rhs) noexcept;

    arena_type* arena_ = nullptr;
};

// ALIAS
// -----

template <
    size_t StackSize,
    size_t Alignment = alignof(std::max_align_t)
>
using stack_resource = resource_adaptor<
    stack_allocator<char, StackSize, Alignment>
>;

// IMPLEMENTATION
// --------------

// ARENA

template <size_t S, size_t A>
const size_t stack_allocator_arena<S, A>::alignment;

template <size_t S, size_t A>
const size_t stack_allocator_arena<S, A>::stack_size;

template <size_t S, size_t A>
stack_allocator_arena<S, A>::stack_allocator_arena() noexcept:
    ptr_(buf_)
{}


template <size_t S, size_t A>
stack_allocator_arena<S, A>::~stack_allocator_arena()
{
    ptr_ = nullptr;
}


template <size_t S, size_t A>
template <size_t RequiredAlignment>
char* stack_allocator_arena<S, A>::allocate(size_t n)
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
        alignment <= alignof(std::max_align_t),
        "Alignment is larger than alignof(std::max_align_t), and cannot be guaranteed by new."
    );
    return static_cast<char*>(operator new(n));
}


template <size_t S, size_t A>
void stack_allocator_arena<S, A>::deallocate(char* p, size_t n) noexcept
{
    assert(pointer_in_buffer(ptr_) && "Allocator has outlived arena.");

    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == ptr_) {
            ptr_ = p;
        }
    } else {
        delete p;
    }
}


template <size_t S, size_t A>
size_t stack_allocator_arena<S, A>::size() noexcept
{
    return stack_size;
}


template <size_t S, size_t A>
size_t stack_allocator_arena<S, A>::used() const noexcept
{
    return static_cast<size_t>(ptr_ - buf_);
}


template <size_t S, size_t A>
void stack_allocator_arena<S, A>::reset() noexcept
{
    ptr_ = buf_;
}


template <size_t S, size_t A>
size_t stack_allocator_arena<S, A>::align_up(size_t n) noexcept
{
    return (n + (alignment-1)) & ~(alignment-1);
}


template <size_t S, size_t A>
bool stack_allocator_arena<S, A>::pointer_in_buffer(char* p) noexcept
{
    return (buf_ <= p) && (p <= buf_ + stack_size);
}

// ALLOCATOR

template <typename T, size_t S, size_t A>
const size_t stack_allocator<T, S, A>::alignment;

template <typename T, size_t S, size_t A>
const size_t stack_allocator<T, S, A>::stack_size;


template <typename T, size_t S, size_t A>
stack_allocator<T, S, A>::stack_allocator():
    arena_(nullptr)
{}


template <typename T, size_t S, size_t A>
stack_allocator<T, S, A>::stack_allocator(arena_type& arena) noexcept:
    arena_(&arena)
{}


template <typename T, size_t S, size_t A>
stack_allocator<T, S, A>::stack_allocator(const self_t& rhs):
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A>
template <typename U>
stack_allocator<T, S, A>::stack_allocator(const stack_allocator<U, S, A>& rhs):
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A>
auto stack_allocator<T, S, A>::operator=(const self_t& rhs) -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A>
template <typename U>
auto stack_allocator<T, S, A>::operator=(const stack_allocator<U, S, A>& rhs) -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A>
stack_allocator<T, S, A>::~stack_allocator() noexcept
{
    arena_ = nullptr;
}


template <typename T, size_t S, size_t A>
auto stack_allocator<T, S, A>::allocate(size_type n, const void* hint) -> pointer
{
    assert(arena_ && "Arena cannot be null.");
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t S, size_t A>
void stack_allocator<T, S, A>::deallocate(pointer p, size_type n)
{
    assert(arena_ && "Arena cannot be null.");
    arena_->deallocate(reinterpret_cast<char*>(p), sizeof(T) * n);
}


template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
bool operator==(const stack_allocator<T1, S1, A1>& lhs, const stack_allocator<T2, S2, A2>& rhs) noexcept
{
    return lhs.arena_ == rhs.arena_;
}


template <typename T1, size_t S1, size_t A1, typename T2, size_t S2, size_t A2>
bool operator!=(const stack_allocator<T1, S1, A1>& lhs, const stack_allocator<T2, S2, A2>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
