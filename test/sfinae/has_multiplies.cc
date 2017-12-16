//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator*()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_multiplies.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_multiplies, has_multiplies)
{
    using p1 = pair<int, int>;

    static_assert(has_multiplies<int>::value, "");
    static_assert(has_multiplies<int, int>::value, "");
    static_assert(!has_multiplies<p1>::value, "");
    static_assert(!has_multiplies<p1, p1>::value, "");
    static_assert(!has_multiplies<p1, int>::value, "");
    static_assert(!has_multiplies<int, p1>::value, "");
}
