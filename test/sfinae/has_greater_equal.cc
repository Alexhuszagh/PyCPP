//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator>=()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_greater_equal.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_greater_equal, has_greater_equal)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(has_greater_equal<pair>::value, "");
    static_assert(has_greater_equal<t1>::value, "");
    static_assert(has_greater_equal<t2>::value, "");
    static_assert(has_greater_equal<t3>::value, "");
    static_assert(has_greater_equal<int>::value, "");
    static_assert(has_greater_equal<char>::value, "");
    static_assert(!has_greater_equal<pair, t1>::value, "");
    static_assert(!has_greater_equal<pair, t2>::value, "");
    static_assert(!has_greater_equal<pair, t3>::value, "");
    static_assert(!has_greater_equal<pair, int>::value, "");
    static_assert(!has_greater_equal<pair, char>::value, "");
}
