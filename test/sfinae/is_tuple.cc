//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `tuple` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_tuple.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_tuple, is_tuple)
{
    using p1 = pair<int, int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(!is_tuple<p1>::value, "");
    static_assert(is_tuple<t1>::value, "");
    static_assert(is_tuple<t2>::value, "");
    static_assert(is_tuple<t3>::value, "");
    static_assert(!is_tuple<int>::value, "");
    static_assert(!is_tuple<char>::value, "");
}
