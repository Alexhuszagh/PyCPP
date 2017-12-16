//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL bitset hash unittests.
 */

#include <pycpp/stl/bitset.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(bitset, hash)
{
    using bitset_type = bitset<10>;
    using hash_type = hash<bitset_type>;
    using result_type = decltype(hash_type()(declval<bitset_type>()));
    static_assert(is_same<result_type, size_t>::value, "");
}
