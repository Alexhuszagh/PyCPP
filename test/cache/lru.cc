//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief LRU cache unittests.
 */

#include <pycpp/allocator/stack.h>
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
{
    using cache_type = lru_cache<int, int>;

    cache_type cache(50);
    EXPECT_EQ(cache.size(), 0);

    for (size_t i = 0; i < 50; ++i) {
        cache.insert(i, 2*i);
    }
    EXPECT_EQ(cache.size(), 50);
    EXPECT_EQ(cache.at(0), 0);

    cache.erase(cache.cbegin());
    EXPECT_EQ(cache.size(), 49);

    cache.erase(1);
    EXPECT_EQ(cache.size(), 48);

    cache.erase(cache.cbegin(), cache.cend());
    EXPECT_EQ(cache.size(), 0);

    cache_type copy(cache);
    cache.insert(1, 1);
    cache.swap(copy);
    EXPECT_EQ(copy.size(), 1);
    EXPECT_EQ(cache.size(), 0);

    copy.clear();
    EXPECT_EQ(copy.size(), 0);
}


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

    for (size_t i = 50; i < 60; ++i) {
        cache.insert(i, 2*i);
    }
    EXPECT_EQ(cache.size(), 50);
    EXPECT_NE(cache.find(0), cache.end());
    EXPECT_EQ(cache.find(1), cache.end());
}


TEST(lru_cache, access)
{
    // test the core functionality of an LRU cache
    // accessing an item causes that item
    // to be "refreshed", while unaccessed items
    // are evicted from the cache.
    using cache_type = lru_cache<int, int>;
    cache_type c1(2);

    // initialize c1
    c1.insert(1, 1);
    c1.insert(2, 4);
    ASSERT_EQ(c1.size(), 2);
    ASSERT_EQ(c1.cache_size(), 2);
    cache_type c2(c1);

    // access first item in c1
    EXPECT_EQ(c1.at(1), 1);

    // insert items to both caches
    c1.insert(3, 9);
    c2.insert(3, 9);

    // ensure the proper items are evicted
    ASSERT_EQ(c1.size(), 2);
    ASSERT_EQ(c1.cache_size(), 2);
    ASSERT_EQ(c2.size(), 2);
    ASSERT_EQ(c2.cache_size(), 2);
    EXPECT_EQ(*c1.begin(), 9);
    EXPECT_EQ(*c2.begin(), 9);
    EXPECT_EQ(*++c1.begin(), 1);
    EXPECT_EQ(*++c2.begin(), 4);
}


TEST(lru_cache, stack)
{
    using hash_type = hash<int>;
    using equal_type = equal_to<int>;
    using allocator_type = stack_allocator<pair<const int, int>, 200>;
    using arena_type = typename allocator_type::arena_type;
    using cache_type = lru_cache<int, int, hash_type, equal_type, allocator_type>;

    arena_type arena;
    cache_type c1(128, arena);
    c1.insert(3, 9);

    EXPECT_GE(arena.used(), 2 * sizeof(int));
}
