//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL memory hash unittests.
 */

#include <pycpp/stl/memory.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(allocator, allocator)
{
    using allocator_type = allocator<int>;
    allocator_type alloc;
}


TEST(unique_ptr, hash)
{
    using unique_ptr_type = unique_ptr<int>;
    using hash_type = hash<unique_ptr_type>;
    using result_type = decltype(hash_type()(declval<unique_ptr_type>()));
    static_assert(is_same<result_type, size_t>::value, "");
}


TEST(shared_ptr, hash)
{
    using shared_ptr_type = shared_ptr<int>;
    using hash_type = hash<shared_ptr_type>;
    using result_type = decltype(hash_type()(declval<shared_ptr_type>()));
    static_assert(is_same<result_type, size_t>::value, "");
}
