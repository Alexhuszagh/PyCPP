//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator>>()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_right_shift.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_right_shift, has_right_shift)
{
    using p1 = pair<int, int>;

    static_assert(has_right_shift<int>::value, "");
    static_assert(has_right_shift<int, int>::value, "");
    static_assert(!has_right_shift<p1>::value, "");
    static_assert(!has_right_shift<p1, p1>::value, "");
    static_assert(!has_right_shift<p1, int>::value, "");
    static_assert(!has_right_shift<int, p1>::value, "");
}
