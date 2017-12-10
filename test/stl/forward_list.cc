//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL forward_list alias unittests.
 */

#include <pycpp/stl/forward_list.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(forward_list, forward_list)
{
    using forward_list_type = forward_list<int>;
    forward_list_type fwd;
}
