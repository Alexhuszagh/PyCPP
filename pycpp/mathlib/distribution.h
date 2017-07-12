//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Math
 *  \brief Routines for distribution PDFs and CDFs.
 */

#pragma once

#include <pycpp/config.h>
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
double norm_pdf(const double x);

///** \brief Calculate probability density function for normed gaussian.
// *
// *  \param x                Positions for normed PDF.
// */
//std::vector<double> norm_pdf(const std::vector<double> &x);
//
///** \brief Calculate probability density function for gaussian.
// *
// *  \param x                Position for PDF.
// *  \param mean             Mean of Gaussian
// *  \param fwhm             Standard deviation of Gaussian.
// */
//double gaussian_pdf(const double x,
//    const double mean,
//    const double sigma);
//
///** \brief Calculate probability density function for gaussian.
// *
// *  \param x                Positions for PDF.
// *  \param mean             Mean of Gaussian
// *  \param fwhm             Standard deviation of Gaussian.
// */
//std::vector<double> gaussian_pdf(const std::vector<double> &x,
//    const double mean,
//    const double sigma);
//
///** \brief Calculate Cauchy probability density function.
// *
// *  Calculate probability density function for cauchy-distributed
// *  data. Cauchy is used for the normed distribution, lorentzian is
// *  used for spectral lineshapes.
// *
// *  \param x                Position for Cauchy PDF.
// */
//double cauchy_pdf(const double x);
//
///** \brief Calculate Cauchy probability density function.
// *
// *  Calculate probability density function for cauchy-distributed
// *  data. Cauchy is used for the normed distribution, lorentzian is
// *  used for spectral lineshapes.
// *
// *  \param x                Positions for Cauchy PDF.
// */
//std::vector<double> cauchy_pdf(const std::vector<double> &x);
//
///** \brief Calculate probability density function for Lorentzian.
// *
// *  \param x                Position for Lorentzian PDF.
// *  \param mean             Mean of Lorentzian
// *  \param fwhm             Full-width at half-max of function.
// */
//double lorentzian_pdf(const double x,
//    const double mean,
//    const double fwhm);
//
///** \brief Calculate probability density function for Lorentzian.
// *
// *  \param x                Positions for Lorentzian PDF.
// *  \param mean             Mean of Lorentzian
// *  \param fwhm             Full-width at half-max of function.
// */
//std::vector<double> lorentzian_pdf(const std::vector<double> &x,
//    const double mean,
//    const double fwhm);
//
//// CDFS
//
///** \brief Calculate cumulative density function for normed gaussian.
// *
// *  \param x                Position for normed CDF.
// */
//double norm_cdf(const double x);
//
///** \brief Calculate cumulative density function for normed gaussian.
// *
// *  \param x                Positions for normed CDF.
// */
//std::vector<double> norm_cdf(const std::vector<double> &x);
//
///** \brief Calculate cumulative density function for gaussian.
// *
// *  \param x                Position for CDF.
// *  \param mean             Mean of Gaussian
// *  \param fwhm             Standard deviation of Gaussian.
// */
//double gaussian_cdf(const double x,
//    const double mean,
//    const double sigma);
//
///** \brief Calculate cumulative density function for gaussian.
// *
// *  \param x                Positions for CDF.
// *  \param mean             Mean of Gaussian
// *  \param fwhm             Standard deviation of Gaussian.
// */
//std::vector<double> gaussian_cdf(const std::vector<double> &x,
//    const double mean,
//    const double sigma);
//
///** \brief Calculate Cauchy cumulative density function.
// *
// *  Calculate cumulative density function for cauchy-distributed
// *  data. Cauchy is used for the normed distribution, lorentzian is
// *  used for spectral lineshapes.
// *
// *  \param x                Position for Cauchy CDF.
// */
//double cauchy_cdf(const double x);
//
///** \brief Calculate Cauchy cumulative density function.
// *
// *  Calculate cumulative density function for cauchy-distributed
// *  data. Cauchy is used for the normed distribution, lorentzian is
// *  used for spectral lineshapes.
// *
// *  \param x                Positions for Cauchy CDF.
// */
//std::vector<double> cauchy_cdf(const std::vector<double> &x);
//
///** \brief Calculate cumulative density function for Lorentzian.
// *
// *  \param x                Position for Lorentzian CDF.
// *  \param mean             Mean of Lorentzian
// *  \param fwhm             Full-width at half-max of function.
// */
//double lorentzian_cdf(const double x,
//    const double mean,
//    const double fwhm);
//
///** \brief Calculate cumulative density function for Lorentzian.
// *
// *  \param x                Positions for Lorentzian CDF.
// *  \param mean             Mean of Lorentzian
// *  \param fwhm             Full-width at half-max of function.
// */
//std::vector<double> lorentzian_cdf(const std::vector<double> &x,
//    const double mean,
//    const double fwhm);

PYCPP_END_NAMESPACE
