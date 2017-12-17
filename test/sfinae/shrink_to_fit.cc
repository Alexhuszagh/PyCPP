//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `shrink_to_fit` and `has_shrink_to_fit` unittests.
 */

#include <pycpp/sfinae/shrink_to_fit.h>
#include <pycpp/stl/list.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(shrink_to_fit, shrink_to_fit)
{
    list<int> l;
    vector<int> v;

    shrink_to_fit()(l);
    shrink_to_fit()(v);
}


TEST(shrink_to_fit, has_shrink_to_fit)
{
    using list_type = list<int>;
    using vector_type = vector<int>;

    static_assert(!has_shrink_to_fit<list_type>::value, "");
    static_assert(has_shrink_to_fit<vector_type>::value, "");
}
