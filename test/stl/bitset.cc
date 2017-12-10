//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL bitset hash unittests.
 */

#include <pycpp/stl/bitset.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(bitset, hash)
{
    using bitset = std::bitset<10>;
    using hash_type = hash<bitset>;
    using result_type = decltype(hash_type()(std::declval<bitset>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}
