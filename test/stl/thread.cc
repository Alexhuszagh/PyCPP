//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL thread hash unittests.
 */

#include <pycpp/stl/thread.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(thread, hash)
{
    using hash_type = hash<std::thread::id>;
    using result_type = decltype(hash_type()(std::declval<std::thread::id>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}
