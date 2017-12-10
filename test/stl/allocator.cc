//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL allocator alias unittests.
 */

#include <pycpp/stl/allocator.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(allocator, allocator)
{
    using allocator_type = allocator<int>;
    allocator_type alloc;
}
