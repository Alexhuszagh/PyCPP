//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/random.h>
#include <pycpp/preprocessor/tls.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/chrono.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/random.h>
#include <pycpp/stl/optional.h>
#include <math.h>
#include <string.h>
#include <time.h>

PYCPP_BEGIN_NAMESPACE
\
// ALIAS
// -----

using highres_clock_t = chrono::high_resolution_clock;
using mersene_twister = mt19937_64;
using default_random = default_random_engine;

template <typename T>
using memory_type = aligned_storage_t<sizeof(T)>;

// GLOBALS
// -------

seed_t GLOBAL_SEED = 0;
thread_local_storage seed_t SEED = 0;
thread_local_storage bool MERSENE_TWISTER_INIT = false;
thread_local_storage memory_type<mersene_twister> MERSENE_TWISTER;
thread_local_storage bool DEFAULT_RANDOM_INIT = false;
thread_local_storage memory_type<default_random> DEFAULT_RANDOM;

// HELPERS
// -------

static mersene_twister& get_mersene_twister()
{
    auto& generator = reinterpret_cast<mersene_twister&>(MERSENE_TWISTER);
    if (!MERSENE_TWISTER_INIT) {
        MERSENE_TWISTER_INIT = true;
        generator = mersene_twister();
        generator.seed(GLOBAL_SEED);
    }

    if (SEED != GLOBAL_SEED) {
        SEED = GLOBAL_SEED;
        generator.seed(SEED);
    }

    return generator;
}


static default_random& get_default_random()
{
    using uint = typename default_random::result_type;

    auto& generator = reinterpret_cast<default_random&>(DEFAULT_RANDOM);
    if (!DEFAULT_RANDOM_INIT) {
        DEFAULT_RANDOM_INIT = true;
        generator = default_random();
        generator.seed(static_cast<uint>(GLOBAL_SEED));
    }

    if (SEED != GLOBAL_SEED) {
        SEED = GLOBAL_SEED;
        generator.seed(static_cast<uint>(SEED));
    }

    return generator;
}


template <typename Distribution>
static auto random_value(Distribution& distribution) -> typename Distribution::result_type
{
    return distribution(get_default_random());
}


template <typename Distribution>
static auto random_list(Distribution& distribution, size_t n) -> vector<typename Distribution::result_type>
{
    auto &generator = get_default_random();

    vector<typename Distribution::result_type> list;
    list.reserve(n);
    while (n--) {
        list.emplace_back(distribution(generator));
    }

    return list;
}


// FUNCTIONS
// ---------


void seed(seed_t value)
{
    GLOBAL_SEED = value;
}


size_t pseudorandom(void* dst, size_t dstlen, bool deterministic)
{
    auto& generator = get_mersene_twister();
    if (deterministic) {
        generator.seed(time(nullptr));
    }

    char* buffer = reinterpret_cast<char*>(dst);
    size_t counter = dstlen / sizeof(uint_fast64_t);
    while (counter--) {
        uint_fast64_t value = generator();
        memmove(buffer, &value, sizeof(value));
        buffer += sizeof(value);
    }

    if (deterministic) {
        generator.seed(SEED);
    }

    return dstlen;
}


std::string pseudorandom(size_t length, bool deterministic)
{
    char* dst = new char[length];
    pseudorandom(dst, length, deterministic);
    std::string output(dst, length);
    delete[] dst;
    return output;
}


random_t gammavariate(random_t alpha, random_t beta)
{
    gamma_distribution<random_t> distribution(alpha, beta);
    return random_value(distribution);
}


random_list_t gammavariate(random_t alpha, random_t beta, size_t n)
{
    gamma_distribution<random_t> distribution(alpha, beta);
    return random_list(distribution, n);
}


random_t lognormvariate(random_t mu, random_t sigma)
{
    lognormal_distribution<random_t> distribution(mu, sigma);
    return random_value(distribution);
}


random_list_t lognormvariate(random_t mu, random_t sigma, size_t n)
{
    lognormal_distribution<random_t> distribution(mu, sigma);
    return random_list(distribution, n);
}


random_t expovariate(random_t lambda)
{
    exponential_distribution<random_t> distribution(lambda);
    return random_value(distribution);
}


random_list_t expovariate(random_t lambda, size_t n)
{
    exponential_distribution<random_t> distribution(lambda);
    return random_list(distribution, n);
}


random_t normalvariate(random_t mu, random_t sigma)
{
    normal_distribution<random_t> distribution(mu, sigma);
    return random_value(distribution);
}


random_list_t normalvariate(random_t mu, random_t sigma, size_t n)
{
    normal_distribution<random_t> distribution(mu, sigma);
    return random_list(distribution, n);
}


random_t weibullvariate(random_t alpha, random_t beta)
{
    weibull_distribution<random_t> distribution(alpha, beta);
    return random_value(distribution);
}


random_list_t weibullvariate(random_t alpha, random_t beta, size_t n)
{
    weibull_distribution<random_t> distribution(alpha, beta);
    return random_list(distribution, n);
}


random_int_t randrange(random_int_t start, random_int_t stop, size_t step)
{
    if (stop < start) {
        throw runtime_error("Cannot check negative range.");
    }

    auto distance = (stop - start) / step;
    uniform_int_distribution<random_int_t> distribution(0, distance);
    return (random_value(distribution) * step) + start;
}


random_int_list_t randrange(random_int_t start, random_int_t stop, size_t step, size_t n)
{
    if (stop < start) {
        throw runtime_error("Cannot check negative range.");
    }

    auto distance = (stop - start) / step;
    uniform_int_distribution<random_int_t> distribution(0, distance);
    auto list = random_list(distribution, n);
    for_each(list.begin(), list.end(), [&start, &step](random_int_t& r) {
        r *= step;
        r += start;
    });

    return list;
}


random_int_t randint(random_int_t a, random_int_t b)
{
    return randrange(a, b, 1);
}

random_int_list_t randint(random_int_t a, random_int_t b, size_t n)
{
    return randrange(a, b, 1, n);
}


random_t randnum()
{
    return uniform(0, 1);
}

random_list_t randnum(size_t n)
{
    return uniform(0, 1, n);
}


random_t uniform(random_t a, random_t b)
{
    uniform_real_distribution<random_t> distribution(a, b);
    return random_value(distribution);
}

random_list_t uniform(random_t a, random_t b, size_t n)
{
    uniform_real_distribution<random_t> distribution(a, b);
    return random_list(distribution, n);
}


static random_t triangular_impl(random_t low, random_t high, random_t c, random_t value)
{
    if (value > c) {
        return triangular_impl(high, low, 1-c, 1-value);
    }

    return low + (high - low) * pow(value * c, 0.5);
}


random_t triangular(random_t low, random_t high, random_t mode)
{
    auto c = (mode - low) / (high - low);
    return triangular_impl(low, high, c, randnum());
}


random_list_t triangular(random_t low, random_t high, random_t mode, size_t n)
{
    auto c = (mode - low) / (high - low);
    auto list = randnum(n);
    for (auto &item: list) {
        item = triangular_impl(low, high, c, item);
    }

    return list;
}


static random_t betavariate_impl(random_t a, random_t b)
{
    return a == 0 ? 0 : a / (a + b);
}


random_t betavariate(random_t alpha, random_t beta)
{
    return betavariate_impl(gammavariate(alpha, 1.0), gammavariate(beta, 1.0));
}


random_list_t betavariate(random_t alpha, random_t beta, size_t n)
{
    auto l1 = gammavariate(alpha, 1.0, n);
    auto l2 = gammavariate(beta, 1.0, n);
    for (size_t i = 0; i < n; ++i) {
        l1[i] = betavariate_impl(l1[i], l2[i]);
    }

    return l1;
}


random_t gauss(random_t mu, random_t sigma)
{
    return normalvariate(mu, sigma);
}


random_list_t gauss(random_t mu, random_t sigma, size_t n)
{
    return normalvariate(mu, sigma, n);
}


random_t paretovariate_impl(random_t value, random_t alpha)
{
    return 1 / pow(value, (1 / alpha));
}


random_t paretovariate(random_t alpha)
{
    return paretovariate_impl(1 - randnum(), alpha);
}


random_list_t paretovariate(random_t alpha, size_t n)
{
    auto list = randnum(n);
    for (auto &item: list) {
        item = paretovariate_impl(1 - item, alpha);
    }

    return list;
}

PYCPP_END_NAMESPACE
