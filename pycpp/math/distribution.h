//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Math
 *  \brief Routines for distribution PDFs and CDFs.
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/misc/xrange.h>
#include <pycpp/preprocessor/parallel.h>
#include <algorithm>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr double CAUCHY_CDF = 3.14159265358979323846;    // M_PI
static constexpr double NORM_PDF = 2.5066282746310002;          // sqrt(2*M_PI);

// FUNCTIONS
// ---------

// PDFS

/**
 *  \brief Calculate probability density function for normed gaussian.
 *
 *  \param x                Position for normed PDF.
 */
double norm_pdf(double x);

/**
 *  \brief Calculate probability density function for gaussian.
 *
 *  \param x                Position for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
double gaussian_pdf(double x, double mean, double sigma);

/**
 *  \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Position for Cauchy PDF.
 */
double cauchy_pdf(double x);

/**
 *  \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Position for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
double lorentzian_pdf(double x, double mean, double fwhm);

// CDFS

/**
 *  \brief Calculate cumulative density function for normed gaussian.
 *
 *  \param x                Position for normed CDF.
 */
double norm_cdf(double x);

/**
 *  \brief Calculate cumulative density function for gaussian.
 *
 *  \param x                Position for CDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
double gaussian_cdf(double x, double mean, double sigma);

/**
 *  \brief Calculate Cauchy cumulative density function.
 *
 *  Calculate cumulative density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Position for Cauchy CDF.
 */
double cauchy_cdf(double x);

/**
 *  \brief Calculate cumulative density function for Lorentzian.
 *
 *  \param x                Position for Lorentzian CDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
double lorentzian_cdf(double x, double mean, double fwhm);

// PDF -- RANGES -- OUTPUT ITERATOR

/**
 *  \brief Calculate probability density function for normed gaussian.
 *
 *  \param x                Positions for normed PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
norm_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = norm_pdf(*it);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
gaussian_pdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = gaussian_pdf(*it, mean, sigma);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
cauchy_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = cauchy_pdf(*it);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
lorentzian_pdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = lorentzian_pdf(*it, mean, fwhm);
    }
    return std::distance(first, last);
}


// CDF -- RANGES -- OUTPUT ITERATOR

/**
 *  \brief Calculate cumulative density function for normed gaussian.
 *
 *  \param x                Positions for normed CDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
norm_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = norm_cdf(*it);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
gaussian_cdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = gaussian_cdf(*it, mean, sigma);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
cauchy_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = cauchy_cdf(*it);
    }
    return std::distance(first, last);
}


/**
 *  \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<!is_random_access_iterator<DstIter>::value, size_t>::type
lorentzian_cdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    for (auto it = first; it != last; ++it) {
        *dst++ = lorentzian_cdf(*it, mean, fwhm);
    }
    return std::distance(first, last);
}

// PDF -- RANGES -- RANDOM ACCESS ITERATOR

/**
 *  \brief Calculate probability density function for normed gaussian.
 *
 *  \param x                Positions for normed PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
norm_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = norm_pdf(first[i]);
    });
    return distance;
}


/**
 *  \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
gaussian_pdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = gaussian_pdf(first[i], mean, sigma);
    });
    return distance;
}


/**
 *  \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
cauchy_pdf(SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = cauchy_pdf(first[i]);
    });
    return distance;
}


/**
 *  \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
lorentzian_pdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = lorentzian_pdf(first[i], mean, fwhm);
    });
    return distance;
}


// CDF -- RANGES -- RANDOM ACCESS ITERATOR

/**
 *  \brief Calculate cumulative density function for normed gaussian.
 *
 *  \param x                Positions for normed CDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
norm_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = norm_cdf(first[i]);
    });
    return distance;
}


/**
 *  \brief Calculate probability density function for gaussian.
 *
 *  \param x                Positions for PDF.
 *  \param mean             Mean of Gaussian
 *  \param fwhm             Standard deviation of Gaussian.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
gaussian_cdf(double mean, double sigma, SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = gaussian_cdf(first[i], mean, sigma);
    });
    return distance;
}


/**
 *  \brief Calculate Cauchy probability density function.
 *
 *  Calculate probability density function for cauchy-distributed
 *  data. Cauchy is used for the normed distribution, lorentzian is
 *  used for spectral lineshapes.
 *
 *  \param x                Positions for Cauchy PDF.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
cauchy_cdf(SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = cauchy_cdf(first[i]);
    });
    return distance;
}


/**
 *  \brief Calculate probability density function for Lorentzian.
 *
 *  \param x                Positions for Lorentzian PDF.
 *  \param mean             Mean of Lorentzian
 *  \param fwhm             Full-width at half-max of function.
 */
template <typename SrcIter, typename DstIter>
typename std::enable_if<is_random_access_iterator<DstIter>::value, size_t>::type
lorentzian_cdf(double mean, double fwhm, SrcIter first, SrcIter last, DstIter dst)
{
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        dst[i] = lorentzian_cdf(first[i], mean, fwhm);
    });
    return distance;
}


// OBJECT-ORIENTED

/**
 *  \brief Norm distribution wrappers.
 */
struct norm
{
    double pdf(double x) const;
    double cdf(double x) const;
    template <typename It1, typename It2> size_t pdf(It1, It1, It2);
    template <typename It1, typename It2> size_t cdf(It1, It1, It2);
};

/**
 *  \brief Gaussian distribution wrappers.
 */
struct gaussian
{
    gaussian(double mean, double sigma);
    double pdf(double x) const;
    double cdf(double x) const;
    template <typename It1, typename It2> size_t pdf(It1, It1, It2);
    template <typename It1, typename It2> size_t cdf(It1, It1, It2);

private:
    double mean;
    double sigma;
};

/**
 *  \brief Cauchy distribution wrappers.
 */
struct cauchy
{
    double pdf(double x) const;
    double cdf(double x) const;
    template <typename It1, typename It2> size_t pdf(It1, It1, It2);
    template <typename It1, typename It2> size_t cdf(It1, It1, It2);
};

/**
 *  \brief Lorentzian distribution wrappers.
 */
struct lorentzian
{
    lorentzian(double mean, double fwhm);
    double pdf(double x) const;
    double cdf(double x) const;
    template <typename It1, typename It2> size_t pdf(It1, It1, It2);
    template <typename It1, typename It2> size_t cdf(It1, It1, It2);

private:
    double mean;
    double fwhm;
};


// IMPLEMENTATION
// --------------


template <typename It1, typename It2>
size_t norm::pdf(It1 first, It1 last, It2 dst)
{
    return norm_cdf(first, last, dst);
}


template <typename It1, typename It2>
size_t norm::cdf(It1 first, It1 last, It2 dst)
{
    return norm_cdf(first, last, dst);
}


template <typename It1, typename It2>
size_t gaussian::pdf(It1 first, It1 last, It2 dst)
{
    return gaussian_cdf(mean, sigma, first, last, dst);
}


template <typename It1, typename It2>
size_t gaussian::cdf(It1 first, It1 last, It2 dst)
{
    return gaussian_cdf(mean, sigma, first, last, dst);
}


template <typename It1, typename It2>
size_t cauchy::pdf(It1 first, It1 last, It2 dst)
{
    return cauchy_cdf(first, last, dst);
}


template <typename It1, typename It2>
size_t cauchy::cdf(It1 first, It1 last, It2 dst)
{
    return cauchy_cdf(first, last, dst);
}


template <typename It1, typename It2>
size_t lorentzian::pdf(It1 first, It1 last, It2 dst)
{
    return lorentzian_cdf(mean, fwhm, first, last, dst);
}


template <typename It1, typename It2>
size_t lorentzian::cdf(It1 first, It1 last, It2 dst)
{
    return lorentzian_cdf(mean, fwhm, first, last, dst);
}

PYCPP_END_NAMESPACE
