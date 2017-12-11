//  :copyright: (c) 2011-2012 Andrzej Krzemienski.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Optional implementation for C++11.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional.h>

#if defined(HAVE_CPP17)             // HAVE_CPP17
#   include <optional>
#else                               // !HAVE_CPP17
#   include <pycpp/stl/utility.h>
#   include <utility>
#   include <type_traits>
#   include <initializer_list>
#   include <cassert>
#   include <functional>
#   include <string>
#   include <stdexcept>
#   include <type_traits>
#endif                              // HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_CPP17)             // HAVE_CPP17

// ALIAS
// -----

template <typename T> using optional = std::optional<T>;
using nullopt_t = std::nullopt_t;
using nullopt = std::nullopt;
using bad_optional_access = std::bad_optional_access;

// FUNCTIONS
// ---------

template <typename T, typename... Ts>
auto make_optional(Ts&&... ts) -> decltype(std::make_optional<T>(std::forward<Ts>(ts)...))
{
  return std::make_optional<T>(std::forward<Ts>(ts)...);
}

#else                               // !HAVE_CPP17

// MACROS
// ------

#define TR2_OPTIONAL_REQUIRES(...) typename std::enable_if<__VA_ARGS__::value, bool>::type = false

#if defined(HAVE_GNUC)
#   if (GNUC_MAJOR_VERSION == 4) && (GNUC_MINOR_VERSION >= 8)
#       define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   elif (GNUC_MAJOR_VERSION > 4)
#       define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   endif
#
#   if (GNUC_MAJOR_VERSION == 4) && (GNUC_MINOR_VERSION >= 7)
#       define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   elif (GNUC_MAJOR_VERSION > 4)
#       define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   endif
#
#   if (GNUC_MAJOR_VERSION == 4) && (GNUC_MINOR_VERSION == 8) && (__GNUC_PATCHLEVEL__ >= 1)
#       define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (GNUC_MAJOR_VERSION == 4) && (GNUC_MINOR_VERSION >= 9)
#       define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ > 4)
#       define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   endif
#endif

#if defined(HAVE_CLANG)
#   if (COMPILER_MAJOR_VERSION == 3 && COMPILER_MINOR_VERSION >= 5)
#       define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   elif (COMPILER_MAJOR_VERSION > 3)
#       define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   endif
#   if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#       define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   elif (COMPILER_MAJOR_VERSION == 3 && COMPILER_MINOR_VERSION == 4 && COMPILER_PATCH_VERSION >= 2)
#       define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   endif
#endif
#
#if defined(HAVE_MSVC)
#   if (COMPILER_MAJOR_VERSION >= 14)
#       define TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   endif
#endif

#if defined(HAVE_CLANG)
#   if (COMPILER_MAJOR_VERSION > 2) || (COMPILER_MAJOR_VERSION == 2) && (COMPILER_MINOR_VERSION >= 9)
#       define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#   else
#       define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#   endif
#elif defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#else
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#endif

# if defined(TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___)
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 1
#   define OPTIONAL_CONSTEXPR_INIT_LIST constexpr
# else
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 0
#   define OPTIONAL_CONSTEXPR_INIT_LIST
# endif

#if defined(TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_) && defined(HAVE_CPP14)
#   define OPTIONAL_HAS_MOVE_ACCESSORS 1
#else
#   define OPTIONAL_HAS_MOVE_ACCESSORS 0
#endif

// In C++11 constexpr implies const, so we need to make
// non-const members also non-constexpr.
#if defined(HAVE_CPP14)
#   define OPTIONAL_MUTABLE_CONSTEXPR constexpr
#else
#   define OPTIONAL_MUTABLE_CONSTEXPR
#endif

// Assert
#if defined(NDEBUG)
#   define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
#else
#   define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : ([]{assert(!#CHECK);}(), (EXPR)))
#endif

// ALIAS
// -----

template <typename T>
using is_trivially_destructible = std::is_trivially_destructible<T>;

template <typename T>
struct is_nothrow_move_constructible
{
    constexpr static bool value = std::is_nothrow_constructible<T, T&&>::value;
};


template <typename T, typename U>
struct is_assignable
{
    template <typename X, typename Y>
    constexpr static bool has_assign(...) { return false; }

    template <typename X, typename Y, size_t S = sizeof((std::declval<X>() = std::declval<Y>(), true)) >
    constexpr static bool has_assign(bool) { return true; }

    constexpr static bool value = has_assign<T, U>(true);
};


template <typename T>
struct is_nothrow_move_assignable
{
    template <typename X, bool has_any_move_assign>
    struct has_nothrow_move_assign
    {
      constexpr static bool value = false;
    };

    template <typename X>
    struct has_nothrow_move_assign<X, true>
    {
        constexpr static bool value = noexcept( std::declval<X&>() = std::declval<X&&>() );
    };

    constexpr static bool value = has_nothrow_move_assign<T, is_assignable<T&, T&&>::value>::value;
};

// FORWARD
// -------

template <typename T>
class optional;

template <typename T>
class optional<T&>;

// UTILITY
// -------

// Forwarding for constexpr not working.

template <typename T>
inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template <typename T>
inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <typename T>
inline constexpr typename std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}


// DETAIL
// ------


namespace detail
{

// Constexpr addressof();
template <typename T>
struct has_overloaded_addressof
{
    template <typename X>
    constexpr static bool has_overload(...) { return false; }

    template <typename X, size_t S = sizeof(std::declval<X&>().operator&()) >
    constexpr static bool has_overload(bool) { return true; }

    constexpr static bool value = has_overload<T>(true);
};

template <typename T, TR2_OPTIONAL_REQUIRES(!has_overloaded_addressof<T>)>
constexpr T* static_addressof(T& ref)
{
    return &ref;
}

template <typename T, TR2_OPTIONAL_REQUIRES(has_overloaded_addressof<T>)>
T* static_addressof(T& ref)
{
    return std::addressof(ref);
}

// the call to convert<A>(b) has return type A and converts b to type A iff
// b decltype(b) is implicitly convertible to A
template <typename U>
constexpr U convert(U v) { return v; }

}   /* detail */


// OPTIONAL
// --------


constexpr struct trivial_init_t{} trivial_init{};


// 20.5.7, Disengaged state indicator
struct nullopt_t
{
    struct init{};
    constexpr explicit nullopt_t(init){}
};
constexpr nullopt_t nullopt{nullopt_t::init()};


// 20.5.8, class bad_optional_access
class bad_optional_access : public std::logic_error {
public:
    explicit bad_optional_access(const std::string& what_arg):
        std::logic_error{what_arg}
    {}

    explicit bad_optional_access(const char* what_arg):
        std::logic_error{what_arg}
    {}
};


template <typename T>
union storage_t
{
    unsigned char dummy_;
    T value_;

    constexpr storage_t( trivial_init_t ) noexcept:
        dummy_()
    {};

    template <typename... Args>
    constexpr storage_t( Args&&... args ):
        value_(constexpr_forward<Args>(args)...)
    {}

    ~storage_t(){}
};


template <typename T>
union constexpr_storage_t
{
    unsigned char dummy_;
    T value_;

    constexpr constexpr_storage_t( trivial_init_t ) noexcept:
        dummy_()
    {};

    template <typename... Args>
    constexpr constexpr_storage_t( Args&&... args ):
        value_(constexpr_forward<Args>(args)...)
    {}

    ~constexpr_storage_t() = default;
};


template <typename T>
struct optional_base
{
    bool init_;
    storage_t<T> storage_;

    constexpr optional_base() noexcept:
        init_(false),
        storage_(trivial_init)
    {};

    explicit constexpr optional_base(const T& v):
        init_(true),
        storage_(v)
    {}

    explicit constexpr optional_base(T&& v):
        init_(true),
        storage_(constexpr_move(v))
    {}

    template <typename... Args>
    explicit optional_base(in_place_t, Args&&... args):
        init_(true),
        storage_(constexpr_forward<Args>(args)...)
    {}

    template <typename U, typename... Args, TR2_OPTIONAL_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    explicit optional_base(in_place_t, std::initializer_list<U> il, Args&&... args):
        init_(true),
        storage_(il, std::forward<Args>(args)...)
    {}

    ~optional_base()
    {
        if (init_) storage_.value_.T::~T();
    }
};


template <typename T>
struct constexpr_optional_base
{
    bool init_;
    constexpr_storage_t<T> storage_;

    constexpr constexpr_optional_base() noexcept:
        init_(false),
        storage_(trivial_init)
    {};

    explicit constexpr constexpr_optional_base(const T& v):
        init_(true),
        storage_(v)
    {}

    explicit constexpr constexpr_optional_base(T&& v):
        init_(true),
        storage_(constexpr_move(v))
    {}

    template <typename... Args>
    explicit constexpr constexpr_optional_base(in_place_t, Args&&... args):
        init_(true),
        storage_(constexpr_forward<Args>(args)...)
    {}

    template <typename U, typename... Args, TR2_OPTIONAL_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    OPTIONAL_CONSTEXPR_INIT_LIST explicit constexpr_optional_base(in_place_t, std::initializer_list<U> il, Args&&... args):
        init_(true),
        storage_(il, std::forward<Args>(args)...)
    {}

    ~constexpr_optional_base() = default;
};

template <typename T>
using optional_base_t = typename std::conditional<
    is_trivially_destructible<T>::value,                          // if possible
    constexpr_optional_base<typename std::remove_const<T>::type>, // use base with trivial destructor
    optional_base<typename std::remove_const<T>::type>
>::type;


template <typename T>
class optional : private optional_base_t<T>
{
    static_assert( !std::is_same<typename std::decay<T>::type, nullopt_t>::value, "bad T" );
    static_assert( !std::is_same<typename std::decay<T>::type, in_place_t>::value, "bad T" );

    constexpr bool initialized() const noexcept
    {
        return optional_base_t<T>::init_;
    }

    typename std::remove_const<T>::type* dataptr()
    {
        return std::addressof(optional_base_t<T>::storage_.value_);
    }

    constexpr const T* dataptr() const
    {
        return detail::static_addressof(optional_base_t<T>::storage_.value_);
    }

#if OPTIONAL_HAS_THIS_RVALUE_REFS == 1        // RVALUE

    constexpr const T& contained_val() const&
    {
        return optional_base_t<T>::storage_.value_;
    }

#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1       // HAS MOVE

    OPTIONAL_MUTABLE_CONSTEXPR T&& contained_val() &&
    {
        return std::move(optional_base_t<T>::storage_.value_);
    }

    OPTIONAL_MUTABLE_CONSTEXPR T& contained_val() &
    {
        return optional_base_t<T>::storage_.value_;
    }

#   else                                      // NO RVALUE/MOVE

    T& contained_val() &
    {
        return optional_base_t<T>::storage_.value_;
    }

    T&& contained_val() &&
    {
        return std::move(optional_base_t<T>::storage_.value_);
    }

#   endif

#else

    constexpr const T& contained_val() const
    {
        return optional_base_t<T>::storage_.value_;
    }

    T& contained_val()
    {
        return optional_base_t<T>::storage_.value_;
    }

#endif

    void clear() noexcept
    {
        if (initialized()) {
            dataptr()->T::~T();
        }
        optional_base_t<T>::init_ = false;
    }

    template <typename... Args>
    void initialize(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
    {
        assert(!optional_base_t<T>::init_);
        ::new (static_cast<void*>(dataptr())) T(std::forward<Args>(args)...);
        optional_base_t<T>::init_ = true;
    }

    template <typename U, typename... Args>
    void initialize(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, std::forward<Args>(args)...)))
    {
        assert(!optional_base_t<T>::init_);
        ::new (static_cast<void*>(dataptr())) T(il, std::forward<Args>(args)...);
        optional_base_t<T>::init_ = true;
    }

public:
    typedef T value_type;

    // 20.5.5.1, constructors
    constexpr optional() noexcept : optional_base_t<T>()  {};
    constexpr optional(nullopt_t) noexcept : optional_base_t<T>() {};

    optional(const optional& rhs):
        optional_base_t<T>()
    {
        if (rhs.initialized()) {
            ::new (static_cast<void*>(dataptr())) T(*rhs);
            optional_base_t<T>::init_ = true;
        }
    }

    optional(optional&& rhs) noexcept(is_nothrow_move_constructible<T>::value):
        optional_base_t<T>()
    {
        if (rhs.initialized()) {
            ::new (static_cast<void*>(dataptr())) T(std::move(*rhs));
            optional_base_t<T>::init_ = true;
        }
    }

    constexpr optional(const T& v):
        optional_base_t<T>(v)
    {}

    constexpr optional(T&& v):
        optional_base_t<T>(constexpr_move(v))
    {}

    template <typename... Args>
    explicit constexpr optional(in_place_t, Args&&... args):
        optional_base_t<T>(in_place_t{}, constexpr_forward<Args>(args)...)
    {}

    template <typename U, typename... Args, TR2_OPTIONAL_REQUIRES(std::is_constructible<T, std::initializer_list<U>>)>
    OPTIONAL_CONSTEXPR_INIT_LIST explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args):
        optional_base_t<T>(in_place_t{}, il, constexpr_forward<Args>(args)...)
    {}

    // 20.5.4.2, Destructor
    ~optional() = default;

    // 20.5.4.3, assignment
    optional& operator=(nullopt_t) noexcept
    {
        clear();
        return *this;
    }

    optional& operator=(const optional& rhs)
    {
        if (initialized() == true  && rhs.initialized() == false) {
            clear();
        } else if (initialized() == false && rhs.initialized() == true) {
            initialize(*rhs);
        } else if (initialized() == true  && rhs.initialized() == true) {
            contained_val() = *rhs;
        }
        return *this;
    }

    optional& operator=(optional&& rhs)
    noexcept(is_nothrow_move_assignable<T>::value && is_nothrow_move_constructible<T>::value)
    {
        if (initialized() == true  && rhs.initialized() == false) {
            clear();
        } else if (initialized() == false && rhs.initialized() == true) {
            initialize(std::move(*rhs));
        } else if (initialized() == true  && rhs.initialized() == true) {
            contained_val() = std::move(*rhs);
        }
        return *this;
    }

    template <typename U>
    auto operator=(U&& v) -> typename std::enable_if<
        std::is_same<typename std::decay<U>::type, T>::value,
        optional&
    >::type
    {
        if (initialized()) {
            contained_val() = std::forward<U>(v);
        } else {
            initialize(std::forward<U>(v));
        }
        return *this;
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        clear();
        initialize(std::forward<Args>(args)...);
    }

    template <typename U, typename... Args>
    void emplace(std::initializer_list<U> il, Args&&... args)
    {
        clear();
        initialize<U, Args...>(il, std::forward<Args>(args)...);
    }

    // 20.5.4.4, Swap
    void swap(optional<T>& rhs) noexcept(is_nothrow_move_constructible<T>::value && noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))
    {
        if (initialized() == true  && rhs.initialized() == false) {
            rhs.initialize(std::move(**this));
            clear();
        } else if (initialized() == false && rhs.initialized() == true) {
            initialize(std::move(*rhs));
            rhs.clear();
        } else if (initialized() == true  && rhs.initialized() == true) {
            std::swap(**this, *rhs);
        }
    }

    // 20.5.4.5, Observers

    explicit constexpr operator bool() const noexcept
    {
        return initialized();
    }

    constexpr bool has_value() const noexcept
    {
        return initialized();
    }

    constexpr T const* operator ->() const
    {
        return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), dataptr());
    }

#if OPTIONAL_HAS_MOVE_ACCESSORS == 1

    OPTIONAL_MUTABLE_CONSTEXPR T* operator ->()
    {
        assert (initialized());
        return dataptr();
    }

    constexpr T const& operator *() const&
    {
        return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
    }

    OPTIONAL_MUTABLE_CONSTEXPR T& operator *() &
    {
        assert (initialized());
        return contained_val();
    }

    OPTIONAL_MUTABLE_CONSTEXPR T&& operator *() &&
    {
        assert (initialized());
        return constexpr_move(contained_val());
    }

    constexpr T const& value() const&
    {
        return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
    }

    OPTIONAL_MUTABLE_CONSTEXPR T& value() &
    {
        return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
    }

    OPTIONAL_MUTABLE_CONSTEXPR T&& value() &&
    {
        if (!initialized()) {
            throw bad_optional_access("bad optional access");
        }
        return std::move(contained_val());
    }

#else

    T* operator ->()
    {
        assert (initialized());
        return dataptr();
    }

    constexpr T const& operator *() const
    {
        return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
    }

    T& operator *()
    {
        assert (initialized());
        return contained_val();
    }

    constexpr T const& value() const
    {
        return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
    }

    T& value() {
        return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
    }

#endif

#if OPTIONAL_HAS_THIS_RVALUE_REFS == 1

    template <typename V>
    constexpr T value_or(V&& v) const&
    {
        return *this ? **this : detail::convert<T>(constexpr_forward<V>(v));
    }

#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1

    template <typename V>
    OPTIONAL_MUTABLE_CONSTEXPR T value_or(V&& v) &&
    {
        return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail::convert<T>(constexpr_forward<V>(v));
    }

#   else

    template <typename V>
    T value_or(V&& v) &&
    {
        return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail::convert<T>(constexpr_forward<V>(v));
    }

#   endif

#else

    template <typename V>
    constexpr T value_or(V&& v) const
    {
        return *this ? **this : detail::convert<T>(constexpr_forward<V>(v));
    }

#endif

    // 20.6.3.6, modifiers
    void reset() noexcept
    {
        clear();
    }
};


// OPTIONAL REF
// ------------


template <class T>
class optional<T&>
{
    static_assert(!std::is_same<T, nullopt_t>::value, "bad T");
    static_assert(!std::is_same<T, in_place_t>::value, "bad T");
    T* ref;

public:

    // 20.5.5.1, construction/destruction
    constexpr optional() noexcept:
        ref(nullptr)
    {}

    constexpr optional(nullopt_t) noexcept:
        ref(nullptr)
    {}

    constexpr optional(T& v) noexcept:
        ref(detail::static_addressof(v))
    {}

    optional(T&&) = delete;

    constexpr optional(const optional& rhs) noexcept:
        ref(rhs.ref)
    {}

    explicit constexpr optional(in_place_t, T& v) noexcept:
        ref(detail::static_addressof(v))
    {}

    explicit optional(in_place_t, T&&) = delete;

    ~optional() = default;

    // 20.5.5.2, mutation
    optional& operator=(nullopt_t) noexcept
    {
        ref = nullptr;
        return *this;
    }

    template <typename U>
    auto operator=(U&& rhs) noexcept -> typename std::enable_if<
        std::is_same<typename std::decay<U>::type, optional<T&>>::value,
        optional&
    >::type
    {
        ref = rhs.ref;
        return *this;
    }

    template <typename U>
    auto operator=(U&& rhs) noexcept -> typename std::enable_if<
        !std::is_same<typename std::decay<U>::type, optional<T&>>::value,
        optional&
    >::type = delete;

    void emplace(T& v) noexcept {
        ref = detail::static_addressof(v);
    }

    void emplace(T&&) = delete;

    void swap(optional<T&>& rhs) noexcept
    {
        std::swap(ref, rhs.ref);
    }

    // 20.5.5.3, observers
    constexpr T* operator->() const
    {
        return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, ref);
    }

    constexpr T& operator*() const
    {
        return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, *ref);
    }

    constexpr T& value() const
    {
        return ref ? *ref : (throw bad_optional_access("bad optional access"), *ref);
    }

    explicit constexpr operator bool() const noexcept
    {
        return ref != nullptr;
    }

    constexpr bool has_value() const noexcept
    {
        return ref != nullptr;
    }

    template <typename V>
    constexpr typename std::decay<T>::type value_or(V&& v) const
    {
        return *this ? **this : detail::convert<typename std::decay<T>::type>(constexpr_forward<V>(v));
    }

    // x.x.x.x, modifiers
    void reset() noexcept
    {
        ref = nullptr;
    }
};


// OPTIONAL FWD
// ------------


template <typename T>
class optional<T&&>
{
    static_assert( sizeof(T) == 0, "optional rvalue references disallowed" );
};


// RELATIONAL OPERATORS
// --------------------

// 20.5.8, Relational operators
template <typename T> constexpr bool operator==(const optional<T>& x, const optional<T>& y)
{
    return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <typename T> constexpr bool operator!=(const optional<T>& x, const optional<T>& y)
{
    return !(x == y);
}

template <typename T> constexpr bool operator<(const optional<T>& x, const optional<T>& y)
{
    return (!y) ? false : (!x) ? true : *x < *y;
}

template <typename T> constexpr bool operator>(const optional<T>& x, const optional<T>& y)
{
    return (y < x);
}

template <typename T> constexpr bool operator<=(const optional<T>& x, const optional<T>& y)
{
    return !(y < x);
}

template <typename T> constexpr bool operator>=(const optional<T>& x, const optional<T>& y)
{
    return !(x < y);
}


// 20.5.9, Comparison with nullopt
template <typename T> constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept
{
    return (!x);
}

template <typename T> constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept
{
    return (!x);
}

template <typename T> constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept
{
    return bool(x);
}

template <typename T> constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept
{
    return bool(x);
}

template <typename T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept
{
    return false;
}

template <typename T> constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept
{
    return bool(x);
}

template <typename T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept
{
    return (!x);
}

template <typename T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept
{
    return true;
}

template <typename T> constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept
{
    return bool(x);
}

template <typename T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept
{
    return false;
}

template <typename T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept
{
    return true;
}

template <typename T> constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept
{
    return (!x);
}


// 20.5.10, Comparison with T
template <typename T> constexpr bool operator==(const optional<T>& x, const T& v)
{
    return bool(x) ? *x == v : false;
}

template <typename T> constexpr bool operator==(const T& v, const optional<T>& x)
{
    return bool(x) ? v == *x : false;
}

template <typename T> constexpr bool operator!=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x != v : true;
}

template <typename T> constexpr bool operator!=(const T& v, const optional<T>& x)
{
    return bool(x) ? v != *x : true;
}

template <typename T> constexpr bool operator<(const optional<T>& x, const T& v)
{
    return bool(x) ? *x < v : true;
}

template <typename T> constexpr bool operator>(const T& v, const optional<T>& x)
{
    return bool(x) ? v > *x : true;
}

template <typename T> constexpr bool operator>(const optional<T>& x, const T& v)
{
    return bool(x) ? *x > v : false;
}

template <typename T> constexpr bool operator<(const T& v, const optional<T>& x)
{
    return bool(x) ? v < *x : false;
}

template <typename T> constexpr bool operator>=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x >= v : false;
}

template <typename T> constexpr bool operator<=(const T& v, const optional<T>& x)
{
    return bool(x) ? v <= *x : false;
}

template <typename T> constexpr bool operator<=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x <= v : true;
}

template <typename T> constexpr bool operator>=(const T& v, const optional<T>& x)
{
    return bool(x) ? v >= *x : true;
}


// Comparison of optional<T&> with T
template <typename T> constexpr bool operator==(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x == v : false;
}

template <typename T> constexpr bool operator==(const T& v, const optional<T&>& x)
{
    return bool(x) ? v == *x : false;
}

template <typename T> constexpr bool operator!=(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x != v : true;
}

template <typename T> constexpr bool operator!=(const T& v, const optional<T&>& x)
{
    return bool(x) ? v != *x : true;
}

template <typename T> constexpr bool operator<(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x < v : true;
}

template <typename T> constexpr bool operator>(const T& v, const optional<T&>& x)
{
    return bool(x) ? v > *x : true;
}

template <typename T> constexpr bool operator>(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x > v : false;
}

template <typename T> constexpr bool operator<(const T& v, const optional<T&>& x)
{
    return bool(x) ? v < *x : false;
}

template <typename T> constexpr bool operator>=(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x >= v : false;
}

template <typename T> constexpr bool operator<=(const T& v, const optional<T&>& x)
{
    return bool(x) ? v <= *x : false;
}

template <typename T> constexpr bool operator<=(const optional<T&>& x, const T& v)
{
    return bool(x) ? *x <= v : true;
}

template <typename T> constexpr bool operator>=(const T& v, const optional<T&>& x)
{
    return bool(x) ? v >= *x : true;
}

// Comparison of optional<T const&> with T
template <typename T> constexpr bool operator==(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x == v : false;
}

template <typename T> constexpr bool operator==(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v == *x : false;
}

template <typename T> constexpr bool operator!=(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x != v : true;
}

template <typename T> constexpr bool operator!=(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v != *x : true;
}

template <typename T> constexpr bool operator<(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x < v : true;
}

template <typename T> constexpr bool operator>(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v > *x : true;
}

template <typename T> constexpr bool operator>(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x > v : false;
}

template <typename T> constexpr bool operator<(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v < *x : false;
}

template <typename T> constexpr bool operator>=(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x >= v : false;
}

template <typename T> constexpr bool operator<=(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v <= *x : false;
}

template <typename T> constexpr bool operator<=(const optional<const T&>& x, const T& v)
{
    return bool(x) ? *x <= v : true;
}

template <typename T> constexpr bool operator>=(const T& v, const optional<const T&>& x)
{
    return bool(x) ? v >= *x : true;
}

// FRIENDS
// -------

// 20.5.12, Specialized algorithms
template <typename T>
void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
{
    x.swap(y);
}


template <typename T>
constexpr optional<typename std::decay<T>::type> make_optional(T&& v)
{
    return optional<typename std::decay<T>::type>(constexpr_forward<T>(v));
}

template <typename X>
constexpr optional<X&> make_optional(std::reference_wrapper<X> v)
{
    return optional<X&>(v.get());
}

// CLEANUP
// -------

#undef TR2_OPTIONAL_REQUIRES
#undef TR2_OPTIONAL_ASSERTED_EXPRESSION

#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE

#if !defined(HAVE_CPP17)             // HAVE_CPP17

namespace std
{
// SPECIALIZATION
// --------------

PYCPP_USING_NAMESPACE

template <typename T>
struct hash<optional<T>>
{
    typedef typename hash<T>::result_type result_type;
    typedef optional<T> argument_type;

    constexpr result_type operator()(const argument_type& arg) const
    {
        return arg ? std::hash<T>{}(*arg) : result_type{};
    }
};


template <typename T>
struct hash<optional<T&>>
{
    typedef typename hash<T>::result_type result_type;
    typedef optional<T&> argument_type;

    constexpr result_type operator()(const argument_type& arg) const
    {
        return arg ? std::hash<T>{}(*arg) : result_type{};
    }
};

}   /* std */

#endif                              // HAVE_CPP17


PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

template <typename T>
struct hash<optional<T>>
{
    typedef typename hash<T>::result_type result_type;
    typedef optional<T> argument_type;

    constexpr result_type operator()(const argument_type& arg) const
    {
        return std::hash<argument_type>()(arg);
    }
};

template <typename T>
struct hash<optional<T&>>
{
    typedef typename hash<T>::result_type result_type;
    typedef optional<T&> argument_type;

    constexpr result_type operator()(const argument_type& arg) const
    {
        return std::hash<argument_type>()(arg);
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
