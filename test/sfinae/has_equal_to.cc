//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator==()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_equal_to.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_equal_to, has_equal_to)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(has_equal_to<pair>::value, "");
    static_assert(has_equal_to<t1>::value, "");
    static_assert(has_equal_to<t2>::value, "");
    static_assert(has_equal_to<t3>::value, "");
    static_assert(has_equal_to<int>::value, "");
    static_assert(has_equal_to<char>::value, "");
    static_assert(!has_equal_to<pair, t1>::value, "");
    static_assert(!has_equal_to<pair, t2>::value, "");
    static_assert(!has_equal_to<pair, t3>::value, "");
    static_assert(!has_equal_to<pair, int>::value, "");
    static_assert(!has_equal_to<pair, char>::value, "");
}
