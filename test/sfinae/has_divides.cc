//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator/()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_divides.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_divides, has_divides)
{
    using p1 = pair<int, int>;

    static_assert(has_divides<int>::value, "");
    static_assert(has_divides<int, int>::value, "");
    static_assert(!has_divides<p1>::value, "");
    static_assert(!has_divides<p1, p1>::value, "");
    static_assert(!has_divides<p1, int>::value, "");
    static_assert(!has_divides<int, p1>::value, "");
}
