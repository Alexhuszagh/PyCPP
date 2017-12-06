//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `compressed_pair` unittests.
 */

#include <pycpp/collections/compressed_pair.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct e1
{
    e1() = default;
    e1(const e1&) = default;
    e1& operator=(const e1&) = default;
    e1(e1&&) = default;
    e1& operator=(e1&&) = default;
};


struct e2
{
    e2() = default;
    e2(const e2&) = default;
    e2& operator=(const e2&) = default;
    e2(e2&&) = default;
    e2& operator=(e2&&) = default;
};


struct c1
{
    c1() = default;
    c1(const c1&) = default;
    c1& operator=(const c1&) = default;
    c1(c1&&) = default;
    c1& operator=(c1&&) = default;

    int i;
};


struct c2
{
    c2() = default;
    c2(const c2&) = default;
    c2& operator=(const c2&) = default;
    c2(c2&&) = default;
    c2& operator=(c2&&) = default;

    int i;
};

// TESTS
// -----


TEST(compressed_pair, e1_e1)
{
    using pair = compressed_pair<e1, e1>;
    pair p1, p2;
    p1.first();
    p1.second();
    p1.swap(p2);
}


TEST(compressed_pair, e1_e2)
{
    using pair = compressed_pair<e1, e2>;
    pair p1, p2;
    p1.first();
    p1.second();
    p1.swap(p2);
}


TEST(compressed_pair, c1_e1)
{
    using pair = compressed_pair<c1, e1>;
    static_assert(sizeof(pair) == sizeof(c1));
    pair p1, p2;
    p1.first();
    p1.second();
    p1.swap(p2);
}


TEST(compressed_pair, c1_c1)
{
    using pair = compressed_pair<c1, c1>;
    static_assert(sizeof(pair) >= 2* sizeof(c1));
    pair p1, p2;
    p1.first();
    p1.second();
    p1.swap(p2);
}


TEST(compressed_pair, c1_c2)
{
    using pair = compressed_pair<c1, c2>;
    static_assert(sizeof(pair) >= sizeof(c1) + sizeof(c2));
    pair p1, p2;
    p1.first();
    p1.second();
    p1.swap(p2);
}
