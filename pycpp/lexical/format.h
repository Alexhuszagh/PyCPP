//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Preferred formatting for special types and numbers.
 *
 *  These values can be changed at runtime to change how the
 *  leixcal formatter represents strings.
 */

#include <pycpp/stl/string.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

/**
 *  \brief Representation of `nullptr`.
 */
extern string NULL_STRING;

/**
 *  \brief Representation of `true`.
 */
extern string TRUE_STRING;

/**
 *  \brief Representation of `false`.
 */
extern string FALSE_STRING;

/**
 *  \brief Representation of IEEE754 NaN.
 *  This must be <= 23 characters, or else may cause buffer overflow.
 */
extern string NAN_STRING;

/**
 *  \brief Representation of IEEE754 INF (-INF prepends a minus sign).
 *  This must be <= 23 characters, or else may cause buffer overflow.
 */
extern string INFINITY_STRING;

PYCPP_END_NAMESPACE
