//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Precise floating-point numbers.
 *
 *  Up to a quadruple-precision floating-point number.
 */

#include <pycpp/preprocessor/compiler.h>

#ifdef HAVE_GCC
#   include <quadmath.h>
#endif

PYCPP_BEGIN_NAMESPACE

// Precise float
// Up to a quad-precision floating-point number.
#if defined(HAVE_GCC) && ((COMPILER_MAJOR_VERSION > 4) || ((COMPILER_MAJOR_VERSION == 4) && (COMPILER_MINOR_VERSION >= 6)))
    using precise_float_t = __float128;
#elif defined(HAVE_INTEL)
    using precise_float_t = _Quad;
#else
    using precise_float_t = long double;
#endif

PYCPP_END_NAMESPACE
