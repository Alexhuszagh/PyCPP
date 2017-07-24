//  :copyright: (c) 2015-2017 Michael Park.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief variant unittests.
 */

#include <pycpp/collections/variant.h>
#include <gtest/gtest.h>
#include <sstream>
#include <tuple>

PYCPP_USING_NAMESPACE

// DATA
// ----

enum qual
{
    lref,
    const_lref,
    rref,
    const_rref
};

struct get_qual
{
  constexpr qual operator()(int &) const { return lref; }
  constexpr qual operator()(const int &) const { return const_lref; }
  constexpr qual operator()(int &&) const { return rref; }
  constexpr qual operator()(const int &&) const { return const_rref; }
};

struct concat
{
    template <typename... Args>
    std::string operator()(const Args &... args) const
    {
        std::ostringstream strm;
        std::initializer_list<int>({(strm << args, 0)...});
        return std::move(strm).str();
    }
};

// TESTS
// -----

TEST(variant, visit_mut_var_mut_type)
{
    variant<int> v(42);
    EXPECT_EQ(42, get<int>(v));
    EXPECT_EQ(lref, visit(get_qual(), v));
    EXPECT_EQ(rref, visit(get_qual(), std::move(v)));
}

TEST(variant, visit_mut_var_const_type)
{
    variant<const int> v(42);
    EXPECT_EQ(42, get<const int>(v));
    EXPECT_EQ(const_lref, visit(get_qual(), v));
    EXPECT_EQ(const_rref, visit(get_qual(), std::move(v)));
}


TEST(variant, visit_const_var_mut_type)
{
    const variant<int> v(42);
    EXPECT_EQ(42, get<int>(v));
    EXPECT_EQ(const_lref, visit(get_qual(), v));
    EXPECT_EQ(const_rref, visit(get_qual(), std::move(v)));

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int> cv(42);
    static_assert(42 == get<int>(cv), "");
    static_assert(const_lref == visit(get_qual(), cv), "");
    static_assert(const_rref == visit(get_qual(), std::move(cv)), "");
#endif
}

TEST(variant, visit_const_var_const_type)
{
    const variant<const int> v(42);
    EXPECT_EQ(42, get<const int>(v));
    EXPECT_EQ(const_lref, visit(get_qual(), v));
    EXPECT_EQ(const_rref, visit(get_qual(), std::move(v)));

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<const int> cv(42);
    static_assert(42 == get<const int>(cv), "");
    static_assert(const_lref == visit(get_qual(), cv), "");
    static_assert(const_rref == visit(get_qual(), std::move(cv)), "");
#endif
}


TEST(variant, visit_zero)
{
    EXPECT_EQ("", visit(concat{}));
}

TEST(variant, visit_homogeneous_double)
{
    variant<int, std::string> v("hello"), w("world!");
    EXPECT_EQ("helloworld!", visit(concat{}, v, w));

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int, double> cv(101), cw(202), cx(3.3);

    struct add_ints
    {
        constexpr int operator()(int lhs, int rhs) const { return lhs + rhs; }
        constexpr int operator()(int lhs, double) const { return lhs; }
        constexpr int operator()(double, int rhs) const { return rhs; }
        constexpr int operator()(double, double) const { return 0; }
    };

    static_assert(303 == visit(add_ints{}, cv, cw), "");
    static_assert(202 == visit(add_ints{}, cw, cx), "");
    static_assert(101 == visit(add_ints{}, cx, cv), "");
    static_assert(0 == visit(add_ints{}, cx, cx), "");
#endif
}


TEST(variant, visit_homogenous_quintuple)
{
    variant<int, std::string> v(101), w("+"), x(202), y("="), z(303);
    EXPECT_EQ("101+202=303", visit(concat{}, v, w, x, y, z));
}


TEST(variant, visit_heterogeneous_double)
{
    variant<int, std::string> v("hello");
    variant<double, const char *> w("world!");
    EXPECT_EQ("helloworld!", visit(concat{}, v, w));
}

TEST(variant, visit_heterogeneous_quintuple)
{
    variant<int, double> v(101);
    variant<const char *> w("+");
    variant<bool, std::string, int> x(202);
    variant<char, std::string, const char *> y('=');
    variant<long, short> z(303L);
    EXPECT_EQ("101+202=303", visit(concat{}, v, w, x, y, z));
}

// TODO: finish all the unittests
