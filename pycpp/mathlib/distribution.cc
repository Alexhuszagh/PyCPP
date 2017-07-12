//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/mathlib/distribution.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

double norm_pdf(double x)
{
    return exp(-pow(x, 2) / 2.) / NORM_PDF;
}


double gaussian_pdf(double x, double mean, double sigma)
{
    return norm_pdf((x - mean) / sigma);
}


double cauchy_pdf(double x)
{
    return 1.0 / (CAUCHY_CDF * (1.0 + pow(x, 2)));
}


double lorentzian_pdf(double x, double mean, double fwhm)
{
    double width = fwhm / 2.0;

    return cauchy_pdf((x - mean) / width);
}


double norm_cdf(double x)
{
    return (1. + erf(x / sqrt(2.))) / 2.;
}


double gaussian_cdf(double x, double mean, double sigma)
{
    return norm_cdf((x - mean) / sigma);
}


double cauchy_cdf(double x)
{
    return 0.5 + atan(x) / CAUCHY_CDF;
}


double lorentzian_cdf(double x, double mean, double fwhm)
{
    double width = fwhm / 2.0;

    return cauchy_cdf((x - mean) / width);
}


PYCPP_END_NAMESPACE
