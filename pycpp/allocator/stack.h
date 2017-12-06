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

#include <pycpp/config.h>
#include <cstddef>
#include <cassert>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Arena to allocate memory from the stack.
 */
template <
    size_t N,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator_arena
{
public:
    stack_allocator_arena() noexcept;
    ~stack_allocator_arena();
    stack_allocator_arena(const stack_allocator_arena&) = delete;
    stack_allocator_arena& operator=(const stack_allocator_arena&) = delete;

    // ALLOCATION
    template <size_t RequiredAlignment> char* allocate(size_t n);
    void deallocate(char* p, size_t n) noexcept;

    // PROPERTIES
    static size_t size() noexcept;
    size_t used() const noexcept;
    void reset() noexcept;

private:
    alignas(Alignment) char buf_[N];
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
    size_t N,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator
{
public:
    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t size = N;

    // MEMBER TYPES
    // ------------
    using value_type = T;
    using arena_type = stack_allocator_arena<size, alignment>;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: need a default constructor...
    stack_allocator(const stack_allocator&) = default;
    stack_allocator& operator=(const stack_allocator&) = delete;

private:
    arena_type& arena_;
};

// IMPLEMENTATION
// --------------

// ARENA

template <size_t N, size_t Alignment>
stack_allocator_arena<N, Alignment>::stack_allocator_arena() noexcept:
    ptr_(buf_)
{}


template <size_t N, size_t Alignment>
stack_allocator_arena<N, Alignment>::~stack_allocator_arena()
{
    ptr_ = nullptr;
}


template <size_t N, size_t Alignment>
template <size_t RequiredAlignment>
char* stack_allocator_arena<N, Alignment>::allocate(size_t n)
{
    static_assert(RequiredAlignment <= Alignment, "Alignment is too small for this arena");
    assert(pointer_in_buffer(ptr_) && "Allocator has outlived arena.");

    auto const aligned_n = align_up(n);
    if (static_cast<decltype(aligned_n)>(buf_ + N - ptr_) >= aligned_n) {
        char* r = ptr_;
        ptr_ += aligned_n;
        return r;
    }

    static_assert(
        Alignment <= alignof(std::max_align_t),
        "Alignment is larger than alignof(std::max_align_t), and cannot be guaranteed by new."
    );
    return static_cast<char*>(operator new(n));
}

template <size_t N, size_t Alignment>
void stack_allocator_arena<N, Alignment>::deallocate(char* p, size_t n) noexcept
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


template <size_t N, size_t Alignment>
size_t stack_allocator_arena<N, Alignment>::size() noexcept
{
    return N;
}

template <size_t N, size_t Alignment>
size_t stack_allocator_arena<N, Alignment>::used() const noexcept
{
    return static_cast<size_t>(ptr_ - buf_);
}

template <size_t N, size_t Alignment>
void stack_allocator_arena<N, Alignment>::reset() noexcept
{
    ptr_ = buf_;
}


template <size_t N, size_t Alignment>
size_t stack_allocator_arena<N, Alignment>::align_up(size_t n) noexcept
{
    return (n + (Alignment-1)) & ~(Alignment-1);
}


template <size_t N, size_t Alignment>
bool stack_allocator_arena<N, Alignment>::pointer_in_buffer(char* p) noexcept
{
    return buf_ <= p && p <= buf_ + N;
}

// ALLOCATOR


PYCPP_END_NAMESPACE
