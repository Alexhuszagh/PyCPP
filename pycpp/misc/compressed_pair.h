//  :copyright: (c) 2000 Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license:  Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Pair that compressed empty members.
 */

#pragma once

#include <pycpp/config.h>
#include <warnings/push.h>
#include <warnings/assign-could-not-be-generated.h>
#include <algorithm>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T1, typename T2>
class compressed_pair;

namespace compressed_detail
{
// DECLARATIONS
// ------------

// Helpers to detect the proper class specialization
template <typename T1, typename T2, bool IsSame, bool FirstEmpty, bool SecondEmpty>
struct compressed_pair_switch_helper;

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, false, false>
{
    static const int value = 0;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, true, true>
{
    static const int value = 3;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, true, false>
{
    static const int value = 1;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, false, true>
{
    static const int value = 2;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, true, true, true>
{
    static const int value = 4;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, true, false, false>
{
    static const int value = 5;
};

// Uses the helper to get the value for the proper specialization
template <typename T1, typename T2>
using compressed_switch = compressed_pair_switch_helper<
    T1,
    T2,
    std::is_same<typename std::remove_cv<T1>::type, typename std::remove_cv<T2>::type>::value,
    std::is_empty<T1>::value,
    std::is_empty<T2>::value
>;

template <typename T1, typename T2, int Version>
class compressed_pair_impl;

// 0    derive from neither
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 0>
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(second_const_reference y);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);
    compressed_pair_impl(second_type&& y);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);

private:
    first_type first_;
    second_type second_;
};


// 1    derive from T1
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 1>: protected std::remove_cv<T1>::type
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(second_const_reference y);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);
    compressed_pair_impl(second_type&& y);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);

private:
    second_type second_;
};


// 2    derive from T2
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 2>: protected std::remove_cv<T2>::type
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(second_const_reference y);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);
    compressed_pair_impl(second_type&& y);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);

private:
    first_type first_;
};


// 3    derive from T1 and T2
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 3>:
    protected std::remove_cv<T1>::type,
    protected std::remove_cv<T2>::type
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(second_const_reference y);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);
    compressed_pair_impl(second_type&& y);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);
};


// 4    T1 == T2, T1 and T2 both empty
//  Originally, the pair did not store an instance of T2 at all
//  but that led to problems beause it meant &x.first() == &x.second()
//  which is not true for any other kind of pair, so now we store an
//  instance of T2 just in case the user is relying on first() and
//  second() returning different objects (albeit both empty).
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 4>: protected std::remove_cv<T1>::type
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);

private:
    second_type second_;
};

// 5    T1 == T2, both not empty
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 5>
{
public:
    // MEMBER TYPES
    // ------------
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // MEMBER FUNCTIONS
    // ----------------
    compressed_pair_impl();
    compressed_pair_impl(first_const_reference x, second_const_reference y);
    compressed_pair_impl(first_const_reference x);
    compressed_pair_impl(first_type&& x, second_type&& y);
    compressed_pair_impl(first_type&& x);

    // ELEMENT ACCESS
    first_reference first();
    first_const_reference first() const;
    second_reference second();
    second_const_reference second() const;

    // MODIFIERS
    void swap(compressed_pair<T1, T2>& y);

private:
    first_type first_;
    second_type second_;
};

// Helper to select the proper compressed pair
template <typename T1, typename T2>
using compressed_pair_selector = compressed_pair_impl<
    T1,
    T2,
    compressed_switch<T1, T2>::value
>;

// IMPLEMENTATION
// --------------

// 0    derive from neither

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    first_(x),
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(first_const_reference x):
    first_(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(second_const_reference y):
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(first_type&& x, second_type&& y):
    first_(std::forward<first_type>(x)),
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(first_type&& x):
    first_(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 0>::compressed_pair_impl(second_type&& y):
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 0>::first() -> first_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 0>::first() const -> first_const_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 0>::second() -> second_reference
{
    return second_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 0>::second() const -> second_const_reference
{
    return second_;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 0>::swap(compressed_pair<T1, T2>& y)
{
    std::swap(first_, y.first());
    std::swap(second_, y.second());
}

// 1    derive from T1

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    first_type(x),
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(first_const_reference x):
    first_type(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(second_const_reference y):
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(first_type&& x, second_type&& y):
    first_type(std::forward<first_type>(x)),
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(first_type&& x):
    first_type(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 1>::compressed_pair_impl(second_type&& y):
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 1>::first() -> first_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 1>::first() const -> first_const_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 1>::second() -> second_reference
{
    return second_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 1>::second() const -> second_const_reference
{
    return second_;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 1>::swap(compressed_pair<T1, T2>& y)
{
    // no need to swap empty base class:
    std::swap(second_, y.second());
}

// 2    derive from T2

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    second_type(y),
    first_(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(first_const_reference x):
    first_(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(second_const_reference y):
    second_type(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(first_type&& x, second_type&& y):
    second_type(std::forward<second_type>(y)),
    first_(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(first_type&& x):
    first_(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 2>::compressed_pair_impl(second_type&& y):
    second_type(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 2>::first() -> first_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 2>::first() const -> first_const_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 2>::second() -> second_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 2>::second() const -> second_const_reference
{
    return *this;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 2>::swap(compressed_pair<T1, T2>& y)
{
    // no need to swap empty base class:
    std::swap(first_, y.first());
}

// 3    derive from T1 and T2

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    first_type(x),
    second_type(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(first_const_reference x):
    first_type(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(second_const_reference y):
    second_type(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(first_type&& x, second_type&& y):
    first_type(std::forward<first_type>(x)),
    second_type(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(first_type&& x):
    first_type(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 3>::compressed_pair_impl(second_type&& y):
    second_type(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 3>::first() -> first_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 3>::first() const -> first_const_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 3>::second() -> second_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 3>::second() const -> second_const_reference
{
    return *this;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 3>::swap(compressed_pair<T1, T2>& y)
{
    // no need to swap empty base class:
}

// 4    T1 == T2, T1 and T2 both empty

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 4>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 4>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    first_type(x),
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 4>::compressed_pair_impl(first_const_reference x):
    first_type(x),
    second_(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 4>::compressed_pair_impl(first_type&& x, second_type&& y):
    first_type(std::forward<first_type>(x)),
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 4>::compressed_pair_impl(first_type&& x):
    first_type(x),
    second_(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 4>::first() -> first_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 4>::first() const -> first_const_reference
{
    return *this;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 4>::second() -> second_reference
{
    return second_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 4>::second() const -> second_const_reference
{
    return second_;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 4>::swap(compressed_pair<T1, T2>& y)
{
    // no need to swap empty base class:
}

// 5    T1 == T2, both not empty

template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 5>::compressed_pair_impl()
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 5>::compressed_pair_impl(first_const_reference x, second_const_reference y):
    first_(x),
    second_(y)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 5>::compressed_pair_impl(first_const_reference x):
    first_(x),
    second_(x)
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 5>::compressed_pair_impl(first_type&& x, second_type&& y):
    first_(std::forward<first_type>(x)),
    second_(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair_impl<T1, T2, 5>::compressed_pair_impl(first_type&& x):
    first_(x),
    second_(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 5>::first() -> first_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 5>::first() const -> first_const_reference
{
    return first_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 5>::second() -> second_reference
{
    return second_;
}


template <typename T1, typename T2>
auto compressed_pair_impl<T1, T2, 5>::second() const -> second_const_reference
{
    return second_;
}


template <typename T1, typename T2>
void compressed_pair_impl<T1, T2, 5>::swap(compressed_pair<T1, T2>& y)
{
    std::swap(first_, y.first());
    std::swap(second_, y.second());
}

}   /* compressed_detail */

// DECLARATION
// -----------

template <typename T1, typename T2>
class compressed_pair: private compressed_detail::compressed_pair_selector<T1, T2>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = compressed_pair<T1, T2>;
    using base_t = compressed_detail::compressed_pair_selector<T1, T2>;
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    compressed_pair();
    compressed_pair(first_const_reference x, second_const_reference y);
    explicit compressed_pair(first_const_reference x);
    explicit compressed_pair(second_const_reference y);
    compressed_pair(first_type&& x, second_type&& y);
    explicit compressed_pair(first_type&& x);
    explicit compressed_pair(second_type&& y);

    // ELEMENT ACCESS
   first_reference first();
   first_const_reference first() const;
   second_reference second();
   second_const_reference second() const;

    // MODIFIERS
   void swap(compressed_pair& y);
};


template <typename T>
class compressed_pair<T, T>: private compressed_detail::compressed_pair_selector<T, T>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = compressed_pair<T, T>;
    using base_t = compressed_detail::compressed_pair_selector<T, T>;
    using first_type = T;
    using second_type = T;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    compressed_pair();
    compressed_pair(first_const_reference x, second_const_reference y);
    explicit compressed_pair(first_const_reference x);
    compressed_pair(first_type&& x, second_type&& y);
    explicit compressed_pair(first_type&& x);

    // ELEMENT ACCESS
   first_reference first();
   first_const_reference first() const;
   second_reference second();
   second_const_reference second() const;

    // MODIFIERS
   void swap(compressed_pair& y);
};

// IMPLEMENTATION
// --------------


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair():
    base_t()
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(first_const_reference x, second_const_reference y):
    base_t(x, y)
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(first_const_reference x):
    base_t(x)
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(second_const_reference y):
    base_t(y)
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(first_type&& x, second_type&& y):
    base_t(std::forward<first_type>(x), std::forward<second_type>(y))
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(first_type&& x):
    base_t(std::forward<first_type>(x))
{}


template <typename T1, typename T2>
compressed_pair<T1, T2>::compressed_pair(second_type&& y):
    base_t(std::forward<second_type>(y))
{}


template <typename T1, typename T2>
auto compressed_pair<T1, T2>::first() -> first_reference
{
    return base_t::first();
}


template <typename T1, typename T2>
auto compressed_pair<T1, T2>::first() const -> first_const_reference
{
    return base_t::first();
}


template <typename T1, typename T2>
auto compressed_pair<T1, T2>::second() -> second_reference
{
    return base_t::second();
}


template <typename T1, typename T2>
auto compressed_pair<T1, T2>::second() const -> second_const_reference
{
    return base_t::second();
}


template <typename T1, typename T2>
void compressed_pair<T1, T2>::swap(compressed_pair<T1, T2>& y)
{
    base_t::swap(y);
}


template <typename T>
compressed_pair<T, T>::compressed_pair():
    base_t()
{}


template <typename T>
compressed_pair<T, T>::compressed_pair(first_const_reference x, second_const_reference y):
    base_t(x, y)
{}


template <typename T>
compressed_pair<T, T>::compressed_pair(first_const_reference x):
    base_t(x)
{}


template <typename T>
compressed_pair<T, T>::compressed_pair(first_type&& x, second_type&& y):
    base_t(std::forward<first_type>(x), std::forward<second_type>(y))
{}


template <typename T>
compressed_pair<T, T>::compressed_pair(first_type&& x):
    base_t(std::forward<first_type>(x))
{}


template <typename T>
auto compressed_pair<T, T>::first() -> first_reference
{
    return base_t::first();
}


template <typename T>
auto compressed_pair<T, T>::first() const -> first_const_reference
{
    return base_t::first();
}


template <typename T>
auto compressed_pair<T, T>::second() -> second_reference
{
    return base_t::second();
}


template <typename T>
auto compressed_pair<T, T>::second() const -> second_const_reference
{
    return base_t::second();
}


template <typename T>
void compressed_pair<T, T>::swap(compressed_pair<T, T>& y)
{
    base_t::swap(y);
}


template <typename T1, typename T2>
inline void swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y)
{
   x.swap(y);
}

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
