//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL deque alias unittests.
 */

#include <pycpp/stl/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(deque, deque)
{
    using deque_type = deque<int>;
    deque_type deq;
}
