//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Random unittests.
 */

#include <pycpp/random.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// HELPERS
// -------

template <typename N>
static bool check_range(const N& value, const N& min, const N& max)
{
    return value >= min && value <= max;
}


template <typename N>
static bool check_range(const std::vector<N>& value, const N& min, const N& max)
{
    return std::all_of(value.begin(), value.end(), [&](N n) {
        return check_range(n, min, max);
    });
}

// TESTS
// -----


TEST(random, sysrandom)
{
    // cryptographic random, should be completely unique
    auto reference = sysrandom(8);
    for (int i = 0; i < 200; ++i) {
        EXPECT_NE(sysrandom(8), reference);
    }
}


TEST(random, pseudorandom)
{
    // all are seeded the same, should produce same output
    auto reference = pseudorandom(8, true);
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(pseudorandom(8, true), reference);
    }
}


TEST(random, seed)
{
    seed(0);
    seed(SIZE_MAX);
}


TEST(random, gammavariate)
{
    EXPECT_TRUE(check_range<random_t>(gammavariate(1.0, 1.0), 0., 15.));
    EXPECT_TRUE(check_range<random_t>(gammavariate(1.0, 1.0, 5), 0., 15.));
}


TEST(random, lognormvariate)
{
    EXPECT_TRUE(check_range<random_t>(lognormvariate(1.0, 1.0), 0., 300.));
    EXPECT_TRUE(check_range<random_t>(lognormvariate(1.0, 1.0, 5), 0., 300.));
}


TEST(random, expovariate)
{
    EXPECT_TRUE(check_range<random_t>(expovariate(1.0), 0., 20.));
    EXPECT_TRUE(check_range<random_t>(expovariate(1.0, 5), 0., 20.));
}


TEST(random, normalvariate)
{
    EXPECT_TRUE(check_range<random_t>(normalvariate(1.0, 1.0), 0., 8.));
    EXPECT_TRUE(check_range<random_t>(normalvariate(1.0, 1.0, 5), 0., 8.));
}


TEST(random, weibullvariate)
{
    EXPECT_TRUE(check_range<random_t>(weibullvariate(1.0, 1.0), 0., 12.));
    EXPECT_TRUE(check_range<random_t>(weibullvariate(1.0, 1.0, 5), 0., 12.));
}


TEST(random, randrange)
{
    EXPECT_TRUE(check_range<random_int_t>(randrange(5, 10, 1), 5, 10));
    EXPECT_TRUE(check_range<random_int_t>(randrange(5, 10, 1, 100), 5, 10));
}


TEST(random, randint)
{
    EXPECT_TRUE(check_range<random_int_t>(randint(5, 10), 5, 10));
    EXPECT_TRUE(check_range<random_int_t>(randint(5, 10, 100), 5, 10));
}


TEST(random, randnum)
{
    EXPECT_TRUE(check_range<random_t>(randnum(), 0, 1));
    EXPECT_TRUE(check_range<random_t>(randnum(100), 0, 1));
}


TEST(random, uniform)
{
    EXPECT_TRUE(check_range<random_t>(uniform(0, 5), 0, 5));
    EXPECT_TRUE(check_range<random_t>(uniform(0, 5, 100), 0, 5));
}


TEST(random, triangular)
{
//    EXPECT_TRUE(check_range<random_t>(triangular(0, 5), 0, 5));
//    EXPECT_TRUE(check_range<random_t>(triangular(0, 5, 100), 0, 5));
}


TEST(random, betavariate)
{
//    EXPECT_TRUE(check_range<random_t>(betavariate(0, 5), 0, 5));
//    EXPECT_TRUE(check_range<random_t>(betavariate(0, 5, 100), 0, 5));
}


TEST(random, gauss)
{
//    EXPECT_TRUE(check_range<random_t>(triangular(0, 5), 0, 5));
//    EXPECT_TRUE(check_range<random_t>(triangular(0, 5, 100), 0, 5));
}


TEST(random, paretovariate)
{
//    EXPECT_TRUE(check_range<random_t>(paretovariate(0, 5), 0, 5));
//    EXPECT_TRUE(check_range<random_t>(paretovariate(0, 5, 100), 0, 5));
}


TEST(random, choice)
{
    auto list = uniform(0, 1, 500);
    for (size_t i = 0; i < 1000; ++i) {
        EXPECT_TRUE(choice(list.begin(), list.end()) >= 0);
    }
}


TEST(random, sample)
{
// TODO: bug fix due to the reference... shit
    auto list = uniform(0, 1, 500);
    auto total = std::accumulate(list.begin(), list.end(), 0);
    for (size_t i = 0; i < 499; ++i) {
        auto value = sample(list.begin(), list.end(), i);
        auto x = value.begin();
        //EXPECT_TRUE(std::accumulate(value.begin(), value.end(), 0) < total);
    }
}
