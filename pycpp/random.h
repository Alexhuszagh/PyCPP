//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Random number generators.
 */

#pragma once

#include <pycpp/config.h>
#include <string>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

typedef int64_t seed_t;
typedef double random_t;
typedef std::vector<random_t> random_list_t;

// FUNCTIONS
// ---------

/**
 *  \brief Seed random number generator.
 */
void seed(seed_t value);

/**
 *  \brief Get random bytes for cryptographic applications.
 */
size_t sysrandom(void* dst, size_t dstlen);

/**
 *  \brief Get random bytes for cryptographic applications.
 */
std::string sysrandom(size_t length);

/**
 *  \brief Get psuedo-random bytes for general purposes.
 *
 *  \param deterministic        Use deterministic seed.
 */
size_t pseudorandom(void* dst, size_t dstlen, bool deterministic = false);

/**
 *  \brief Get psuedo-random bytes for general purposes.
 *
 *  \param deterministic        Use deterministic seed.
 */
std::string pseudorandom(size_t length, bool deterministic = false);

/**
 *  \brief Get value from gammavariate() function
 */
random_t gammavariate(random_t alpha, random_t beta);

/**
 *  \brief Get N-values from gammavariate() function
 */
random_list_t gammavariate(random_t alpha, random_t beta, size_t n);

/**
 *  \brief Get value from lognormvariate() function
 */
random_t lognormvariate(random_t mu, random_t sigma);

/**
 *  \brief Get N-values from lognormvariate() function
 */
random_list_t lognormvariate(random_t mu, random_t sigma, size_t n);

/**
 *  \brief Get value from expovariate() function
 */
random_t expovariate(random_t lambda);

/**
 *  \brief Get N-values from expovariate() function
 */
random_list_t expovariate(random_t lambda, size_t n);

/**
 *  \brief Get value from normalvariate() function
 */
random_t normalvariate(random_t mu, random_t sigma);

/**
 *  \brief Get N-values from normalvariate() function
 */
random_list_t normalvariate(random_t mu, random_t sigma, size_t n);

/**
 *  \brief Get value from weibullvariate() function
 */
random_t weibullvariate(random_t alpha, random_t beta);

/**
 *  \brief Get N-values from weibullvariate() function
 */
random_list_t weibullvariate(random_t alpha, random_t beta, size_t n);

// random.randrange(stop)
// random.randrange(start, stop[, step = 1])
// random.randint(a, b)
// random.shuffle(x[, random])
// random.sample(population, k)
// random.random()
// random.uniform(a, b)
// random.triangular(low, high, mode)
// random.betavariate(alpha, beta)
// random.gauss(mu, sigma)
// random.vonmisesvariate(mu, kappa)
// random.paretovariate(alpha)
// class random.WichmannHill([seed])
// random.whseed([x])

PYCPP_END_NAMESPACE
