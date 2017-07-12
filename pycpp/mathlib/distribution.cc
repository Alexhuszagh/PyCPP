//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/mathlib/distribution.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

double norm_pdf(const double x)
{
    return exp(-pow(x, 2) / 2.) / NORM_PDF;
}

PYCPP_END_NAMESPACE
