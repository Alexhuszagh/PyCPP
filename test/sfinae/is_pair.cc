//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `pair` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_pair.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_pair, is_pair)
{
    using p1 = pair<int, int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(is_pair<p1>::value, "");
    static_assert(!is_pair<t1>::value, "");
    static_assert(!is_pair<t2>::value, "");
    static_assert(!is_pair<t3>::value, "");
    static_assert(!is_pair<int>::value, "");
    static_assert(!is_pair<char>::value, "");
}
