//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL hash alias unittests.
 */

#include <pycpp/stl/hash.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(hash, hash)
{
    using hash_type = hash<int>;
    hash_type hsh;
}
