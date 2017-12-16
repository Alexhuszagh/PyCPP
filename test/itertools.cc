//  :copyright: (c) Howard Hinnant 2005-2011.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost,see licenses/boost.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Permutation, combination and product unittests.
 */

#include <pycpp/itertools.h>
#include <pycpp/stl/map.h>
#include <pycpp/stl/numeric.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE
using namespace prod_detail;

// DATA
// ----

static std::vector<int> W = {1380, 1520, 1560, 2140};
static std::vector<int> H = {1710, 1710, 1820, 1820, 1820, 1880, 1880};
typedef typename std::vector<int>::iterator It;

// TESTS
// -----


TEST(itertools, for_each_permutation)
{
    // P(4, 2)
    int c = 0;
    int sum = 0;
    for_each_permutation(W.begin(), W.begin()+2, W.end(), [&](It first, It last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 12);
    EXPECT_EQ(sum, 39600);

    // P(4, 3)
    c = 0;
    sum = 0;
    for_each_permutation(W.begin(), W.begin()+3, W.end(), [&](It first, It last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 24);
    EXPECT_EQ(sum, 118800);
}


TEST(itertools, count_each_permutation)
{
    EXPECT_EQ(count_each_permutation(2, 4), 30);
    EXPECT_EQ(count_each_permutation(3, 4), 210);
}


TEST(itertools, for_each_combination)
{
    // C(4, 2)
    int c = 0;
    int sum = 0;
    for_each_combination(W.begin(), W.begin()+2, W.end(), [&](It first, It last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 6);
    EXPECT_EQ(sum, 19800);

    // C(4, 3)
    c = 0;
    sum = 0;
    for_each_combination(W.begin(), W.begin()+3, W.end(), [&](It first, It last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 4);
    EXPECT_EQ(sum, 19800);
}


TEST(itertools, count_each_combination)
{
    EXPECT_EQ(count_each_combination(2, 4), 15);
    EXPECT_EQ(count_each_combination(3, 4), 35);
}


TEST(itertools, product)
{
    typedef std::vector<std::vector<int>> ll;
    typedef std::vector<std::map<int, int>> lm;
    typedef std::map<int, std::vector<int>> ml;
    typedef std::map<int, std::map<int, int>> mm;

    static_assert(is_list_list<ll>::value, "Cannot detect list of lists");
    static_assert(is_list_map<lm>::value, "Cannot detect list of maps");
    static_assert(is_map_list<ml>::value, "Cannot detect map of lists");
    static_assert(is_map_map<mm>::value, "Cannot detect map of maps");
}


TEST(itertools, cartesian_product)
{
    int c = 0;
    int sum = 0;
    typedef std::reference_wrapper<const int> intw;

    // empty
    std::vector<std::vector<int>> v;
    product(v, [&](intw* first, intw* last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 0);
    EXPECT_EQ(sum, 0);

    // 1x3
    c = 0;
    sum = 0;
    v = {{1, 2, 3}};
    product(v, [&](intw* first, intw* last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 3);
    EXPECT_EQ(sum, 6);

    // 2x3
    c = 0;
    sum = 0;
    v = {{1, 2, 3}, {4, 5, 6}};
    product(v, [&](intw* first, intw* last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 9);
    EXPECT_EQ(sum, 63);

    // 3x3
    c = 0;
    sum = 0;
    v = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    product(v, [&](intw* first, intw* last) {
        ++c;
        sum = std::accumulate(first, last, sum);
        return false;
    });
    EXPECT_EQ(c, 27);
    EXPECT_EQ(sum, 405);
}
