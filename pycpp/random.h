//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Random number generators.
 */

#pragma once

#include <pycpp/misc/xrange.h>
#include <pycpp/reference/vector.h>
#include <array>
#include <string>
#include <type_traits>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

typedef int64_t seed_t;
typedef double random_t;
typedef uint64_t random_int_t;
typedef std::vector<random_t> random_list_t;
typedef std::vector<random_int_t> random_int_list_t;

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
 *  \warning Due to how STL strings allocate memory, this function
 *  cannot be cryptographically secure. Use only for non-cryptographic
 *  applications.
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
 *  \brief Get value from gammavariate() function.
 */
random_t gammavariate(random_t alpha, random_t beta);

/**
 *  \brief Get N-values from gammavariate() function.
 */
random_list_t gammavariate(random_t alpha, random_t beta, size_t n);

/**
 *  \brief Get value from lognormvariate() function.
 */
random_t lognormvariate(random_t mu, random_t sigma);

/**
 *  \brief Get N-values from lognormvariate() function.
 */
random_list_t lognormvariate(random_t mu, random_t sigma, size_t n);

/**
 *  \brief Get value from expovariate() function.
 */
random_t expovariate(random_t lambda);

/**
 *  \brief Get N-values from expovariate() function.
 */
random_list_t expovariate(random_t lambda, size_t n);

/**
 *  \brief Get value from normalvariate() function.
 */
random_t normalvariate(random_t mu, random_t sigma);

/**
 *  \brief Get N-values from normalvariate() function.
 */
random_list_t normalvariate(random_t mu, random_t sigma, size_t n);

/**
 *  \brief Get value from weibullvariate() function.
 */
random_t weibullvariate(random_t alpha, random_t beta);

/**
 *  \brief Get N-values from weibullvariate() function.
 */
random_list_t weibullvariate(random_t alpha, random_t beta, size_t n);

/**
 *  \brief Get random value from start to stop.
 */
random_int_t randrange(random_int_t start, random_int_t stop, size_t step);

/**
 *  \brief Get random N-values from start to stop.
 */
random_int_list_t randrange(random_int_t start, random_int_t stop, size_t step, size_t n);

/**
 *  \brief Get random value so that a <= N <= b.
 */
random_int_t randint(random_int_t a, random_int_t b);

/**
 *  \brief Get random N-values so that a <= N <= b.
 */
random_int_list_t randint(random_int_t a, random_int_t b, size_t n);

/**
 *  \brief Get random in range [0, 1).
 *
 *  Identical to Python's `random.random()`, just renamed to avoid
 *  compatibility issues with BSD random.
 */
random_t randnum();

/**
 *  \brief Get random N-values so that [0, 1).
 */
random_list_t randnum(size_t n);

/**
 *  \brief Get random in range [a, b).
 */
random_t uniform(random_t a, random_t b);

/**
 *  \brief Get random N-values so that [a, b).
 */
random_list_t uniform(random_t a, random_t b, size_t n);

/**
 *  \brief Get triangular distribution in range [low, high).
 */
random_t triangular(random_t low, random_t high, random_t mode);

/**
 *  \brief Get triangular distribution for N-values so that [low, high).
 */
random_list_t triangular(random_t low, random_t high, random_t mode, size_t n);

/**
 *  \brief Get beta distribution in range.
 */
random_t betavariate(random_t alpha, random_t beta);

/**
 *  \brief Get beta distribution for N-values.
 */
random_list_t betavariate(random_t alpha, random_t beta, size_t n);

/**
 *  \brief Alias for normvariate.
 */
random_t gauss(random_t mu, random_t sigma);

/**
 *  \brief Alias for normvariate.
 */
random_list_t gauss(random_t mu, random_t sigma, size_t n);

/**
 *  \brief Get Pareto distribution in range.
 */
random_t paretovariate(random_t alpha);

/**
 *  \brief Get Pareto distribution for N-values.
 */
random_list_t paretovariate(random_t alpha, size_t n);

/**
 *  \brief Select random value from range.
 *
 *  Iter must a RandomAccessIterator.
 */
template <typename Iter>
auto choice(Iter first, Iter last) -> decltype(*std::declval<Iter>())
{
    if (first == last) {
        throw std::runtime_error("choice() on empty range.");
    }
    return first[static_cast<size_t>(randrange(0, std::distance(first, last)-1, 1))];
}


/**
 *  \brief Sample k elements from sequence.
 *
 *  Use a Fisher-Yates shuffle with the sorted index sequence
 *  of the iterator range. Iter must a RandomAccessIterator.
 */
template <typename Iter>
auto sample(Iter first, Iter last, size_t k)
    -> reference_vector<typename std::iterator_traits<Iter>::value_type>
{
    // parameters
    std::ptrdiff_t distance = std::distance(first, last);

    // sanity check
    if (static_cast<std::ptrdiff_t>(k) > distance) {
        throw std::runtime_error("Cannot sample k elements from range size N if k > N.");
    }

    // partial fisher yates shuffling on the indexes
    auto r = xrange<size_t>(0, distance, 1);
    std::vector<size_t> index(r.begin(), r.end());
    for (size_t i = k; i >= 1; --i) {
        size_t j = static_cast<size_t>(randint(0, distance-1));
        std::swap(index[i-1], index[j]);
    }

    // fill vector
    reference_vector<typename std::iterator_traits<Iter>::value_type> vector;
    vector.reserve(k);
    for (size_t i = 0; i < k; ++i) {
        size_t j = index[i];
        vector.push_back(first[j]);
    }

    return vector;
}


/**
 *  \brief Shuffle items in range.
 *
 *  Use a Fisher-Yates method to shuffle elements. Iter must a
 *  RandomAccessIterator.
 */
template <typename Iter>
void shuffle(Iter first, Iter last)
{
    size_t distance = static_cast<size_t>(std::distance(first, last));
    for (size_t i = distance; i >= 1; --i) {
        size_t j = static_cast<size_t>(randint(0, distance-1));
        std::swap(first[i-1], first[j]);
    }
}

PYCPP_END_NAMESPACE
