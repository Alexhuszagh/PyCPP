//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator%()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_modulus.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_modulus, has_modulus)
{
    using p1 = pair<int, int>;

    static_assert(has_modulus<int>::value, "");
    static_assert(has_modulus<int, int>::value, "");
    static_assert(!has_modulus<p1>::value, "");
    static_assert(!has_modulus<p1, p1>::value, "");
    static_assert(!has_modulus<p1, int>::value, "");
    static_assert(!has_modulus<int, p1>::value, "");
}
