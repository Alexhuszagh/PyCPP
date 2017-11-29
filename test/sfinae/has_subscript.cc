//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator[]()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_subscript.h>
#include <gtest/gtest.h>
#include <tuple>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_subscript, has_subscript)
{
    using vec = std::vector<int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(has_subscript<vec, size_t>::value, "");
    static_assert(has_subscript<vec, int>::value, "");
    static_assert(has_subscript<vec, char>::value, "");
    static_assert(!has_subscript<vec, t1>::value, "");
    static_assert(!has_subscript<vec, t2>::value, "");
    static_assert(!has_subscript<vec, t3>::value, "");
    static_assert(!has_subscript<t1, int>::value, "");
    static_assert(!has_subscript<t2, int>::value, "");
    static_assert(!has_subscript<t3, int>::value, "");
}
