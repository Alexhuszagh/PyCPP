//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator<()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_less.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_less, has_less)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(has_less<pair>::value, "");
    static_assert(has_less<t1>::value, "");
    static_assert(has_less<t2>::value, "");
    static_assert(has_less<t3>::value, "");
    static_assert(has_less<int>::value, "");
    static_assert(has_less<char>::value, "");
    static_assert(!has_less<pair, t1>::value, "");
    static_assert(!has_less<pair, t2>::value, "");
    static_assert(!has_less<pair, t3>::value, "");
    static_assert(!has_less<pair, int>::value, "");
    static_assert(!has_less<pair, char>::value, "");
}
