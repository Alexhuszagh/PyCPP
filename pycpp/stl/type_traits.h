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

// Convertible
using std::is_convertible;

// Constructable
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

// Destructable
using std::is_destructible;
using std::is_trivially_destructible;
using std::is_nothrow_destructible;

// Memory
using std::aligned_storage;
using std::alignment_of;

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

template <typename T>
constexpr bool is_destructible_v = is_destructible<T>::value;

template <typename T>
constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

template <typename T>
constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

template <typename T>
constexpr size_t alignment_of_v = alignment_of<T>::value;

#endif          // HAVE_CPP14

PYCPP_END_NAMESPACE
