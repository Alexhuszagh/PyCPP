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
#include <limits>
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
    stack_allocator_arena(stack_allocator_arena&&) = delete;
    stack_allocator_arena& operator=(stack_allocator_arena&&) = delete;

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
 *  \brief Base for stack memory allocator.
 */
struct stack_allocator_base
{
    size_t max_size(size_t size) const noexcept;
};


/**
 *  \brief Allocator optimized for stack-based allocation.
 */
template <
    typename T,
    size_t N,
    size_t Alignment = alignof(std::max_align_t)
>
class stack_allocator: stack_allocator_base
{
public:
    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t size = N;

    // MEMBER TYPES
    // ------------
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
    stack_allocator(arena_type& arena) noexcept;
    stack_allocator(const stack_allocator&) = default;
    stack_allocator& operator=(const stack_allocator&) = default;
    stack_allocator(stack_allocator&&) = delete;
    stack_allocator& operator=(stack_allocator&&) = delete;
    ~stack_allocator() noexcept;

    pointer address(reference) const noexcept;
    const_pointer address(const_reference) const noexcept;
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
    template <typename U, class... Ts> void construct(U*, Ts&&...);
    template <typename U> void destroy(U*);
    size_type max_size() const noexcept;

    // STATIC
    // ------
    static pointer allocate(const stack_allocator<T, N, Alignment>&, size_type, const void* = nullptr);
    static void deallocate(const stack_allocator<T, N, Alignment>&, pointer p, size_type);
    template <typename U, size_t O, size_t B, class... Ts> static void construct(const stack_allocator<U, O, B>&, U*, Ts&&...);
    template <typename U, size_t O, size_t B> static void destroy(const stack_allocator<U, O, B>&, U*);
    static size_type max_size(const stack_allocator<T, N, Alignment>&) noexcept;

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


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::stack_allocator(arena_type& arena) noexcept:
    arena_(arena)
{}


template <typename T, size_t N, size_t Alignment>
stack_allocator<T, N, Alignment>::~stack_allocator() noexcept
{}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::address(reference t) const noexcept -> pointer
{
    return std::addressof(t);
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::address(const_reference t) const noexcept -> const_pointer
{
    return std::addressof(t);
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<T*>(arena_.template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t N, size_t Alignment>
void stack_allocator<T, N, Alignment>::deallocate(pointer p, size_type n)
{
    arena_.deallocate(reinterpret_cast<char*>(p), sizeof(T) * n);
}


template <typename T, size_t N, size_t Alignment>
template<typename U, class... Ts>
void stack_allocator<T, N, Alignment>::construct(U* p, Ts&&... ts)
{
    new ((void*)p) value_type(std::forward<Ts>(ts)...);
}


template <typename T, size_t N, size_t Alignment>
template <typename U>
void stack_allocator<T, N, Alignment>::destroy(U* p)
{
    p->~U();
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::max_size() const noexcept -> size_type
{
    return stack_allocator_base::max_size(sizeof(value_type));
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::allocate(const stack_allocator<T, N, Alignment>& a, size_type n, const void* p) -> pointer
{
    return a.allocate(n, p);
}


template <typename T, size_t N, size_t Alignment>
void stack_allocator<T, N, Alignment>::deallocate(const stack_allocator<T, N, Alignment>& a, pointer p, size_type n)
{
    a.deallocate(p, n);
}


template <typename T, size_t N, size_t Alignment>
template <typename U, size_t O, size_t B, class... Ts>
void stack_allocator<T, N, Alignment>::construct(const stack_allocator<U, O, B>& a, U* p, Ts&&... ts)
{
    a.construct(p, std::forward<Ts>(ts)...);
}


template <typename T, size_t N, size_t Alignment>
template <typename U, size_t O, size_t B>
void stack_allocator<T, N, Alignment>::destroy(const stack_allocator<U, O, B>& a, U* p)
{
    a.destroy(p);
}


template <typename T, size_t N, size_t Alignment>
auto stack_allocator<T, N, Alignment>::max_size(const stack_allocator<T, N, Alignment>& other) noexcept -> size_type
{
    return other.max_size();
}

PYCPP_END_NAMESPACE
