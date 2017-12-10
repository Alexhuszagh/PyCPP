//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL list alias unittests.
 */

#include <pycpp/stl/list.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(list, list)
{
    using list_type = list<int>;
    list_type lst;
}
