//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <optional.h>
#include <random.h>
#include <tls.h>
#include <chrono>
#include <cstring>
#include <ctime>
#include <memory>
#include <random>

// ALIAS
// -----

typedef std::chrono::high_resolution_clock highres_clock_t;
typedef std::mt19937_64 mersene_twister;
typedef std::default_random_engine default_random;

template <typename T>
using memory_type = typename std::aligned_storage<sizeof(T)>::type;

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
    auto& generator = reinterpret_cast<default_random&>(DEFAULT_RANDOM);
    if (!DEFAULT_RANDOM_INIT) {
        DEFAULT_RANDOM_INIT = true;
        generator = default_random();
        generator.seed(GLOBAL_SEED);
    }

    if (SEED != GLOBAL_SEED) {
        SEED = GLOBAL_SEED;
        generator.seed(SEED);
    }

    return generator;
}


template <typename Distribution>
static random_t random_value(Distribution& distribution)
{
    return distribution(get_default_random());
}


template <typename Distribution>
static random_list_t random_list(Distribution& distribution, size_t n)
{
    auto &generator = get_default_random();

    random_list_t list;
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
    size_t counter = dstlen / sizeof(uint32_t);
    while (counter--) {
        uint32_t value = generator();
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
    std::gamma_distribution<random_t> distribution(alpha, beta);
    return random_value(distribution);
}


random_list_t gammavariate(random_t alpha, random_t beta, size_t n)
{
    std::gamma_distribution<random_t> distribution(alpha, beta);
    return random_list(distribution, n);
}


random_t lognormvariate(random_t mu, random_t sigma)
{
    std::lognormal_distribution<random_t> distribution(mu, sigma);
    return random_value(distribution);
}


random_list_t lognormvariate(random_t mu, random_t sigma, size_t n)
{
    std::lognormal_distribution<random_t> distribution(mu, sigma);
    return random_list(distribution, n);
}


random_t expovariate(random_t lambda)
{
    std::exponential_distribution<random_t> distribution(lambda);
    return random_value(distribution);
}


random_list_t expovariate(random_t lambda, size_t n)
{
    std::exponential_distribution<random_t> distribution(lambda);
    return random_list(distribution, n);
}


random_t normalvariate(random_t mu, random_t sigma)
{
    std::normal_distribution<random_t> distribution(mu, sigma);
    return random_value(distribution);
}


random_list_t normalvariate(random_t mu, random_t sigma, size_t n)
{
    std::normal_distribution<random_t> distribution(mu, sigma);
    return random_list(distribution, n);
}


random_t weibullvariate(random_t alpha, random_t beta)
{
    std::weibull_distribution<random_t> distribution(alpha, beta);
    return random_value(distribution);
}


random_list_t weibullvariate(random_t alpha, random_t beta, size_t n)
{
    std::weibull_distribution<random_t> distribution(alpha, beta);
    return random_list(distribution, n);
}
