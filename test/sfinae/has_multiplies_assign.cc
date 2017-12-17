//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator*=()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_multiplies_assign.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_multiplies_assign, has_multiplies_assign)
{
    using p1 = pair<int, int>;

    static_assert(has_multiplies_assign<int>::value, "");
    static_assert(has_multiplies_assign<int, int>::value, "");
    static_assert(!has_multiplies_assign<p1>::value, "");
    static_assert(!has_multiplies_assign<p1, p1>::value, "");
    static_assert(!has_multiplies_assign<p1, int>::value, "");
    static_assert(!has_multiplies_assign<int, p1>::value, "");
}
