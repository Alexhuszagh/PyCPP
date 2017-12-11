//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <type_traits> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>

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

template <typename ... Ts>
using common_type = std::common_type<Ts...>;

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
template <size_t Len, size_t Align>
using aligned_storage = std::aligned_storage<Len, Align>;

template <typename T>
using alignment_of = std::alignment_of<T>;

// Integral Constant
template <typename T, T v>
using integral_constant = std::integral_constant<T, v>;

using true_type = std::true_type;
using false_type = std::false_type;

// TODO: need C++14 overloads

PYCPP_END_NAMESPACE
