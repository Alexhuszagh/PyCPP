//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator+()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_plus.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_plus, has_plus)
{
    using p1 = pair<int, int>;

    static_assert(has_plus<int>::value, "");
    static_assert(has_plus<int, int>::value, "");
    static_assert(!has_plus<p1>::value, "");
    static_assert(!has_plus<p1, p1>::value, "");
    static_assert(!has_plus<p1, int>::value, "");
    static_assert(!has_plus<int, p1>::value, "");
}
