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
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
}


TEST(lru_cache, iterator)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    for (auto value: cache)
        ;

    for (auto it = cache.cbegin(); it != cache.cend(); ++it)
        ;
}


TEST(lru_cache, at)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    // TODO: need to put item
    try {
        cache.at(5);
    } catch (...) {
        return;
    }
    EXPECT_TRUE(false);
}


TEST(lru_cache, lookup)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    cache.find(1);
    cache.find(5);
}
