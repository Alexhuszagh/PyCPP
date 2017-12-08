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
#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>

PYCPP_BEGIN_NAMESPACE

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
    size_t N,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator_arena
{
public:
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
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1, size_t N1 = N, size_t A1 = Alignment>
    struct rebind { using other = stack_allocator<T1, N1, A1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t size = N;

    // MEMBER TYPES
    // ------------
    using self_t = stack_allocator<T, N, Alignment>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using arena_type = stack_allocator_arena<size, alignment>;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator();
    stack_allocator(arena_type& arena) noexcept;
    stack_allocator(const self_t&);
    self_t& operator=(const self_t&);
    ~stack_allocator() noexcept;

    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);

private:
    bool delete_;
    arena_type* arena_;
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

    size_t aligned_n = align_up(n);
    if (static_cast<size_t>(buf_ + N - ptr_) >= aligned_n) {
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


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::stack_allocator():
    delete_(true),
    arena_(new arena_type)
{}


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::stack_allocator(arena_type& arena) noexcept:
    delete_(false),
    arena_(&arena)
{}


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::stack_allocator(const self_t& rhs):
    delete_(rhs.delete_),
    arena_(delete_ ? new arena_type : rhs.arena_)
{}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::operator=(const self_t& rhs) -> self_t&
{
    delete_ = rhs.delete_;
    arena_ = delete_ ? new arena_type : rhs.arena_;
}


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::~stack_allocator() noexcept
{
    if (delete_) {
        delete arena_;
    }
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t N, size_t Alignment>
void stack_allocator<T, N, Alignment>::deallocate(pointer p, size_type n)
{
    arena_->deallocate(reinterpret_cast<char*>(p), sizeof(T) * n);
}


template <typename T1, size_t N1, size_t A1, typename T2, size_t N2, size_t A2>
bool operator==(const stack_allocator<T1, N1, A1>&, const stack_allocator<T2, N2, A2>&) noexcept
{
    return false;
}


template <typename T1, size_t N1, size_t A1, typename T2, size_t N2, size_t A2>
bool operator!=(const stack_allocator<T1, N1, A1>& lhs, const stack_allocator<T2, N2, A2>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
