//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/math/distribution.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// PDF

double norm_pdf(double x) noexcept
{
    return exp(-pow(x, 2) / 2.) / NORM_PDF;
}


double gaussian_pdf(double x, double mean, double sigma) noexcept
{
    return norm_pdf((x - mean) / sigma);
}


double cauchy_pdf(double x) noexcept
{
    return 1.0 / (CAUCHY_CDF * (1.0 + pow(x, 2)));
}


double lorentzian_pdf(double x, double mean, double fwhm) noexcept
{
    double width = fwhm / 2.0;

    return cauchy_pdf((x - mean) / width);
}


// CDF


double norm_cdf(double x) noexcept
{
    return (1. + erf(x / sqrt(2.))) / 2.;
}


double gaussian_cdf(double x, double mean, double sigma) noexcept
{
    return norm_cdf((x - mean) / sigma);
}


double cauchy_cdf(double x) noexcept
{
    return 0.5 + atan(x) / CAUCHY_CDF;
}


double lorentzian_cdf(double x, double mean, double fwhm) noexcept
{
    double width = fwhm / 2.0;

    return cauchy_cdf((x - mean) / width);
}


// OBJECT-ORIENTED


double norm::pdf(double x) const noexcept
{
    return norm_pdf(x);
}


double norm::cdf(double x) const noexcept
{
    return norm_cdf(x);
}


gaussian::gaussian(double mean, double sigma) noexcept:
    mean(mean),
    sigma(sigma)
{}


double gaussian::pdf(double x) const noexcept
{
    return gaussian_pdf(x, mean, sigma);
}


double gaussian::cdf(double x) const noexcept
{
    return gaussian_cdf(x, mean, sigma);
}


double cauchy::pdf(double x) const noexcept
{
    return cauchy_pdf(x);
}


double cauchy::cdf(double x) const noexcept
{
    return cauchy_cdf(x);
}


lorentzian::lorentzian(double mean, double fwhm) noexcept:
    mean(mean),
    fwhm(fwhm)
{}


double lorentzian::pdf(double x) const noexcept
{
    return lorentzian_pdf(x, mean, fwhm);
}


double lorentzian::cdf(double x) const noexcept
{
    return lorentzian_cdf(x, mean, fwhm);
}

PYCPP_END_NAMESPACE
