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
template <typename T>
using is_arithmetic = std::is_arithmetic<T>;

template <typename T>
using is_integral = std::is_integral<T>;

template <typename T>
using is_floating_point = std::is_floating_point<T>;

template <typename T, typename U>
using is_same = std::is_same<T, U>;

template <typename T, typename U>
using is_base_of = std::is_base_of<T, U>;

template <typename T>
using is_reference = std::is_reference<T>;

template <typename T>
using is_array = std::is_array<T>;

template <typename T>
using is_void = std::is_void<T>;

template <typename T>
using is_lvalue_reference = std::is_lvalue_reference<T>;

template <typename T>
using is_rvalue_reference = std::is_rvalue_reference<T>;

template <typename T>
using is_enum = std::is_enum<T>;

template <typename T>
using is_empty = std::is_empty<T>;

template <typename ... Ts>
using common_type = std::common_type<Ts...>;

// Conversion
template <typename Enum>
using underlying_type = std::underlying_type<Enum>;

// Functions
template <typename F, typename ... Ts>
using result_of = std::result_of<F, Ts...>;

// Remove qualifiers
template <typename T>
using remove_const = std::remove_const<T>;

template <typename T>
using remove_volatile = std::remove_volatile<T>;

template <typename T>
using remove_cv = std::remove_cv<T>;

template <typename T>
using remove_reference = std::remove_reference<T>;

template <typename T>
using remove_extent = std::remove_extent<T>;

template <typename T>
using decay = std::decay<T>;

// Add qualifiers
template <typename T>
using add_cv = std::add_cv<T>;

template <typename T>
using add_const = std::add_const<T>;

template <typename T>
using add_volatile = std::add_volatile<T>;

template <typename T>
using add_lvalue_reference = std::add_lvalue_reference<T>;

template <typename T>
using add_rvalue_reference = std::add_rvalue_reference<T>;

template <typename T>
using add_pointer = std::add_pointer<T>;

// Logic
template <bool B, typename T = void>
using enable_if = std::enable_if<B, T>;

template <bool B, typename T, typename F>
using conditional = std::conditional<B, T, F>;

// Convertible
template <typename From, typename To>
using is_convertible = std::is_convertible<From, To>;

// Constructable
template <typename T, typename ... Ts>
using is_constructible = std::is_constructible<T, Ts...>;

template <typename T, typename ... Ts>
using is_trivially_constructible = std::is_trivially_constructible<T, Ts...>;

template <typename T, typename ... Ts>
using is_nothrow_constructible = std::is_nothrow_constructible<T, Ts...>;

template <typename T>
using is_default_constructible = std::is_default_constructible<T>;

template <typename T>
using is_trivially_default_constructible = std::is_trivially_default_constructible<T>;

template <typename T>
using is_nothrow_default_constructible = std::is_nothrow_default_constructible<T>;

template <typename T>
using is_copy_constructible = std::is_copy_constructible<T>;

template <typename T>
using is_trivially_copy_constructible = std::is_trivially_copy_constructible<T>;

template <typename T>
using is_nothrow_copy_constructible = std::is_nothrow_copy_constructible<T>;

template <typename T>
using is_move_constructible = std::is_move_constructible<T>;

template <typename T>
using is_trivially_move_constructible = std::is_trivially_move_constructible<T>;

template <typename T>
using is_nothrow_move_constructible = std::is_nothrow_move_constructible<T>;

// Assignable
template <typename T, typename ... Ts>
using is_assignable = std::is_assignable<T, Ts...>;

template <typename T, typename ... Ts>
using is_trivially_assignable = std::is_trivially_assignable<T, Ts...>;

template <typename T, typename ... Ts>
using is_nothrow_assignable = std::is_nothrow_assignable<T, Ts...>;

template <typename T>
using is_copy_assignable = std::is_copy_assignable<T>;

template <typename T>
using is_trivially_copy_assignable = std::is_trivially_copy_assignable<T>;

template <typename T>
using is_nothrow_copy_assignable = std::is_nothrow_copy_assignable<T>;

template <typename T>
using is_move_assignable = std::is_move_assignable<T>;

template <typename T>
using is_trivially_move_assignable = std::is_trivially_move_assignable<T>;

template <typename T>
using is_nothrow_move_assignable = std::is_nothrow_move_assignable<T>;

// Destructable
template <typename T>
using is_destructible = std::is_destructible<T>;

template <typename T>
using is_trivially_destructible = std::is_trivially_destructible<T>;

template <typename T>
using is_nothrow_destructible = std::is_nothrow_destructible<T>;

// Memory
template <
    size_t Len,
    size_t Align = alignof(typename std::aligned_storage<Len>::type)
>
using aligned_storage = std::aligned_storage<Len, Align>;

template <typename T>
using alignment_of = std::alignment_of<T>;

// Void
template <typename...>
using void_t = void;

// Integral Constant
template <typename T, T v>
using integral_constant = std::integral_constant<T, v>;

template <bool B>
using bool_constant = integral_constant<bool, B>;

template <size_t N>
using size_constant = integral_constant<size_t, N>;

using true_type = std::true_type;
using false_type = std::false_type;

// CONVENIENCE
// -----------

template <typename ... Ts>
using common_type_t = typename std::common_type<Ts...>::type;

template <typename Enum>
using underlying_type_t = typename std::underlying_type<Enum>::type;

template <typename F, typename ... Ts>
using result_of_t = typename std::result_of<F, Ts...>::type;

template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

template <typename T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
using remove_extent_t = typename std::remove_extent<T>::type;

template <typename T>
using decay_t = typename std::decay<T>::type;

template <typename T>
using add_cv_t = typename std::add_cv<T>::type;

template <typename T>
using add_const_t = typename std::add_const<T>::type;

template <typename T>
using add_volatile_t = typename std::add_volatile<T>::type;

template <typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

template <typename T>
using add_pointer_t = typename std::add_pointer<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <bool B, typename T, typename F>
using conditional_t = typename std::conditional<B, T, F>::type;

template <
    size_t Len,
    size_t Align = alignof(typename std::aligned_storage<Len>::type)
>
using aligned_storage_t = typename std::aligned_storage<Len, Align>::type;
// TODO:

#if defined(HAVE_CPP14)

template <typename T>
constexpr bool is_arithmetic_v = std::is_arithmetic<T>::value;

template <typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template <typename T>
constexpr bool is_floating_point_v = std::is_floating_point<T>::value;

template <typename T, typename U>
constexpr bool is_same_v = std::is_same<T, U>::value;

template <typename T, typename U>
constexpr bool is_base_of_v = std::is_base_of<T, U>::value;

template <typename T>
constexpr bool is_reference_v = std::is_reference<T>::value;

template <typename T>
constexpr bool is_array_v = std::is_array<T>::value;

template <typename T>
constexpr bool is_void_v = std::is_void<T>::value;

template <typename T>
constexpr bool is_lvalue_reference_v = std::is_lvalue_reference<T>::value;

template <typename T>
constexpr bool is_rvalue_reference_v = std::is_rvalue_reference<T>::value;

template <typename T>
constexpr bool is_enum_v = std::is_enum<T>::value;

template <typename T>
constexpr bool is_empty_v = std::is_empty<T>::value;

template <typename From, typename To>
constexpr bool is_convertible_v = std::is_convertible<From, To>::value;

template <typename T, typename ... Ts>
constexpr bool is_constructible_v = std::is_constructible<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_trivially_constructible_v = std::is_trivially_constructible<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_constructible_v = std::is_nothrow_constructible<T, Ts...>::value;

template <typename T>
constexpr bool is_default_constructible_v = std::is_default_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_default_constructible_v = std::is_trivially_default_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_default_constructible_v = std::is_nothrow_default_constructible<T>::value;

template <typename T>
constexpr bool is_copy_constructible_v = std::is_copy_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_copy_constructible_v = std::is_trivially_copy_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_constructible_v = std::is_nothrow_copy_constructible<T>::value;

template <typename T>
constexpr bool is_move_constructible_v = std::is_move_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_move_constructible_v = std::is_trivially_move_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_move_constructible_v = std::is_nothrow_move_constructible<T>::value;

template <typename T, typename ... Ts>
constexpr bool is_assignable_v = std::is_assignable<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_trivially_assignable_v = std::is_trivially_assignable<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_assignable_v = std::is_nothrow_assignable<T, Ts...>::value;

template <typename T>
constexpr bool is_copy_assignable_v = std::is_copy_assignable<T>::value;

template <typename T>
constexpr bool is_trivially_copy_assignable_v = std::is_trivially_copy_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_assignable_v = std::is_nothrow_copy_assignable<T>::value;

template <typename T>
constexpr bool is_move_assignable_v = std::is_move_assignable<T>::value;

template <typename T>
constexpr bool is_trivially_move_assignable_v = std::is_trivially_move_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_move_assignable_v = std::is_nothrow_move_assignable<T>::value;

template <typename T>
constexpr bool is_destructible_v = std::is_destructible<T>::value;

template <typename T>
constexpr bool is_trivially_destructible_v = std::is_trivially_destructible<T>::value;

template <typename T>
constexpr bool is_nothrow_destructible_v = std::is_nothrow_destructible<T>::value;

template <typename T>
constexpr size_t alignment_of_v = std::alignment_of<T>::value;

#endif          // HAVE_CPP14

PYCPP_END_NAMESPACE
