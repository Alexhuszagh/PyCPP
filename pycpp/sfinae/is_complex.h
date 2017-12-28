//  :copyright: (c) 2007 John Maddock.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if the type is a complex number.
 *
 *  Detect if the type is a specialization of `std::complex`.
 *
 *  \synopsis
 *      template <typename T
 *      using is_complex<T> = implementation-defined;
 */

#include <pycpp/sfinae/is_specialization.h>
#include <pycpp/stl/complex.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
using is_complex = is_specialization<T, complex>;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_complex_v = is_complex<T>::value;

#endif

PYCPP_END_NAMESPACE
