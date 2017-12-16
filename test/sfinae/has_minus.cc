//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator-()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_minus.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_minus, has_minus)
{
    using p1 = pair<int, int>;

    static_assert(has_minus<int>::value, "");
    static_assert(has_minus<int, int>::value, "");
    static_assert(!has_minus<p1>::value, "");
    static_assert(!has_minus<p1, p1>::value, "");
    static_assert(!has_minus<p1, int>::value, "");
    static_assert(!has_minus<int, p1>::value, "");
}
