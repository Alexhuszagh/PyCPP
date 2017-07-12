//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Math
 *  \brief Routines for distribution PDFs and CDFs.
 */

#pragma once

#include <pycpp/config.h>
#include <iterator>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr double CAUCHY_CDF = M_PI;
static constexpr double NORM_PDF = sqrt(2 * M_PI);

// FUNCTIONS
// ---------

// PDFS

/** \brief Calculate probability density function for normed gaussian.
 *
 *  \param x                Position for normed PDF.
 */
double norm_pdf(double x);

/** \brief Calculate probability density function for gaussian.
 *
 *  \param x                Position for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
double gaussian_pdf(double x, double mean, double sigma);

/** \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Position for Cauchy PDF.
 */
double cauchy_pdf(double x);

/** \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Position for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
double lorentzian_pdf(double x, double mean, double fwhm);

// CDFS

/** \brief Calculate cumulative density function for normed gaussian.
 *
 *  \param x                Position for normed CDF.
 */
double norm_cdf(double x);

/** \brief Calculate cumulative density function for gaussian.
 *
 *  \param x                Position for CDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
double gaussian_cdf(double x, double mean, double sigma);

/** \brief Calculate Cauchy cumulative density function.
 *
 *  Calculate cumulative density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Position for Cauchy CDF.
 */
double cauchy_cdf(double x);

/** \brief Calculate cumulative density function for Lorentzian.
 *
 *  \param x                Position for Lorentzian CDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
double lorentzian_cdf(double x, double mean, double fwhm);

// PDF -- RANGES


/** \brief Calculate probability density function for normed gaussian.
 *
 *  \param x                Positions for normed PDF.
 */
template <typename SrcIter, typename DstIter>
size_t norm_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = norm_pdf(*it);
    }
    return std::distance(first, last);
}


/** \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
size_t gaussian_pdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = gaussian_pdf(*it, mean, sigma);
    }
    return std::distance(first, last);
}


/** \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
size_t cauchy_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = cauchy_pdf(*it);
    }
    return std::distance(first, last);
}


/** \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
size_t lorentzian_pdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = lorentzian_pdf(*it, mean, fwhm);
    }
    return std::distance(first, last);
}


// CDF -- RANGES

/** \brief Calculate cumulative density function for normed gaussian.
 *
 *  \param x                Positions for normed CDF.
 */
template <typename SrcIter, typename DstIter>
size_t norm_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = norm_cdf(*it);
    }
    return std::distance(first, last);
}


/** \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
size_t gaussian_cdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = gaussian_cdf(*it, mean, sigma);
    }
    return std::distance(first, last);
}


/** \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
size_t cauchy_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = cauchy_cdf(*it);
    }
    return std::distance(first, last);
}


/** \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
size_t lorentzian_cdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = lorentzian_cdf(*it, mean, fwhm);
    }
    return std::distance(first, last);
}

PYCPP_END_NAMESPACE
