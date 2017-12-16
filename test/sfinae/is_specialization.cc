//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `is_specialization` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_specialization.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_specialization, is_specialization)
{
    using p1 = pair<int, int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(is_specialization<p1, pair>::value, "");
    static_assert(is_specialization<t1, tuple>::value, "");
    static_assert(is_specialization<t2, tuple>::value, "");
    static_assert(is_specialization<t3, tuple>::value, "");
    static_assert(!is_specialization<p1, tuple>::value, "");
    static_assert(!is_specialization<t1, pair>::value, "");
    static_assert(!is_specialization<t2, pair>::value, "");
    static_assert(!is_specialization<t3, pair>::value, "");
}
