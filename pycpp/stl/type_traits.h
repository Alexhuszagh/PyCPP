//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <type_traits> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>
#include <utility>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Type deduction
using std::is_arithmetic;
using std::is_integral;
using std::is_floating_point;
using std::is_same;
using std::is_base_of;
using std::is_reference;
using std::is_array;
using std::is_void;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_class;
using std::is_enum;
using std::is_empty;
using std::is_union;
using std::is_trivial;
using std::is_trivially_copyable;
using std::is_standard_layout;
using std::is_literal_type;
using std::is_pod;
using std::is_polymorphic;
using std::is_abstract;
using std::is_signed;
using std::is_unsigned;
using std::common_type;

// Conversion
using std::underlying_type;

// Functions
using std::result_of;

// Check qualifiers
using std::is_const;
using std::is_volatile;

// Remove qualifiers
using std::remove_const;
using std::remove_volatile;
using std::remove_cv;
using std::remove_pointer;
using std::remove_reference;
using std::remove_extent;
using std::decay;

// Add qualifiers
using std::add_cv;
using std::add_const;
using std::add_volatile;
using std::add_lvalue_reference;
using std::add_rvalue_reference;
using std::add_pointer;

// Logic
using std::enable_if;
using std::conditional;

// Helpers
template <typename...>
using void_t = void;

// Integral Constant
using std::integral_constant;

template <bool B>
using bool_constant = integral_constant<bool, B>;

template <size_t N>
using size_constant = integral_constant<size_t, N>;

using std::true_type;
using std::false_type;

// Convertible
using std::is_convertible;

// Constructible
using std::is_constructible;
using std::is_trivially_constructible;
using std::is_nothrow_constructible;
using std::is_default_constructible;
using std::is_trivially_default_constructible;
using std::is_nothrow_default_constructible;
using std::is_copy_constructible;
using std::is_trivially_copy_constructible;
using std::is_nothrow_copy_constructible;
using std::is_move_constructible;
using std::is_trivially_move_constructible;
using std::is_nothrow_move_constructible;

// Assignable
using std::is_assignable;
using std::is_trivially_assignable;
using std::is_nothrow_assignable;
using std::is_copy_assignable;
using std::is_trivially_copy_assignable;
using std::is_nothrow_copy_assignable;
using std::is_move_assignable;
using std::is_trivially_move_assignable;
using std::is_nothrow_move_assignable;

// Swappable (C++17 backport)
template <typename T1, typename T2>
struct is_swappable_with_impl
{
private:
    template <typename U1, typename U2, typename = decltype(swap(std::declval<U1&>(), std::declval<U2&>()))>
    inline static true_type test(int);

    template <typename U1, typename U2>
    inline static false_type test(...);

public:
    using type = decltype(test<T1, T2>(0));
};

template <typename T1, typename T2>
using is_swappable_with = typename is_swappable_with_impl<T1, T2>::type;

template <typename T1, typename T2, bool = is_swappable_with<T1, T2>::value>
struct is_nothrow_swappable_with
{
    static constexpr bool value = noexcept(swap(std::declval<T1&>(), std::declval<T2&>()));
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with<T1, T2, false>: false_type
{};

template <typename T>
using is_swappable = is_swappable_with<T, T>;

template <typename T>
using is_nothrow_swappable = is_nothrow_swappable_with<T, T>;

// Relocatable (Proposed extension)
// `is_relocatable` should be specialized for any types if possible,
// since it allows raw bitwise copies.
// Inspired by Working Group paper P0023R0.
template <typename T>
struct is_relocatable: bool_constant<
        // Empty classes rely on global, or no state,
        // and therefore can always be relocated.
        is_empty<T>::value ||
        // Trivially copyable classes are guaranteed to be copyable
        // via `memcpy`.
        is_trivially_copyable<T>::value ||
        // Trivially move-constructible classes should also satisfy
        // trivially copyable types.
        is_trivially_move_constructible<T>::value
    >
{};

// Virtual classes using virtual tables **are** relocatable, since
// they effectively contain a virtual table with a pointer to
// a static table containing the virtual function pointers.
// Almost every C++ virtual implementation uses vtables, however,
// it is not standardized, so we assume they are **not** copyable.
// If need be, specialize this to allow relocatable virtual classes,
// using book-keeping to track compilers using vtables.
using is_virtual_relocatable = false_type;

// Destructible
using std::is_destructible;
using std::is_trivially_destructible;
using std::is_nothrow_destructible;

// Memory
using std::aligned_storage;
using std::alignment_of;

// CONVENIENCE
// -----------

template <typename ... Ts>
using common_type_t = typename common_type<Ts...>::type;

template <typename Enum>
using underlying_type_t = typename underlying_type<Enum>::type;

template <typename T>
using result_of_t = typename result_of<T>::type;

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename T>
using decay_t = typename decay<T>::type;

template <typename T>
using add_cv_t = typename add_cv<T>::type;

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

template <
    size_t Len,
    size_t Align = alignof(typename aligned_storage<Len>::type)
>
using aligned_storage_t = typename aligned_storage<Len, Align>::type;

#if defined(HAVE_CPP14)

template <typename T>
constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <typename T>
constexpr bool is_integral_v = is_integral<T>::value;

template <typename T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

template <typename T, typename U>
constexpr bool is_base_of_v = is_base_of<T, U>::value;

template <typename T>
constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
constexpr bool is_array_v = is_array<T>::value;

template <typename T>
constexpr bool is_void_v = is_void<T>::value;

template <typename T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <typename T>
constexpr bool is_class_v = is_class<T>::value;

template <typename T>
constexpr bool is_enum_v = is_enum<T>::value;

template <typename T>
constexpr bool is_empty_v = is_empty<T>::value;

template <typename T>
constexpr bool is_union_v = is_union<T>::value;

template <typename T>
constexpr bool is_trivial_v = is_trivial<T>::value;

template <typename T>
constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

template <typename T>
constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

template <typename T>
constexpr bool is_literal_type_v = is_literal_type<T>::value;

template <typename T>
constexpr bool is_pod_v = is_pod<T>::value;

template <typename T>
constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

template <typename T>
constexpr bool is_abstract_v = is_abstract<T>::value;

template <typename T>
constexpr bool is_signed_v = is_signed<T>::value;

template <typename T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <typename T>
constexpr bool is_const_v = is_const<T>::value;

template <typename T>
constexpr bool is_volatile_v = is_volatile<T>::value;

template <typename From, typename To>
constexpr bool is_convertible_v = is_convertible<From, To>::value;

template <typename T, typename ... Ts>
constexpr bool is_constructible_v = is_constructible<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Ts...>::value;

template <typename T>
constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

template <typename T>
constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

template <typename T>
constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

template <typename T, typename ... Ts>
constexpr bool is_assignable_v = is_assignable<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, Ts...>::value;

template <typename T>
constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <typename T>
constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

template <typename T>
constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

template <typename T>
constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

template <typename T, typename U>
constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

template <typename T, typename U>
constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T, U>::value;

template <typename T>
constexpr bool is_swappable_v = is_swappable<T>::value;

template <typename T>
constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

template <typename T>
constexpr bool is_destructible_v = is_destructible<T>::value;

template <typename T>
constexpr bool is_relocatable_v = is_relocatable<T>::value;

template <typename T>
constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

template <typename T>
constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

template <typename T>
constexpr size_t alignment_of_v = alignment_of<T>::value;

#endif          // HAVE_CPP14

namespace type_detail
{
// DETAIL
// ------

template <template <typename> class T, typename ... Ts>
struct map_and;


template <template <typename> class T>
struct map_and<T>: true_type
{};


template <template <typename> class T, typename U, typename ... Ts>
struct map_and<T, U, Ts...>: conditional_t<T<U>::value && map_and<T, Ts...>::value, true_type, false_type>
{};


template <template <typename> class T, typename ... Ts>
struct map_or;


template <template <typename> class T>
struct map_or<T>: false_type
{};


template <template <typename> class T, typename U, typename ... Ts>
struct map_or<T, U, Ts...>: conditional_t<T<U>::value || map_or<T, Ts...>::value, true_type, false_type>
{};

}   /* type_detail */

// MACROS
// ------

// Specify no-except conditions for 1-5 arguments
#define PYCPP_TYPE_ID(x) x
#define PYCPP_GET_TYPE(_1,_2,_3,_4,_5,NAME,...) NAME

// noexcept(noexcept(is_nothrow_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_default_constructible, T1>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_default_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_default_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_default_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_default_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_copy_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_constructible, T1>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_copy_assignable<Ts...>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_assignable, T1>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_assignable, T1, T2>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_assignable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_assignable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_copy_assignable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_COPY_ASSIGNABLE5, PYCPP_NOEXCEPT_COPY_ASSIGNABLE4, PYCPP_NOEXCEPT_COPY_ASSIGNABLE3, PYCPP_NOEXCEPT_COPY_ASSIGNABLE2, PYCPP_NOEXCEPT_COPY_ASSIGNABLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_move_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_move_constructible, T1>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_move_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_move_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_move_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_move_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_move_assignable<Ts...>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_move_assignable, T1>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_move_assignable, T1, T2>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_move_assignable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_move_assignable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_move_assignable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE5, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE4, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE3, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE2, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_swappable<Ts...>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE1(T1) noexcept(noexcept(type_detail::map_and<is_nothrow_swappable, T1>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE2(T1, T2) noexcept(noexcept(type_detail::map_and<is_nothrow_swappable, T1, T2>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE3(T1, T2, T3) noexcept(noexcept(type_detail::map_and<is_nothrow_swappable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE4(T1, T2, T3, T4) noexcept(noexcept(type_detail::map_and<is_nothrow_swappable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(type_detail::map_and<is_nothrow_swappable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_SWAPPABLE5, PYCPP_NOEXCEPT_SWAPPABLE4, PYCPP_NOEXCEPT_SWAPPABLE3, PYCPP_NOEXCEPT_SWAPPABLE2, PYCPP_NOEXCEPT_SWAPPABLE1)(__VA_ARGS__))

PYCPP_END_NAMESPACE
