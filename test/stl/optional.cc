//  :copyright: (c) 2011-2012 Andrzej Krzemienski.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Optional type unittests.
 */

#include <pycpp/stl/optional.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

enum state
{
    sDefaultConstructed,
    sValueCopyConstructed,
    sValueMoveConstructed,
    sCopyConstructed,
    sMoveConstructed,
    sMoveAssigned,
    sCopyAssigned,
    sValueCopyAssigned,
    sValueMoveAssigned,
    sMovedFrom,
    sValueConstructed
};

struct oracle_value
{
    state s;
    int i;
    oracle_value(int i = 0) : s(sValueConstructed), i(i) {}
};

struct oracle
{
    state s;
    oracle_value val;

    oracle() : s(sDefaultConstructed) {}
    oracle(const oracle_value& v) : s(sValueCopyConstructed), val(v) {}
    oracle(oracle_value&& v) : s(sValueMoveConstructed), val(std::move(v)) {v.s = sMovedFrom;}
    oracle(const oracle& o) : s(sCopyConstructed), val(o.val) {}
    oracle(oracle&& o) : s(sMoveConstructed), val(std::move(o.val)) {o.s = sMovedFrom;}

    oracle& operator=(const oracle_value& v) { s = sValueCopyConstructed; val = v; return *this; }
    oracle& operator=(oracle_value&& v) { s = sValueMoveConstructed; val = std::move(v); v.s = sMovedFrom; return *this; }
    oracle& operator=(const oracle& o) { s = sCopyConstructed; val = o.val; return *this; }
    oracle& operator=(oracle&& o) { s = sMoveConstructed; val = std::move(o.val); o.s = sMovedFrom; return *this; }
};

bool operator==(oracle const& a, oracle const& b) { return a.val.i == b.val.i; }
bool operator!=(oracle const& a, oracle const& b) { return a.val.i != b.val.i; }

template <class T>
struct move_aware
{
    T val;
    bool moved;

    move_aware(T val):
        val(val),
        moved(false)
    {}

    move_aware(move_aware const&) = delete;
    move_aware(move_aware&& rhs):
        val(rhs.val),
        moved(rhs.moved)
    {
        rhs.moved = true;
    }

    move_aware& operator=(move_aware const&) = delete;
    move_aware& operator=(move_aware&& rhs)
    {
        val = (rhs.val);
        moved = (rhs.moved);
        rhs.moved = true;
        return *this;
    }
};

struct guard
{
    std::string val;

    guard():
        val{}
    {}

    explicit guard(std::string s, int = 0):
        val(s)
    {}

    guard(const guard&) = delete;
    guard(guard&&) = delete;
    void operator=(const guard&) = delete;
    void operator=(guard&&) = delete;
};

struct date
{
    int i;
    date() = delete;
    date(int i) : i{i} {};
    date(date&& d) : i(d.i) { d.i = 0; }
    date(const date&) = delete;
    date& operator=(const date&) = delete;
    date& operator=(date&& d) { i = d.i; d.i = 0; return *this;};
};

// TESTS
// -----


TEST(optional, disengaged_ctor)
{
    optional<int> o1;
    EXPECT_FALSE(o1);

    optional<int> o2 = nullopt;
    EXPECT_FALSE(o2);

    optional<int> o3 = o2;
    EXPECT_FALSE(o3);

    EXPECT_EQ(o1, nullopt);
    EXPECT_EQ(o1, optional<int>{});
    EXPECT_FALSE(o1);
    EXPECT_FALSE(bool(o1));

    EXPECT_EQ(o2, nullopt);
    EXPECT_EQ(o2, optional<int>{});
    EXPECT_FALSE(o2);
    EXPECT_FALSE(bool(o2));

    EXPECT_EQ(o3, nullopt);
    EXPECT_EQ(o3, optional<int>{});
    EXPECT_FALSE(o3);
    EXPECT_FALSE(bool(o3));

    EXPECT_EQ(o1, o2);
    EXPECT_EQ(o2, o1);
    EXPECT_EQ(o1, o3);
    EXPECT_EQ(o3, o1);
    EXPECT_EQ(o2, o3);
    EXPECT_EQ(o3, o2);
}


TEST(optional, value_ctor)
{
    oracle_value v;
    optional<oracle> oo1(v);
    EXPECT_NE(oo1, nullopt);
    EXPECT_NE(oo1, optional<oracle>{});
    EXPECT_EQ(oo1, optional<oracle>{v});
    EXPECT_TRUE(!!oo1);
    EXPECT_TRUE(bool(oo1));
    EXPECT_EQ(oo1->s, sMoveConstructed);
    EXPECT_EQ(v.s, sValueConstructed);

    optional<oracle> oo2(std::move(v));
    EXPECT_NE(oo2, nullopt);
    EXPECT_NE(oo2, optional<oracle>{});
    EXPECT_EQ(oo2, oo1);
    EXPECT_TRUE(!!oo2);
    EXPECT_TRUE(bool(oo2));
    EXPECT_EQ(oo2->s, sMoveConstructed);
    EXPECT_EQ(v.s, sMovedFrom);

    {
        oracle_value v;
        optional<oracle> oo1{in_place, v};
        EXPECT_NE(oo1, nullopt);
        EXPECT_NE(oo1, optional<oracle>{});
        EXPECT_EQ(oo1, optional<oracle>{v});
        EXPECT_TRUE(!!oo1);
        EXPECT_TRUE(bool(oo1));
        EXPECT_EQ(oo1->s, sValueCopyConstructed);
        EXPECT_EQ(v.s, sValueConstructed);

        optional<oracle> oo2{in_place, std::move(v)};
        EXPECT_NE(oo2, nullopt);
        EXPECT_NE(oo2, optional<oracle>{});
        EXPECT_EQ(oo2, oo1);
        EXPECT_TRUE(!!oo2);
        EXPECT_TRUE(bool(oo2));
        EXPECT_EQ(oo2->s, sValueMoveConstructed);
        EXPECT_EQ(v.s, sMovedFrom);
    }
};


TEST(optional, assignment)
{
    optional<int> oi;
    oi = optional<int>{1};
    EXPECT_EQ(*oi, 1);

    oi = nullopt;
    EXPECT_TRUE(!oi);

    oi = 2;
    EXPECT_EQ(*oi, 2);

    oi = {};
    EXPECT_TRUE(!oi);
};


TEST(optional, moved_from_state)
{
    // first, test mock:
    move_aware<int> i{1}, j{2};
    EXPECT_EQ(i.val, 1);
    EXPECT_FALSE(i.moved);
    EXPECT_EQ(j.val, 2);
    EXPECT_FALSE(j.moved);

    move_aware<int> k = std::move(i);
    EXPECT_EQ(k.val, 1);
    EXPECT_FALSE(k.moved);
    EXPECT_EQ(i.val, 1);
    EXPECT_TRUE(i.moved);

    k = std::move(j);
    EXPECT_EQ(k.val, 2);
    EXPECT_FALSE(k.moved);
    EXPECT_EQ(j.val, 2);
    EXPECT_TRUE(j.moved);

    // now, test optional
    optional<move_aware<int>> oi{1}, oj{2};
    EXPECT_TRUE(oi);
    EXPECT_FALSE(oi->moved);
    EXPECT_TRUE(oj);
    EXPECT_FALSE(oj->moved);

    optional<move_aware<int>> ok = std::move(oi);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(ok->moved);
    EXPECT_TRUE(oi);
    EXPECT_TRUE(oi->moved);

    ok = std::move(oj);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(ok->moved);
    EXPECT_TRUE(oj);
    EXPECT_TRUE(oj->moved);
};


TEST(optional, copy_move_ctor_optional_int)
{
    optional<int> oi;
    optional<int> oj = oi;

    EXPECT_FALSE(oj);
    EXPECT_EQ(oj, oi);
    EXPECT_EQ(oj, nullopt);
    EXPECT_FALSE(bool(oj));

    oi = 1;
    optional<int> ok = oi;
    EXPECT_TRUE(!!ok);
    EXPECT_TRUE(bool(ok));
    EXPECT_EQ(ok, oi);
    EXPECT_NE(ok, oj);
    EXPECT_EQ(*ok, 1);

    optional<int> ol = std::move(oi);
    EXPECT_TRUE(!!ol);
    EXPECT_TRUE(bool(ol));
    EXPECT_EQ(ol, oi);
    EXPECT_NE(ol, oj);
    EXPECT_EQ(*ol, 1);
};


TEST(optional, optional_optional)
{
    optional<optional<int>> oi1 = nullopt;
    EXPECT_EQ(oi1, nullopt);
    EXPECT_FALSE(oi1);

    {
        optional<optional<int>> oi2 {in_place};
        EXPECT_NE(oi2, nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, nullopt);
    }

    {
        optional<optional<int>> oi2 {in_place, nullopt};
        EXPECT_NE(oi2, nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, nullopt);
        EXPECT_FALSE(*oi2);
    }

    {
        optional<optional<int>> oi2 {optional<int>{}};
        EXPECT_NE(oi2, nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, nullopt);
        EXPECT_FALSE(*oi2);
    }

    optional<int> oi;
    auto ooi = make_optional(oi);
    static_assert(std::is_same<optional<optional<int>>, decltype(ooi)>::value, "");
};


TEST(optional, example_guard)
{
    optional<guard> oga;
    optional<guard> ogb(in_place, "res1");
    EXPECT_TRUE(bool(ogb));
    EXPECT_EQ(ogb->val, "res1");

    optional<guard> ogc(in_place);
    EXPECT_TRUE(bool(ogc));
    EXPECT_EQ(ogc->val, "");

    oga.emplace("res1");
    EXPECT_TRUE(bool(oga));
    EXPECT_EQ(oga->val, "res1");

    oga.emplace();

    EXPECT_TRUE(bool(oga));
    EXPECT_EQ(oga->val, "");

    oga = nullopt;
    EXPECT_FALSE(oga);
};


template <typename T>
static T get_value(optional<T> src = nullopt, optional<T&> dst = nullopt)
{
    T cached{};

    if (src) {
        cached = *src;

        if (dst) {
            *dst = *src;
        }
    }
    return cached;
}


TEST(optional, example_optional_arg)
{
    int iii = 0;
    iii = get_value<int>(iii, iii);
    iii = get_value<int>(iii);
    iii = get_value<int>();

    {
        optional<guard> grd1{in_place, "res1", 1};
        optional<guard> grd2;

        grd2.emplace("res2", 2);
        grd1 = nullopt;
    }
};


static std::tuple<date, date, date> get_triplet()
{
    return std::tuple<date, date, date>{date{1}, date{2}, date{3}};
}


static void run_triplet(date const&, date const&, date const&)
{}


TEST(optional, example_date)
{
    optional<date> start, mid, end;

    std::tie(start, mid, end) = get_triplet();
    run_triplet(*start, *mid, *end);
};


template <typename T>
static void unused(T&&)
{}


TEST(optional, example_conceptual_model)
{
    optional<int> oi = 0;
    optional<int> oj = 1;
    optional<int> ok = nullopt;

    oi = 1;
    oj = nullopt;
    ok = 0;

    unused(oi == nullopt);
    unused(oj == 0);
    unused(ok == 1);
};


static bool fun(std::string, optional<int> oi = nullopt)
{
    return bool(oi);
}


TEST(optional, example_converting_ctor)
{
    EXPECT_TRUE(fun("dog", 2));
    EXPECT_FALSE(fun("dog"));
    EXPECT_FALSE(fun("dog", nullopt));
};


TEST(optional, bad_comparison)
{
    optional<int> oi, oj;
    int i;
    bool b = (oi == oj);
    b = (oi >= i);
    b = (oi == i);
    unused(b);
};


TEST(optional, value_or)
{
    optional<int> oi = 1;
    int i = oi.value_or(0);
    EXPECT_EQ(i, 1);

    oi = nullopt;
    EXPECT_EQ(oi.value_or(3), 3);

    optional<std::string> os{"AAA"};
    EXPECT_EQ(os.value_or("BBB"), "AAA");
    os = {};
    EXPECT_EQ(os.value_or("BBB"), "BBB");
};


TEST(optional, reset)
{
    optional<int> oi {1};
    oi.reset();
    EXPECT_FALSE(oi);

    int i = 1;
    optional<const int&> oir {i};
    oir.reset();
    EXPECT_FALSE(oir);
};


TEST(optional, mixed_order)
{
    optional<int> on {nullopt};
    optional<int> o0 {0};
    optional<int> o1 {1};

    EXPECT_TRUE((on < 0));
    EXPECT_TRUE((on < 1));
    EXPECT_TRUE(!(o0 < 0));
    EXPECT_TRUE((o0 < 1));
    EXPECT_TRUE(!(o1 < 0));
    EXPECT_TRUE(!(o1 < 1));

    EXPECT_TRUE(!(on >= 0));
    EXPECT_TRUE(!(on >= 1));
    EXPECT_TRUE((o0 >= 0));
    EXPECT_TRUE(!(o0 >= 1));
    EXPECT_TRUE((o1 >= 0));
    EXPECT_TRUE((o1 >= 1));

    EXPECT_TRUE(!(on > 0));
    EXPECT_TRUE(!(on > 1));
    EXPECT_TRUE(!(o0 > 0));
    EXPECT_TRUE(!(o0 > 1));
    EXPECT_TRUE((o1 > 0));
    EXPECT_TRUE(!(o1 > 1));

    EXPECT_TRUE((on <= 0));
    EXPECT_TRUE((on <= 1));
    EXPECT_TRUE((o0 <= 0));
    EXPECT_TRUE((o0 <= 1));
    EXPECT_TRUE(!(o1 <= 0));
    EXPECT_TRUE((o1 <= 1));

    EXPECT_TRUE((0 > on));
    EXPECT_TRUE((1 > on));
    EXPECT_TRUE(!(0 > o0));
    EXPECT_TRUE((1 > o0));
    EXPECT_TRUE(!(0 > o1));
    EXPECT_TRUE(!(1 > o1));

    EXPECT_TRUE(!(0 <= on));
    EXPECT_TRUE(!(1 <= on));
    EXPECT_TRUE((0 <= o0));
    EXPECT_TRUE(!(1 <= o0));
    EXPECT_TRUE((0 <= o1));
    EXPECT_TRUE((1 <= o1));

    EXPECT_TRUE(!(0 < on));
    EXPECT_TRUE(!(1 < on));
    EXPECT_TRUE(!(0 < o0));
    EXPECT_TRUE(!(1 < o0));
    EXPECT_TRUE((0 < o1));
    EXPECT_TRUE(!(1 < o1));

    EXPECT_TRUE((0 >= on));
    EXPECT_TRUE((1 >= on));
    EXPECT_TRUE((0 >= o0));
    EXPECT_TRUE((1 >= o0));
    EXPECT_TRUE(!(0 >= o1));
    EXPECT_TRUE((1 >= o1));
};


TEST(optional, hash)
{
    using on1 = optional<int>;
    using on2 = optional<int&>;
    using rt1 = decltype(hash<on1>()(std::declval<on1>()));
    using rt2 = decltype(hash<on2>()(std::declval<on2>()));

    static_assert(std::is_same<rt1, size_t>::value, "");
    static_assert(std::is_same<rt2, size_t>::value, "");
}
