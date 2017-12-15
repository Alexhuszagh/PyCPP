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
 *
 *  Dynamic allocation can be configured (in debug builds) by toggling
 *  the `UseFallback`, which aborts program execution of the `UseFallback`
 *  is off and the initial buffer exhausted. `Fallback` is a the custom
 *  allocator to control dynamic allocation if the initial buffer is
 *  exhausted.
 */

#pragma once

#include <pycpp/misc/compressed_pair.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <cassert>
#include <cstddef>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<char>
>
class stack_allocator_arena;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<char>
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
    size_t Alignment,
    bool UseFallback,
    typename Fallback
>
class stack_allocator_arena
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <size_t S1 = StackSize, size_t A1 = Alignment, bool U1 = UseFallback, typename F1 = Fallback>
    struct rebind { using other = stack_allocator_arena<S1, A1, U1, F1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_fallback = UseFallback;

    // MEMBER TYPES
    // ------------
    using fallback_type = Fallback;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator_arena(const fallback_type& fallback = fallback_type()) noexcept;
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
    compressed_pair<char*, Fallback> data_;

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
    size_t Alignment,
    bool UseFallback,
    typename Fallback
>
class stack_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1 = T, size_t S1 = StackSize, size_t A1 = Alignment, bool U1 = UseFallback, typename F1 = Fallback>
    struct rebind { using other = stack_allocator<T1, S1, A1, U1, F1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_fallback = UseFallback;

    // MEMBER TYPES
    // ------------
    using self_t = stack_allocator<T, StackSize, Alignment, UseFallback, Fallback>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using fallback_type = Fallback;
    using arena_type = stack_allocator_arena<stack_size, alignment>;
    using propagate_on_container_move_assignment = std::true_type;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator() noexcept;
    stack_allocator(arena_type& arena) noexcept;
    stack_allocator(const self_t&) noexcept;
    template <typename T1> stack_allocator(const stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback>&) noexcept;
    self_t& operator=(const self_t&) noexcept;
    template <typename T1> self_t& operator=(const stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback>&) noexcept;
    stack_allocator(self_t&&) noexcept;
    template <typename T1> stack_allocator(stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback>&&) noexcept;
    self_t& operator=(self_t&&) noexcept;
    template <typename T1> self_t& operator=(stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback>&&) noexcept;
    ~stack_allocator() noexcept;

    // ALLOCATOR TRAITS
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);

private:
    template <typename T1, size_t S, size_t A, bool U, typename F>
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
    size_t Alignment = alignof(max_align_t)
>
using stack_resource = resource_adaptor<
    stack_allocator<char, StackSize, Alignment>
>;

// IMPLEMENTATION
// --------------

// ARENA

template <size_t S, size_t A, bool U, typename F>
const size_t stack_allocator_arena<S, A, U, F>::alignment;

template <size_t S, size_t A, bool U, typename F>
const size_t stack_allocator_arena<S, A, U, F>::stack_size;

template <size_t S, size_t A, bool U, typename F>
const bool stack_allocator_arena<S, A, U, F>::use_fallback;

template <size_t S, size_t A, bool U, typename F>
stack_allocator_arena<S, A, U, F>::stack_allocator_arena(const fallback_type& fallback) noexcept:
    data_(buf_, fallback)
{}


template <size_t S, size_t A, bool U, typename F>
stack_allocator_arena<S, A, U, F>::~stack_allocator_arena()
{
    data_.first() = nullptr;
}


template <size_t S, size_t A, bool U, typename F>
template <size_t RequiredAlignment>
char* stack_allocator_arena<S, A, U, F>::allocate(size_t n)
{
    static_assert(RequiredAlignment <= alignment, "Alignment is too small for this arena");
    assert(pointer_in_buffer(data_.first()) && "Allocator has outlived arena.");

    size_t aligned_n = align_up(n);
    if (static_cast<size_t>(buf_ + stack_size - data_.first()) >= aligned_n) {
        char* r = data_.first();
        data_.first() += aligned_n;
        return r;
    }

    static_assert(
        alignment <= alignof(max_align_t),
        "Alignment is larger than alignof(max_align_t), and cannot be guaranteed by new."
    );

    assert(use_fallback && "Exceeding pre-allocated buffer.");
    return static_cast<char*>(data_.second().allocate(n));
}


template <size_t S, size_t A, bool U, typename F>
void stack_allocator_arena<S, A, U, F>::deallocate(char* p, size_t n) noexcept
{
    assert(pointer_in_buffer(data_.first()) && "Allocator has outlived arena.");

    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == data_.first()) {
            data_.first() = p;
        }
        return;
    }

    assert(use_fallback && "Exceeding pre-allocated buffer.");
    data_.second().deallocate(p, n);
}


template <size_t S, size_t A, bool U, typename F>
size_t stack_allocator_arena<S, A, U, F>::size() noexcept
{
    return stack_size;
}


template <size_t S, size_t A, bool U, typename F>
size_t stack_allocator_arena<S, A, U, F>::used() const noexcept
{
    return static_cast<size_t>(data_.first() - buf_);
}


template <size_t S, size_t A, bool U, typename F>
void stack_allocator_arena<S, A, U, F>::reset() noexcept
{
    data_.first() = buf_;
}


template <size_t S, size_t A, bool U, typename F>
size_t stack_allocator_arena<S, A, U, F>::align_up(size_t n) noexcept
{
    return (n + (alignment-1)) & ~(alignment-1);
}


template <size_t S, size_t A, bool U, typename F>
bool stack_allocator_arena<S, A, U, F>::pointer_in_buffer(char* p) noexcept
{
    return (buf_ <= p) && (p <= buf_ + stack_size);
}

// ALLOCATOR

template <typename T, size_t S, size_t A, bool U, typename F>
const size_t stack_allocator<T, S, A, U, F>::alignment;

template <typename T, size_t S, size_t A, bool U, typename F>
const size_t stack_allocator<T, S, A, U, F>::stack_size;

template <typename T, size_t S, size_t A, bool U, typename F>
const bool stack_allocator<T, S, A, U, F>::use_fallback;

template <typename T, size_t S, size_t A, bool U, typename F>
stack_allocator<T, S, A, U, F>::stack_allocator() noexcept:
    arena_(nullptr)
{}


template <typename T, size_t S, size_t A, bool U, typename F>
stack_allocator<T, S, A, U, F>::stack_allocator(arena_type& arena) noexcept:
    arena_(&arena)
{}


template <typename T, size_t S, size_t A, bool U, typename F>
stack_allocator<T, S, A, U, F>::stack_allocator(const self_t& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool U, typename F>
template <typename T1>
stack_allocator<T, S, A, U, F>::stack_allocator(const stack_allocator<T1, S, A, U, F>& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool U, typename F>
auto stack_allocator<T, S, A, U, F>::operator=(const self_t& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool U, typename F>
template <typename T1>
auto stack_allocator<T, S, A, U, F>::operator=(const stack_allocator<T1, S, A, U, F>& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool U, typename F>
stack_allocator<T, S, A, U, F>::stack_allocator(self_t&& rhs) noexcept
{
    std::swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool U, typename F>
template <typename T1>
stack_allocator<T, S, A, U, F>::stack_allocator(stack_allocator<T1, S, A, U, F>&& rhs) noexcept
{
    std::swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool U, typename F>
auto stack_allocator<T, S, A, U, F>::operator=(self_t&& rhs) noexcept -> self_t&
{
    std::swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool U, typename F>
template <typename T1>
auto stack_allocator<T, S, A, U, F>::operator=(stack_allocator<T1, S, A, U, F>&& rhs) noexcept -> self_t&
{
    std::swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool U, typename F>
stack_allocator<T, S, A, U, F>::~stack_allocator() noexcept
{
    arena_ = nullptr;
}


template <typename T, size_t S, size_t A, bool U, typename F>
auto stack_allocator<T, S, A, U, F>::allocate(size_type n, const void* hint) -> pointer
{
    assert(arena_ && "Arena cannot be null.");
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t S, size_t A, bool U, typename F>
void stack_allocator<T, S, A, U, F>::deallocate(pointer p, size_type n)
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
