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
    EXPECT_EQ(cache.size(), 0);

    cache.insert(1, 1);
    EXPECT_EQ(cache.size(), 1);

    // copy constructor
    cache_type copy(cache);
    EXPECT_EQ(copy.size(), 1);

    // copy assignment
    copy = cache;
    EXPECT_EQ(copy.size(), 1);

    // move constructor
    cache_type blank(std::move(cache));
    EXPECT_EQ(cache.size(), 0);
    EXPECT_EQ(blank.size(), 1);

    // move assignment
    cache = std::move(copy);
    EXPECT_EQ(copy.size(), 0);
    EXPECT_EQ(cache.size(), 1);
}


TEST(lru_cache, capacity)
{
    using cache_type = lru_cache<int, int>;
    cache_type cache(50);

    // EMPTY
    EXPECT_EQ(cache.size(), 0);
    EXPECT_EQ(cache.cache_size(), 50);
    EXPECT_GE(cache.max_size(), 50);
    EXPECT_TRUE(cache.empty());

    // 1 element
    cache.insert(1, 1);
    EXPECT_EQ(cache.size(), 1);
    EXPECT_EQ(cache.cache_size(), 50);
    EXPECT_GE(cache.max_size(), 50);
    EXPECT_FALSE(cache.empty());
}


TEST(lru_cache, iterator)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    cache.insert(1, 2);
    ASSERT_EQ(cache.size(), 1);
    for (auto value: cache)
        EXPECT_EQ(value, 2);

    for (auto it = cache.cbegin(); it != cache.cend(); ++it)
        EXPECT_EQ(*it, 2);
}


TEST(lru_cache, indexing)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    cache.insert(1, 2);
    EXPECT_EQ(cache[1], 2);
    cache[5] = 3;
    EXPECT_EQ(cache[5], 3);
}


TEST(lru_cache, at)
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    cache.insert(1, 2);
    EXPECT_EQ(cache.at(1), 2);
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
    cache.insert(1, 2);
    EXPECT_NE(cache.find(1), cache.end());
    EXPECT_EQ(cache.find(5), cache.end());
    EXPECT_EQ(cache.count(1), 1);
    EXPECT_EQ(cache.count(5), 0);

    auto pair = cache.equal_range(1);
    EXPECT_EQ(std::distance(pair.first, pair.second), 1);

    pair = cache.equal_range(5);
    EXPECT_EQ(std::distance(pair.first, pair.second), 0);
}


TEST(lru_cache, modifiers)
{}


TEST(lru_cache, bucket)
{
    using cache_type = lru_cache<int, int>;
    cache_type cache(50);
    cache.insert(1, 1);
    cache.bucket_count();
    cache.max_bucket_count();
    cache.bucket_size(cache.bucket(1));
}


TEST(lru_cache, hash)
{
    using cache_type = lru_cache<int, int>;
    cache_type cache(50);
    cache.load_factor();
    cache.max_load_factor();
    cache.max_load_factor(5.0);
    cache.rehash(5);
    cache.reserve(5);
}


TEST(lru_cache, observers)
{
    using cache_type = lru_cache<int, int>;
    cache_type cache(50);

    cache.hash_function();
    cache.key_eq();
    cache.get_allocator();
}


TEST(lru_cache, cache_size)
{
    using cache_type = lru_cache<int, int>;
    cache_type cache(50);

    for (size_t i = 0; i < 50; ++i) {
        cache.insert(i, 2*i);
    }
    EXPECT_EQ(cache.size(), 50);
    EXPECT_EQ(cache.at(0), 0);

    // TODO: add 50 elements
    // access 1
    // TODO: add more
    // ensure size is 50 and the one accessed remains...
    exit(0);
}
