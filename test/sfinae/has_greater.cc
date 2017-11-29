//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator>()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_greater.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_greater, has_greater)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(has_greater<pair>::value, "");
    static_assert(has_greater<t1>::value, "");
    static_assert(has_greater<t2>::value, "");
    static_assert(has_greater<t3>::value, "");
    static_assert(has_greater<int>::value, "");
    static_assert(has_greater<char>::value, "");
    static_assert(!has_greater<pair, t1>::value, "");
    static_assert(!has_greater<pair, t2>::value, "");
    static_assert(!has_greater<pair, t3>::value, "");
    static_assert(!has_greater<pair, int>::value, "");
    static_assert(!has_greater<pair, char>::value, "");
}
