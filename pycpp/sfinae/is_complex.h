//  :copyright: (c) 2007 John Maddock.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if the type is a complex number.
 */

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>
#include <complex>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
struct is_complex: std::false_type
{};

template <typename T>
struct is_complex<const T>: is_complex<T>
{};

template <typename T>
struct is_complex<volatile const T>: is_complex<T>
{};

template <typename T>
struct is_complex<volatile T>: is_complex<T>
{};

template <typename T>
struct is_complex<std::complex<T>>: std::true_type
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_complex_v = is_complex<T>::value;

#endif

PYCPP_END_NAMESPACE
