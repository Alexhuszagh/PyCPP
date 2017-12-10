//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL memory hash unittests.
 */

#include <pycpp/stl/memory.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(unique_ptr, hash)
{
    using unique_ptr = std::unique_ptr<int>;
    using hash_type = hash<unique_ptr>;
    using result_type = decltype(hash_type()(std::declval<unique_ptr>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}


TEST(shared_ptr, hash)
{
    using shared_ptr = std::shared_ptr<int>;
    using hash_type = hash<shared_ptr>;
    using result_type = decltype(hash_type()(std::declval<shared_ptr>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}
