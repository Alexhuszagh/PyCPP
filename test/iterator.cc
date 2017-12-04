//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator unittests.
 */

#include <pycpp/iterator.h>
#include <gtest/gtest.h>
#include <map>
#include <unordered_map>
#include <vector>

PYCPP_USING_NAMESPACE

// HELPERS
// -------


int mul2(int x)
{
    return x * 2;
}


struct int_generator
{
    using value_type = int;
    int counter;

    int_generator(int c = 5):
        counter(c)
    {}

    int operator()()
    {
        return counter++;
    }

    explicit operator bool() const
    {
        return counter < 5;
    }
};

// TESTS
// -----


TEST(iterator, category)
{
    using input_iterator = std::istream_iterator<int>;
    using output_iterator = std::ostream_iterator<int>;
    using forward_iterator = typename std::unordered_map<int, int>::iterator;
    using bidirectional_iterator = typename std::map<int, int>::iterator;
    using random_access_iterator = typename std::vector<int>::iterator;

    // type
    static_assert(is_input_iterator<input_iterator>::value, "");
    static_assert(!is_input_iterator<output_iterator>::value, "");
    static_assert(!is_input_iterator<forward_iterator>::value, "");
    static_assert(!is_input_iterator<bidirectional_iterator>::value, "");
    static_assert(!is_input_iterator<random_access_iterator>::value, "");
    static_assert(!is_output_iterator<input_iterator>::value, "");
    static_assert(is_output_iterator<output_iterator>::value, "");
    static_assert(!is_output_iterator<forward_iterator>::value, "");
    static_assert(!is_output_iterator<bidirectional_iterator>::value, "");
    static_assert(!is_output_iterator<random_access_iterator>::value, "");
    static_assert(!is_forward_iterator<input_iterator>::value, "");
    static_assert(!is_forward_iterator<output_iterator>::value, "");
    static_assert(is_forward_iterator<forward_iterator>::value, "");
    static_assert(!is_forward_iterator<bidirectional_iterator>::value, "");
    static_assert(!is_forward_iterator<random_access_iterator>::value, "");
    static_assert(!is_bidirectional_iterator<input_iterator>::value, "");
    static_assert(!is_bidirectional_iterator<output_iterator>::value, "");
    static_assert(!is_bidirectional_iterator<forward_iterator>::value, "");
    static_assert(is_bidirectional_iterator<bidirectional_iterator>::value, "");
    static_assert(!is_bidirectional_iterator<random_access_iterator>::value, "");
    static_assert(!is_random_access_iterator<input_iterator>::value, "");
    static_assert(!is_random_access_iterator<output_iterator>::value, "");
    static_assert(!is_random_access_iterator<forward_iterator>::value, "");
    static_assert(!is_random_access_iterator<bidirectional_iterator>::value, "");
    static_assert(is_random_access_iterator<random_access_iterator>::value, "");

    // concepts
    static_assert(is_input_iterable<input_iterator>::value, "");
    static_assert(!is_input_iterable<output_iterator>::value, "");
    static_assert(is_input_iterable<forward_iterator>::value, "");
    static_assert(is_input_iterable<bidirectional_iterator>::value, "");
    static_assert(is_input_iterable<random_access_iterator>::value, "");
    static_assert(!is_output_iterable<input_iterator>::value, "");
    static_assert(is_output_iterable<output_iterator>::value, "");
    static_assert(!is_output_iterable<forward_iterator>::value, "");
    static_assert(!is_output_iterable<bidirectional_iterator>::value, "");
    static_assert(!is_output_iterable<random_access_iterator>::value, "");
    static_assert(!is_forward_iterable<input_iterator>::value, "");
    static_assert(!is_forward_iterable<output_iterator>::value, "");
    static_assert(is_forward_iterable<forward_iterator>::value, "");
    static_assert(is_forward_iterable<bidirectional_iterator>::value, "");
    static_assert(is_forward_iterable<random_access_iterator>::value, "");
    static_assert(!is_bidirectional_iterable<input_iterator>::value, "");
    static_assert(!is_bidirectional_iterable<output_iterator>::value, "");
    static_assert(!is_bidirectional_iterable<forward_iterator>::value, "");
    static_assert(is_bidirectional_iterable<bidirectional_iterator>::value, "");
    static_assert(is_bidirectional_iterable<random_access_iterator>::value, "");
    static_assert(!is_random_access_iterable<input_iterator>::value, "");
    static_assert(!is_random_access_iterable<output_iterator>::value, "");
    static_assert(!is_random_access_iterable<forward_iterator>::value, "");
    static_assert(!is_random_access_iterable<bidirectional_iterator>::value, "");
    static_assert(is_random_access_iterable<random_access_iterator>::value, "");
}


TEST(iterator, transform_iterator)
{
    std::vector<int> list = {0, 1, 2, 3, 4};
    auto first = make_transform_iterator(list.begin(), mul2);
    auto last = make_transform_iterator(list.end(), mul2);
    EXPECT_TRUE(std::equal(first, last, list.begin(), [](int l, int r) {
        return l == r * 2;
    }));

    // incrementors
    EXPECT_EQ(*first, 0);
    ++first;
    EXPECT_EQ(*first, 2);
    --first;

    // indexing
    EXPECT_EQ(first[4], 8);

    // arithmetic
    first += 4;
    EXPECT_EQ(*first, 8);
    first -= 2;
    EXPECT_EQ(*first, 4);
    first -= 2;
    EXPECT_EQ(*first, 0);

    // comparison
    EXPECT_EQ(first, first);
    EXPECT_NE(first, last);
    EXPECT_LT(first, last);
    EXPECT_LE(first, last);
    EXPECT_GT(last, first);
    EXPECT_GE(last, first);
}


TEST(iterator, input_iterator_facade)
{
    using iterator = input_iterator_facade<int_generator>;
    int_generator g(0);
    auto v = std::vector<int>(iterator(g), iterator());
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[4], 4);
}


TEST(iterator, chunked_range)
{
    using vector = std::vector<int>;
    using vector_range = chunked_range<typename vector::const_iterator>;
    using input_iterator = input_iterator_facade<int_generator>;
    using input_range = chunked_range<input_iterator>;

    // forward+ iterators
    vector v = {1, 2, 3, 1, 4, 2, 5};
    vector_range r1(v.begin(), v.end(), 3);
    auto f1 = r1.begin();
    auto l1 = r1.end();

    // first
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 3);
    EXPECT_EQ(f1->at(0), v[0]);
    EXPECT_EQ(f1->at(1), v[1]);
    EXPECT_EQ(f1->at(2), v[2]);
    auto f1_copy = f1;      // store a copy to ensure incrementing doesn't use existing chunk
    ++f1;

    // first -- copy
    // This is technically above the guarantees of an input iterator,
    // but our input iterator is a forward iterator with expensive
    // increments, so we make it masquerade as an input iterator.
    ASSERT_EQ(f1_copy->size(), 3);
    EXPECT_EQ(f1_copy->at(0), v[0]);
    EXPECT_EQ(f1_copy->at(1), v[1]);
    EXPECT_EQ(f1_copy->at(2), v[2]);

    // second
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 3);
    EXPECT_EQ(f1->at(0), v[3]);
    EXPECT_EQ(f1->at(1), v[4]);
    EXPECT_EQ(f1->at(2), v[5]);
    ++f1;

    // third
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 1);
    EXPECT_EQ(f1->at(0), v[6]);
    ++f1;

    // fourth
    ASSERT_EQ(f1, l1);

    // forward+ iterators -- 2nd pass
    vector_range r2(v.begin(), v.end(), 3);
    auto f2 = r2.begin();
    auto l2 = r2.end();
    ASSERT_NE(f2, l2);
    ASSERT_EQ(f2->size(), 3);
    EXPECT_EQ(f2->at(0), v[0]);
    EXPECT_EQ(f2->at(1), v[1]);
    EXPECT_EQ(f2->at(2), v[2]);

    // input iterators
    int_generator g(0);
    auto r3 = input_range(input_iterator(g), input_iterator());
    auto f3 = r3.begin();
    auto l3 = r3.end();

    // first
    ASSERT_NE(f3, l3);
    ASSERT_EQ(f3->size(), 3);
    EXPECT_EQ(f3->at(0), 0);
    EXPECT_EQ(f3->at(1), 1);
    EXPECT_EQ(f3->at(2), 2);
    ++f3;

    // second
    ASSERT_NE(f3, l3);
    ASSERT_EQ(f3->size(), 2);
    EXPECT_EQ(f3->at(0), 3);
    EXPECT_EQ(f3->at(1), 4);
    ++f3;

    // third
    ASSERT_EQ(f3, l3);
}


TEST(iterator, unique_range)
{
    using vector = std::vector<int>;
    using vector_range = unique_range<typename vector::const_iterator>;
    using input_iterator = input_iterator_facade<int_generator>;
    using input_range = unique_range<input_iterator>;

    // forward+ iterators
    vector v = {1, 2, 3, 1, 4, 2, 5};
    vector_range r1(v.begin(), v.end());
    vector_range r2(v.begin(), v.end());
    vector v1(r1.begin(), r1.end());
    vector v2(r2.begin(), r2.end());
    ASSERT_EQ(v1.size(), 5);
    ASSERT_EQ(v2.size(), 5);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v1[4], 5);
    EXPECT_EQ(v2[4], 5);

    // input iterators
    int_generator g(0);
    auto r3 = input_range(input_iterator(g), input_iterator());
    vector v3(r3.begin(), r3.end());
    ASSERT_EQ(v3.size(), 5);
    EXPECT_EQ(v3[0], 0);
    EXPECT_EQ(v3[4], 4);
}


TEST(iterator, windowed_range)
{
    using vector = std::vector<int>;
    using vector_range = windowed_range<typename vector::const_iterator>;
    using input_iterator = input_iterator_facade<int_generator>;
    using input_range = windowed_range<input_iterator>;

    // forward+ iterators
    vector v = {1, 2, 3, 4, 5};
    vector_range r1(v.begin(), v.end(), 3);
    auto f1 = r1.begin();
    auto l1 = r1.end();

    // first
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 3);
    EXPECT_EQ(f1->at(0), v[0]);
    EXPECT_EQ(f1->at(1), v[1]);
    EXPECT_EQ(f1->at(2), v[2]);
    auto f1_copy = f1;      // store a copy to ensure incrementing doesn't use existing chunk
    ++f1;

    // first -- copy
    // This is technically above the guarantees of an input iterator,
    // but our input iterator is a forward iterator with expensive
    // increments, so we make it masquerade as an input iterator.
    ASSERT_EQ(f1_copy->size(), 3);
    EXPECT_EQ(f1_copy->at(0), v[0]);
    EXPECT_EQ(f1_copy->at(1), v[1]);
    EXPECT_EQ(f1_copy->at(2), v[2]);

    // second
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 3);
    EXPECT_EQ(f1->at(0), v[1]);
    EXPECT_EQ(f1->at(1), v[2]);
    EXPECT_EQ(f1->at(2), v[3]);
    ++f1;

    // third
    ASSERT_NE(f1, l1);
    ASSERT_EQ(f1->size(), 3);
    EXPECT_EQ(f1->at(0), v[2]);
    EXPECT_EQ(f1->at(1), v[3]);
    EXPECT_EQ(f1->at(2), v[4]);
    ++f1;

    // fourth
    ASSERT_EQ(f1, l1);

    // forward+ iterators -- 2nd pass
    vector_range r2(v.begin(), v.end(), 3);
    auto f2 = r2.begin();
    auto l2 = r2.end();
    ASSERT_NE(f2, l2);
    ASSERT_EQ(f2->size(), 3);
    EXPECT_EQ(f2->at(0), v[0]);
    EXPECT_EQ(f2->at(1), v[1]);
    EXPECT_EQ(f2->at(2), v[2]);

    // input iterators
    int_generator g(0);
    auto r3 = input_range(input_iterator(g), input_iterator(), 3);
    auto f3 = r3.begin();
    auto l3 = r3.end();

    // first
    ASSERT_NE(f3, l3);
    ASSERT_EQ(f3->size(), 3);
    EXPECT_EQ(f3->at(0), 0);
    EXPECT_EQ(f3->at(1), 1);
    EXPECT_EQ(f3->at(2), 2);
    ++f3;

    // second
    ASSERT_NE(f3, l3);
    ASSERT_EQ(f3->size(), 3);
    EXPECT_EQ(f3->at(0), 1);
    EXPECT_EQ(f3->at(1), 2);
    EXPECT_EQ(f3->at(2), 3);
    ++f3;

    // third
    ASSERT_NE(f3, l3);
    ASSERT_EQ(f3->size(), 3);
    EXPECT_EQ(f3->at(0), 2);
    EXPECT_EQ(f3->at(1), 3);
    EXPECT_EQ(f3->at(2), 4);
    ++f3;

    // fourth
    ASSERT_EQ(f3, l3);
}
