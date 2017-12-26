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
 *
 *  By default, `stack_allocator` and `stack_allocator_arena` are not
 *  thread-safe, for performance. Using the locked variant of
 *  `stack_allocator`, by setting `UseLocks`, ensures thread safety
 *  through a shared mutex.
 */

#pragma once

#include <pycpp/misc/compressed_pair.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/mutex.h>
#include <pycpp/stl/tuple.h>
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
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = false
>
class stack_allocator_arena;

template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = false
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
    typename Fallback,
    bool UseLocks
>
class stack_allocator_arena
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <size_t S1 = StackSize, size_t A1 = Alignment, bool UF1 = UseFallback, typename F1 = Fallback, bool UL1 = UseLocks>
    struct rebind { using other = stack_allocator_arena<S1, A1, UF1, F1, UL1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_fallback = UseFallback;
    static constexpr bool use_locks = UseLocks;

    // MEMBER TYPES
    // ------------
    using fallback_type = Fallback;
    using mutex_type = conditional_t<UseLocks, mutex, dummy_mutex>;

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator_arena(const fallback_type& fallback = fallback_type()) noexcept;
    stack_allocator_arena(const stack_allocator_arena&) = delete;
    stack_allocator_arena& operator=(const stack_allocator_arena&) = delete;
    stack_allocator_arena(stack_allocator_arena&&) = delete;
    stack_allocator_arena& operator=(stack_allocator_arena&&) = delete;
    ~stack_allocator_arena() noexcept;

    // ALLOCATION
    template <size_t RequiredAlignment> byte* allocate(size_t n);
    void deallocate(byte* p, size_t n) noexcept;

    // PROPERTIES
    static size_t size() noexcept;
    size_t used() const noexcept;
    void reset() noexcept;

private:
    // Although `tuple` uses EBO for all STL implementations,
    // it does not have `piecewise_construct`, so it cannot
    // hold a mutex value. Explicilty use `compressed_pair`.
    alignas(Alignment) byte buf_[StackSize];
    compressed_pair<compressed_pair<byte*, fallback_type>, mutex_type> data_;

    byte*& ptr_() noexcept;
    byte* const& ptr_() const noexcept;
    fallback_type& fallback_() noexcept;
    const fallback_type& fallback_() const noexcept;
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
    bool UseFallback,
    typename Fallback,
    bool UseLocks
>
class stack_allocator
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename T1, size_t S1 = StackSize, size_t A1 = Alignment, bool UF1 = UseFallback, typename F1 = Fallback, bool UL1 = UseLocks>
    struct rebind { using other = stack_allocator<T1, S1, A1, UF1, F1, UL1>; };

    // STATIC VARIABLES
    // ----------------
    static constexpr size_t alignment = Alignment;
    static constexpr size_t stack_size = StackSize;
    static constexpr bool use_fallback = UseFallback;
    static constexpr bool use_locks = UseLocks;

    // MEMBER TYPES
    // ------------
    using self_t = stack_allocator<T, StackSize, Alignment, UseFallback, Fallback, UseLocks>;
    using value_type = T;
    using fallback_type = Fallback;
    using arena_type = stack_allocator_arena<stack_size, alignment>;
    using mutex_type = typename arena_type::mutex_type;
    using propagate_on_container_move_assignment = true_type;
#if defined(CPP11_PARTIAL_COMPATIBILITY)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
#endif      // CPP11_PARTIAL_COMPATIBILITY

    // MEMBER FUNCTIONS
    // ----------------
    stack_allocator() noexcept;
    stack_allocator(arena_type& arena) noexcept;
    stack_allocator(const self_t&) noexcept;
    template <typename T1> stack_allocator(const stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback, UseLocks>&) noexcept;
    self_t& operator=(const self_t&) noexcept;
    template <typename T1> self_t& operator=(const stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback, UseLocks>&) noexcept;
    stack_allocator(self_t&&) noexcept;
    template <typename T1> stack_allocator(stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback, UseLocks>&&) noexcept;
    self_t& operator=(self_t&&) noexcept;
    template <typename T1> self_t& operator=(stack_allocator<T1, StackSize, Alignment, UseFallback, Fallback, UseLocks>&&) noexcept;
    ~stack_allocator() noexcept;

    // ALLOCATOR TRAITS
    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
#if defined(CPP11_PARTIAL_COMPATIBILITY)
    template <typename ... Ts>
    void construct(T* p, Ts&&... ts) { ::new (static_cast<void*>(p)) T(std::forward<Ts>(ts)...); }
    void destroy(T* p) { p->~T(); }
    size_type max_size() { return std::numeric_limits<size_type>::max(); }
#endif      // CPP11_PARTIAL_COMPATIBILITY

private:
    template <typename T1, size_t S, size_t A, bool UL, typename F, bool UF>
    friend class stack_allocator;

    template <
        typename T1, size_t S1, size_t A1, bool UF1, typename F1, bool UL1,
        typename T2, size_t S2, size_t A2, bool UF2, typename F2, bool UL2
    >
    friend bool operator==(const stack_allocator<T1, S1, A1, UF1, F1, UL1>& lhs,
                           const stack_allocator<T2, S2, A2, UF2, F2, UL2>& rhs) noexcept;

    arena_type* arena_ = nullptr;
};

// ALIAS
// -----

template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = false
>
using stack_resource = resource_adaptor<
    stack_allocator<byte, StackSize, Alignment, UseFallback, Fallback, UseLocks>
>;


template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = false
>
using stack_unlocked_resource = resource_adaptor<
    stack_allocator<byte, StackSize, Alignment, UseFallback, Fallback, UseLocks>
>;


template <
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = true
>
using stack_locked_resource = resource_adaptor<
    stack_allocator<byte, StackSize, Alignment, UseFallback, Fallback, UseLocks>
>;


template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = false
>
using stack_unlocked_allocator = stack_allocator<T, StackSize, Alignment, UseFallback, Fallback, UseLocks>;


template <
    typename T,
    size_t StackSize,
    size_t Alignment = alignof(max_align_t),
    bool UseFallback = true,
    typename Fallback = std::allocator<byte>,
    bool UseLocks = true
>
using stack_locked_allocator = stack_allocator<T, StackSize, Alignment, UseFallback, Fallback, UseLocks>;

// SPECIALIZATION
// --------------

template <size_t S, size_t A, bool UF, typename F, bool UL>
struct is_relocatable<stack_allocator_arena<S, A, UF, F, UL>>: false_type
{};

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
struct is_relocatable<stack_allocator<T, S, A, UF, F, UL>>: true_type
{};

// IMPLEMENTATION
// --------------

// ARENA

template <size_t S, size_t A, bool UF, typename F, bool UL>
const size_t stack_allocator_arena<S, A, UF, F, UL>::alignment;

template <size_t S, size_t A, bool UF, typename F, bool UL>
const size_t stack_allocator_arena<S, A, UF, F, UL>::stack_size;

template <size_t S, size_t A, bool UF, typename F, bool UL>
const bool stack_allocator_arena<S, A, UF, F, UL>::use_fallback;

template <size_t S, size_t A, bool UF, typename F, bool UL>
const bool stack_allocator_arena<S, A, UF, F, UL>::use_locks;

template <size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator_arena<S, A, UF, F, UL>::stack_allocator_arena(const fallback_type& fallback) noexcept:
    data_({compressed_pair<byte*, fallback_type>{buf_, fallback}})
{}


template <size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator_arena<S, A, UF, F, UL>::~stack_allocator_arena() noexcept
{
    ptr_() = nullptr;
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
template <size_t RequiredAlignment>
byte* stack_allocator_arena<S, A, UF, F, UL>::allocate(size_t n)
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

    assert(use_fallback && "Exceeding pre-allocated buffer.");
    return static_cast<byte*>(fallback_().allocate(n));
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
void stack_allocator_arena<S, A, UF, F, UL>::deallocate(byte* p, size_t n) noexcept
{
    assert(pointer_in_buffer(ptr_()) && "Allocator has outlived arena.");

    lock_guard<mutex_type> lock(mutex_());
    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == ptr_()) {
            ptr_() = p;
        }
        return;
    }

    assert(use_fallback && "Exceeding pre-allocated buffer.");
    fallback_().deallocate(p, n);
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
size_t stack_allocator_arena<S, A, UF, F, UL>::size() noexcept
{
    return stack_size;
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
size_t stack_allocator_arena<S, A, UF, F, UL>::used() const noexcept
{
    return static_cast<size_t>(ptr_() - buf_);
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
void stack_allocator_arena<S, A, UF, F, UL>::reset() noexcept
{
    lock_guard<mutex_type> lock(mutex_());
    ptr_() = buf_;
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::ptr_() noexcept -> byte*&
{
    return get<0>(get<0>(data_));
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::ptr_() const noexcept -> byte* const&
{
    return get<0>(get<0>(data_));
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::fallback_() noexcept -> fallback_type&
{
    return get<1>(get<0>(data_));
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::fallback_() const noexcept -> const fallback_type&
{
    return get<1>(get<0>(data_));
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::mutex_() noexcept -> mutex_type&
{
    return get<1>(data_);
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator_arena<S, A, UF, F, UL>::mutex_() const noexcept -> const mutex_type&
{
    return get<1>(data_);
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
size_t stack_allocator_arena<S, A, UF, F, UL>::align_up(size_t n) noexcept
{
    return (n + (alignment-1)) & ~(alignment-1);
}


template <size_t S, size_t A, bool UF, typename F, bool UL>
bool stack_allocator_arena<S, A, UF, F, UL>::pointer_in_buffer(byte* p) noexcept
{
    return (buf_ <= p) && (p <= buf_ + stack_size);
}

// ALLOCATOR

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
const size_t stack_allocator<T, S, A, UF, F, UL>::alignment;

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
const size_t stack_allocator<T, S, A, UF, F, UL>::stack_size;

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
const bool stack_allocator<T, S, A, UF, F, UL>::use_fallback;

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
const bool stack_allocator<T, S, A, UF, F, UL>::use_locks;

template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator() noexcept:
    arena_(nullptr)
{}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator(arena_type& arena) noexcept:
    arena_(&arena)
{}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator(const self_t& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
template <typename T1>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator(const stack_allocator<T1, S, A, UF, F, UL>& rhs) noexcept:
    arena_(rhs.arena_)
{}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator<T, S, A, UF, F, UL>::operator=(const self_t& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
template <typename T1>
auto stack_allocator<T, S, A, UF, F, UL>::operator=(const stack_allocator<T1, S, A, UF, F, UL>& rhs) noexcept -> self_t&
{
    arena_ = rhs.arena_;
    return *this;
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator(self_t&& rhs) noexcept
{
    swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
template <typename T1>
stack_allocator<T, S, A, UF, F, UL>::stack_allocator(stack_allocator<T1, S, A, UF, F, UL>&& rhs) noexcept
{
    swap(arena_, rhs.arena_);
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator<T, S, A, UF, F, UL>::operator=(self_t&& rhs) noexcept -> self_t&
{
    swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
template <typename T1>
auto stack_allocator<T, S, A, UF, F, UL>::operator=(stack_allocator<T1, S, A, UF, F, UL>&& rhs) noexcept -> self_t&
{
    swap(arena_, rhs.arena_);
    return *this;
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
stack_allocator<T, S, A, UF, F, UL>::~stack_allocator() noexcept
{
    arena_ = nullptr;
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
auto stack_allocator<T, S, A, UF, F, UL>::allocate(size_type n, const void* hint) -> pointer
{
    assert(arena_ && "Arena cannot be null.");
    return reinterpret_cast<T*>(arena_->template allocate<alignof(T)>(sizeof(T) * n));
}


template <typename T, size_t S, size_t A, bool UF, typename F, bool UL>
void stack_allocator<T, S, A, UF, F, UL>::deallocate(pointer p, size_type n)
{
    assert(arena_ && "Arena cannot be null.");
    arena_->deallocate(reinterpret_cast<byte*>(p), sizeof(T) * n);
}


template <
    typename T1, size_t S1, size_t A1, bool UF1, typename F1, bool UL1,
    typename T2, size_t S2, size_t A2, bool UF2, typename F2, bool UL2
>
bool operator==(const stack_allocator<T1, S1, A1, UF1, F1, UL1>& lhs,
                const stack_allocator<T2, S2, A2, UF2, F2, UL2>& rhs) noexcept
{
    return lhs.arena_ == rhs.arena_;
}


template <
    typename T1, size_t S1, size_t A1, bool UF1, typename F1, bool UL1,
    typename T2, size_t S2, size_t A2, bool UF2, typename F2, bool UL2
>
bool operator!=(const stack_allocator<T1, S1, A1, UF1, F1, UL1>& lhs,
                const stack_allocator<T2, S2, A2, UF2, F2, UL2>& rhs) noexcept
{
    return !(lhs == rhs);
}

PYCPP_END_NAMESPACE
