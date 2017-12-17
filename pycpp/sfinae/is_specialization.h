//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for specialized types.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

/**
 *  Check if a class is specialized from a template class.
 *  Does not work if the base template class is an alias.
 *  For example, `is_specialization<string, basic_string>`
 *  will fail, howwver, `is_specialization<string,
 *  std::basic_string>` will work.
 */
template <typename T, template <typename...> class C>
struct is_specialization: false_type
{};

template <template <typename...> class C, typename... Ts>
struct is_specialization<C<Ts...>, C>: true_type
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T, template <typename...> class C>
constexpr bool is_specialization_v = is_specialization<T, C>::value;

#endif

PYCPP_END_NAMESPACE
