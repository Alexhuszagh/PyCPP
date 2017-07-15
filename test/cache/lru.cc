//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief LRU cache unittests.
 */

#include <pycpp/cache/lru.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(lru_cache, constructor)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
}


TEST(lru_cache, capacity)
{}


TEST(lru_cache, iterator)
{}
