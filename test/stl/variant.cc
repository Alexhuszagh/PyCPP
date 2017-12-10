//  :copyright: (c) 2015-2017 Michael Park.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief variant unittests.
 */

#include <pycpp/stl/string.h>
#include <pycpp/stl/variant.h>
#include <gtest/gtest.h>
#include <sstream>
#include <tuple>

PYCPP_USING_NAMESPACE

// DATA
// ----

enum qual
{
    ptr,
    const_ptr,
    lref,
    const_lref,
    rref,
    const_rref
};

struct get_qual
{
    constexpr qual operator()(int *) const { return ptr; }
    constexpr qual operator()(const int *) const { return const_ptr; }
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


struct move_thrower_t
{
    constexpr move_thrower_t() {}
    move_thrower_t(const move_thrower_t &) = default;
    [[noreturn]] move_thrower_t(move_thrower_t &&) { throw std::exception{}; }
    move_thrower_t& operator=(const move_thrower_t&) = default;
    move_thrower_t& operator=(move_thrower_t&&) { throw std::exception{}; }
};

bool operator<(const move_thrower_t &, const move_thrower_t &) { return false; }
bool operator>(const move_thrower_t &, const move_thrower_t &) { return false; }
bool operator<=(const move_thrower_t &, const move_thrower_t &) { return true; }
bool operator>=(const move_thrower_t &, const move_thrower_t &) { return true; }
bool operator==(const move_thrower_t &, const move_thrower_t &) { return true; }
bool operator!=(const move_thrower_t &, const move_thrower_t &) { return false; }


struct dtor
{
    dtor(bool &dtor_called) : dtor_called_(dtor_called) {}
    ~dtor() { dtor_called_ = true; }
    bool &dtor_called_;
};

// TESTS
// -----

TEST(variant, basic)
{
    variant<int, std::string> v("hello world!");
    EXPECT_EQ("hello world!", get<std::string>(v));

    EXPECT_THROW(get<int>(v), bad_variant_access);

    variant<int, std::string> w(v);

    EXPECT_FALSE(get_if<int>(&w));
    EXPECT_TRUE(get_if<std::string>(&w));

    v = 42;

    struct unary
    {
      int operator()(int) const { return 0; }
      int operator()(const std::string &) const { return 1; }
    };

    EXPECT_EQ(0, visit(unary{}, v));

    w = "hello";

    EXPECT_NE(v, w);

    w = 42;

    EXPECT_EQ(v, w);

    struct binary
    {
      int operator()(int, int) const { return 0; }
      int operator()(int, const std::string &) const { return 1; }
      int operator()(const std::string &, int) const { return 2; }
      int operator()(const std::string &, const std::string &) const { return 3; }
    };

    EXPECT_EQ(0, visit(binary{}, v, w));
}


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


TEST(variant, relops_same_type_same_value)
{
    variant<int, std::string> v(0), w(0);
    // `v` op `w`
    EXPECT_TRUE(v == w);
    EXPECT_FALSE(v != w);
    EXPECT_FALSE(v < w);
    EXPECT_FALSE(v > w);
    EXPECT_TRUE(v <= w);
    EXPECT_TRUE(v >= w);
    // `w` op `v`
    EXPECT_TRUE(w == v);
    EXPECT_FALSE(w != v);
    EXPECT_FALSE(w < v);
    EXPECT_FALSE(w > v);
    EXPECT_TRUE(w <= v);
    EXPECT_TRUE(w >= v);

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int, const char *> cv(0), cw(0);
    // `cv` op `cw`
    static_assert(cv == cw, "");
    static_assert(!(cv != cw), "");
    static_assert(!(cv < cw), "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(cv >= cw, "");
    // `cw` op `cv`
    static_assert(cw == cv, "");
    static_assert(!(cw != cv), "");
    static_assert(!(cw < cv), "");
    static_assert(!(cw > cv), "");
    static_assert(cw <= cv, "");
    static_assert(cw >= cv, "");
#endif
}


TEST(variant, relops_same_type_diff_value)
{
    variant<int, std::string> v(0), w(1);
    // `v` op `w`
    EXPECT_FALSE(v == w);
    EXPECT_TRUE(v != w);
    EXPECT_TRUE(v < w);
    EXPECT_FALSE(v > w);
    EXPECT_TRUE(v <= w);
    EXPECT_FALSE(v >= w);
    // `w` op `v`
    EXPECT_FALSE(w == v);
    EXPECT_TRUE(w != v);
    EXPECT_FALSE(w < v);
    EXPECT_TRUE(w > v);
    EXPECT_FALSE(w <= v);
    EXPECT_TRUE(w >= v);

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int, const char *> cv(0), cw(1);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
#endif
}


TEST(variant, relops_diff_type_same_value)
{
    variant<int, unsigned int> v(0), w(0u);
    // `v` op `w`
    EXPECT_FALSE(v == w);
    EXPECT_TRUE(v != w);
    EXPECT_TRUE(v < w);
    EXPECT_FALSE(v > w);
    EXPECT_TRUE(v <= w);
    EXPECT_FALSE(v >= w);
    // `w` op `v`
    EXPECT_FALSE(w == v);
    EXPECT_TRUE(w != v);
    EXPECT_FALSE(w < v);
    EXPECT_TRUE(w > v);
    EXPECT_FALSE(w <= v);
    EXPECT_TRUE(w >= v);

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int, unsigned int> cv(0), cw(0u);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
#endif
}


TEST(variant, relops_diff_type_diff_value)
{
    variant<int, unsigned int> v(0), w(1u);
    // `v` op `w`
    EXPECT_FALSE(v == w);
    EXPECT_TRUE(v != w);
    EXPECT_TRUE(v < w);
    EXPECT_FALSE(v > w);
    EXPECT_TRUE(v <= w);
    EXPECT_FALSE(v >= w);
    // `w` op `v`
    EXPECT_FALSE(w == v);
    EXPECT_TRUE(w != v);
    EXPECT_FALSE(w < v);
    EXPECT_TRUE(w > v);
    EXPECT_FALSE(w <= v);
    EXPECT_TRUE(w >= v);

#if !defined(HAVE_GCC) || COMPILER_MAJOR_VERSION >= 5
    constexpr variant<int, unsigned int> cv(0), cw(1u);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
#endif
}


TEST(variant, relops_one_valueless_by_exception)
{
    // `v` normal, `w` corrupted.
    variant<int, move_thrower_t> v(42), w(42);
    EXPECT_THROW(w = move_thrower_t{}, std::exception);
    EXPECT_FALSE(v.valueless_by_exception());
    EXPECT_TRUE(w.valueless_by_exception());
    // `v` op `w`
    EXPECT_FALSE(v == w);
    EXPECT_TRUE(v != w);
    EXPECT_FALSE(v < w);
    EXPECT_TRUE(v > w);
    EXPECT_FALSE(v <= w);
    EXPECT_TRUE(v >= w);
}


TEST(variant, relops_both_valueless_by_exception)
{
    // `v`, `w` both corrupted.
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    variant<int, move_thrower_t> w(v);
    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_TRUE(w.valueless_by_exception());
    // `v` op `w`
    EXPECT_TRUE(v == w);
    EXPECT_FALSE(v != w);
    EXPECT_FALSE(v < w);
    EXPECT_FALSE(v > w);
    EXPECT_TRUE(v <= w);
    EXPECT_TRUE(v >= w);
}


TEST(variant, swap_same)
{
    variant<int, std::string> v("hello");
    variant<int, std::string> w("world");

    EXPECT_EQ("hello", get<std::string>(v));
    EXPECT_EQ("world", get<std::string>(w));

    std::swap(v, w);

    EXPECT_EQ("world", get<std::string>(v));
    EXPECT_EQ("hello", get<std::string>(w));
}


TEST(variant, swap_diff)
{
    variant<int, std::string> v(42);
    variant<int, std::string> w("hello");

    EXPECT_EQ(42, get<int>(v));
    EXPECT_EQ("hello", get<std::string>(w));

    std::swap(v, w);

    EXPECT_EQ("hello", get<std::string>(v));
    EXPECT_EQ(42, get<int>(w));
}


TEST(variant, swap_one_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42), w(42);
    EXPECT_THROW(w = move_thrower_t{}, std::exception);

    EXPECT_EQ(42, get<int>(v));
    EXPECT_TRUE(w.valueless_by_exception());

    std::swap(v, w);

    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_EQ(42, get<int>(w));
}


TEST(variant, swap_both_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    variant<int, move_thrower_t> w(v);

    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_TRUE(w.valueless_by_exception());

    std::swap(v, w);

    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_TRUE(w.valueless_by_exception());
}


TEST(variant, modifier_emplace_index_direct)
{
    variant<int, std::string> v;
    v.emplace<1>("42");
    EXPECT_EQ("42", get<1>(v));
}


TEST(variant, modifier_emplace_index_direct_duplicate)
{
    variant<int, int> v;
    v.emplace<1>(42);
    EXPECT_EQ(42, get<1>(v));
}


TEST(variant, modifier_emplace_index_conversion)
{
    variant<int, std::string> v;
    v.emplace<1>("42");
    EXPECT_EQ("42", get<1>(v));
}


TEST(variant, modifier_emplace_index_conversion_duplicate)
{
    variant<int, int> v;
    v.emplace<1>(1.1);
    EXPECT_EQ(1, get<1>(v));
}


TEST(variant, modifier_emplace_index_initializer_list)
{
    variant<int, std::string> v;
    v.emplace<1>({'4', '2'});
    EXPECT_EQ("42", get<1>(v));
}


TEST(variant, modifier_emplace_type_direct)
{
    variant<int, std::string> v;
    v.emplace<std::string>("42");
    EXPECT_EQ("42", get<std::string>(v));
}


TEST(variant, modifier_emplace_type_conversion)
{
    variant<int, std::string> v;
    v.emplace<int>(1.1);
    EXPECT_EQ(1, get<int>(v));
}


TEST(variant, modifier_emplace_type_initializer_list)
{
    variant<int, std::string> v;
    v.emplace<std::string>({'4', '2'});
    EXPECT_EQ("42", get<std::string>(v));
}


TEST(variant, hash_monostate)
{
    variant<int, monostate, std::string> v(monostate{});
    std::hash<monostate> std_monostate_hash;
    std::hash<variant<int, monostate, std::string>> std_variant_hash;
    EXPECT_NE(std_monostate_hash(monostate{}), std_variant_hash(v));

    hash<monostate> pycpp_monostate_hash;
    hash<variant<int, monostate, std::string>> pycpp_variant_hash;
    EXPECT_NE(pycpp_monostate_hash(monostate{}), pycpp_variant_hash(v));
}


TEST(variant, hash_string)
{
    variant<int, std::string> v("hello");
    EXPECT_EQ("hello", get<std::string>(v));
    std::hash<std::string> std_string_hash;
    std::hash<variant<int, std::string>> std_variant_hash;
    EXPECT_NE(std_string_hash("hello"), std_variant_hash(v));

    hash<std::string> pycpp_string_hash;
    hash<variant<int, std::string>> pycpp_variant_hash;
    EXPECT_NE(pycpp_string_hash("hello"), pycpp_variant_hash(v));
}


TEST(variant, get_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_THROW(get<int>(v), bad_variant_access);
    EXPECT_THROW(get<move_thrower_t>(v), bad_variant_access);
}


TEST(variant, getif_mut_var_mut_type)
{
    variant<int> v(42);
    EXPECT_EQ(42, *get_if<int>(&v));
    EXPECT_EQ(ptr, get_qual()(get_if<int>(&v)));
}


TEST(variant, getif_mut_var_const_type)
{
    variant<const int> v(42);
    EXPECT_EQ(42, *get_if<const int>(&v));
    EXPECT_EQ(const_ptr, get_qual()(get_if<const int>(&v)));
}


TEST(variant, getif_const_var_mut_type)
{
    const variant<int> v(42);
    EXPECT_EQ(42, *get_if<int>(&v));
    EXPECT_EQ(const_ptr, get_qual()(get_if<int>(&v)));

    static constexpr variant<int> cv(42);
    static_assert(42 == *get_if<int>(&cv), "");
    static_assert(const_ptr == get_qual()(get_if<int>(&cv)), "");
}


TEST(variant, getif_const_var_const_type)
{
    const variant<const int> v(42);
    EXPECT_EQ(42, *get_if<const int>(&v));
    EXPECT_EQ(const_ptr, get_qual()(get_if<const int>(&v)));

    static constexpr variant<const int> cv(42);
    static_assert(42 == *get_if<const int>(&cv), "");
    static_assert(const_ptr == get_qual()(get_if<const int>(&cv)), "");
}


TEST(variant, getif_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    EXPECT_EQ(nullptr, get_if<int>(&v));
    EXPECT_EQ(nullptr, get_if<move_thrower_t>(&v));
}


TEST(variant, dtor)
{
    bool dtor_called = false;
    {
        variant<dtor> v(in_place_type_t<dtor>{}, dtor_called);
    }
    EXPECT_TRUE(dtor_called);
}


TEST(variant, ctor_move_value)
{
    variant<int, std::string> v("hello");
    EXPECT_EQ("hello", get<std::string>(v));
    variant<int, std::string> w(std::move(v));
    EXPECT_EQ("hello", get<std::string>(w));
    EXPECT_TRUE(get<std::string>(v).empty());

    constexpr variant<int, const char *> cv(42);
    static_assert(42 == get<int>(cv), "");
    constexpr variant<int, const char *> cw(std::move(cv));
    static_assert(42 == get<int>(cw), "");
}


TEST(variant, ctor_move_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    variant<int, move_thrower_t> w(std::move(v));
    EXPECT_TRUE(w.valueless_by_exception());
}


TEST(variant, ctor_inplace_index_direct)
{
    variant<int, std::string> v(in_place_index_t<0>{}, 42);
    EXPECT_EQ(42, get<0>(v));

    constexpr variant<int, const char *> cv(in_place_index_t<0>{}, 42);
    static_assert(42 == get<0>(cv), "");
}


TEST(variant, ctor_inplace_index_direct_duplicate)
{
    variant<int, int> v(in_place_index_t<0>{}, 42);
    EXPECT_EQ(42, get<0>(v));

    constexpr variant<int, int> cv(in_place_index_t<0>{}, 42);
    static_assert(42 == get<0>(cv), "");
}


TEST(variant, ctor_inplace_index_conversion)
{
    variant<int, std::string> v(in_place_index_t<1>{}, "42");
    EXPECT_EQ("42", get<1>(v));

    constexpr variant<int, const char *> cv(in_place_index_t<0>{}, 1.1);
    static_assert(1 == get<0>(cv), "");
}


TEST(variant, ctor_inplace_index_initializer_list)
{
    variant<int, std::string> v(in_place_index_t<1>{}, {'4', '2'});
    EXPECT_EQ("42", get<1>(v));
}


TEST(variant, ctor_inplace_type_direct)
{
    variant<int, std::string> v(in_place_type_t<std::string>{}, "42");
    EXPECT_EQ("42", get<std::string>(v));

    constexpr variant<int, const char *> cv(in_place_type_t<int>{}, 42);
    static_assert(42 == get<int>(cv), "");
}


TEST(variant, ctor_inplace_type_conversion)
{
    variant<int, std::string> v(in_place_type_t<int>{}, 42.5);
    EXPECT_EQ(42, get<int>(v));

    constexpr variant<int, const char *> cv(in_place_type_t<int>{}, 42.5);
    static_assert(42 == get<int>(cv), "");
}


TEST(variant, ctor_inplace_type_initializer_list)
{
    variant<int, std::string> v(in_place_type_t<std::string>{}, {'4', '2'});
    EXPECT_EQ("42", get<std::string>(v));
}


TEST(variant, ctor_fwd_direct)
{
    variant<int, std::string> v(42);
    EXPECT_EQ(42, get<int>(v));

    constexpr variant<int, const char *> cv(42);
    static_assert(42 == get<int>(cv), "");
}


TEST(variant, ctor_fwd_direct_conversion)
{
    variant<int, std::string> v("42");
    EXPECT_EQ("42", get<std::string>(v));

    constexpr variant<int, const char *> cv(1.1);
    static_assert(1 == get<int>(cv), "");
}


TEST(variant, ctor_fwd_copy_initialization)
{
    variant<int, std::string> v = 42;
    EXPECT_EQ(42, get<int>(v));

    constexpr variant<int, const char *> cv = 42;
    static_assert(42 == get<int>(cv), "");
}


TEST(variant, ctor_fwd_copy_initialization_conversion)
{
    variant<int, std::string> v = "42";
    EXPECT_EQ("42", get<std::string>(v));

    constexpr variant<int, const char *> cv = 1.1;
    static_assert(1 == get<int>(cv), "");
}


TEST(variant, ctor_default)
{
    variant<int, std::string> v;
    EXPECT_EQ(0, get<0>(v));

    constexpr variant<int> cv{};
    static_assert(0 == get<0>(cv), "");
}


TEST(variant, ctor_copy)
{
    variant<int, std::string> v("hello");
    EXPECT_EQ("hello", get<std::string>(v));
    variant<int, std::string> w(v);
    EXPECT_EQ("hello", get<std::string>(w));
    EXPECT_EQ("hello", get<std::string>(v));

    constexpr variant<int, const char *> cv(42);
    static_assert(42 == get<int>(cv), "");
    constexpr variant<int, const char *> cw(cv);
    static_assert(42 == get<int>(cw), "");
}


TEST(variant, ctor_copy_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    variant<int, move_thrower_t> w(v);
    EXPECT_TRUE(w.valueless_by_exception());
}

TEST(variant, assign_fwd_same_type)
{
    variant<int, std::string> v(101);
    EXPECT_EQ(101, get<int>(v));
    v = 202;
    EXPECT_EQ(202, get<int>(v));
}


TEST(variant, assign_fwd_same_type_fwd)
{
    variant<int, std::string> v(1.1);
    EXPECT_EQ(1, get<int>(v));
    v = 2.2;
    EXPECT_EQ(2, get<int>(v));
}


TEST(variant, assign_fwd_diff_type)
{
    variant<int, std::string> v(42);
    EXPECT_EQ(42, get<int>(v));
    v = "42";
    EXPECT_EQ("42", get<std::string>(v));
}


TEST(variant, assign_fwd_diff_type_fwd)
{
    variant<int, std::string> v(42);
    EXPECT_EQ(42, get<int>(v));
    v = "42";
    EXPECT_EQ("42", get<std::string>(v));
}


TEST(variant, assign_fwd_exact_match)
{
    variant<const char *, std::string> v;
    v = std::string("hello");
    EXPECT_EQ("hello", get<std::string>(v));
}

TEST(variant, assign_fwd_better_match)
{
    variant<int, double> v;
    v = 'x';
    EXPECT_EQ(static_cast<int>('x'), get<int>(v));
}


TEST(variant, assign_fwd_nomatch)
{
    struct x {};
    static_assert(!std::is_assignable<variant<int, std::string>, x>{}, "variant<int, std::string> v; v = x;");
}


TEST(variant, assign_fwd_ambiguous)
{
    static_assert(!std::is_assignable<variant<short, long>, int>{}, "variant<short, long> v; v = 42;");
}


TEST(variant, assign_fwd_same_type_optimization)
{
    variant<int, std::string> v("hello world!");
    const std::string &x = get<std::string>(v);
    EXPECT_EQ("hello world!", x);
    auto capacity = x.capacity();
    v = "hello";
    const std::string &y = get<std::string>(v);
    EXPECT_EQ("hello", y);
    EXPECT_EQ(capacity, y.capacity());
}


TEST(variant, assign_fwd_throw_on_assign)
{
    variant<int, move_thrower_t> v(in_place_type_t<move_thrower_t>{});
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_FALSE(v.valueless_by_exception());
    EXPECT_EQ(1u, v.index());
    v = 42;
    EXPECT_FALSE(v.valueless_by_exception());
    EXPECT_EQ(42, get<int>(v));
}


TEST(variant, assign_move_same_type)
{
    struct x {
        constexpr x() {}
        x(const x &) = delete;
        x(x &&) noexcept { EXPECT_TRUE(false); }
        x &operator=(const x &) = delete;
        x &operator=(x &&) noexcept { EXPECT_TRUE(true); return *this; }
    };
    variant<x, int> v, w;
    v = std::move(w);
}

TEST(variant, assign_move_diff_type)
{
    struct x {
        constexpr x() {}
        x(const x &) = delete;
        x(x &&) noexcept { EXPECT_TRUE(true); }
        x &operator=(const x &) = delete;
        x &operator=(x &&) noexcept { EXPECT_TRUE(false); return *this; }
    };
    variant<x, int> v(42), w;
    v = std::move(w);
}

TEST(variant, assign_move_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    variant<int, move_thrower_t> w(42);
    w = std::move(v);
    EXPECT_TRUE(w.valueless_by_exception());
}


TEST(variant, assign_copy_same_type)
{
    struct x {
        constexpr x() {}
        x(const x &) noexcept { EXPECT_TRUE(false); }
        x(x &&) = default;
        x &operator=(const x &) noexcept { EXPECT_TRUE(true); return *this; }
        x &operator=(x &&) = delete;
    };
    variant<x, int> v, w;
    v = w;
}


TEST(variant, assign_copy_diff_type)
{
    struct x {
        constexpr x() {}
        x(const x &) noexcept { EXPECT_TRUE(true); }
        x(x &&) = default;
        x &operator=(const x &) noexcept { EXPECT_TRUE(false); return *this; }
        x &operator=(x &&) = delete;
    };
    variant<x, int> v(42), w;
    v = w;
}


TEST(variant, assign_copy_valueless_by_exception)
{
    variant<int, move_thrower_t> v(42);
    EXPECT_THROW(v = move_thrower_t{}, std::exception);
    EXPECT_TRUE(v.valueless_by_exception());
    variant<int, move_thrower_t> w(42);
    w = v;
    EXPECT_TRUE(w.valueless_by_exception());
}
