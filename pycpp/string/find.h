//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Generic string find/rfind implementations.
 */

#pragma once

#include <pycpp/config.h>
#include <algorithm>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


template <typename Iter1, typename Iter2>
Iter1 find(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept
{
    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length >= sublen; --length, ++first) {
        if (std::equal(substr_first, substr_last, first)) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 find_of(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length, ++first) {
        if (std::any_of(substr_first, substr_last, [first](Char c) {
            return *first == c;
        })) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 find_not_of(Iter1 first, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length, ++first) {
        if (std::none_of(substr_first, substr_last, [first](Char c) {
            return *first == c;
        })) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 rfind(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    last -= sublen;
    for (; length >= sublen; --length) {
        if (std::equal(substr_first, substr_last, --last)) {
            return last;
        }
    }

    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 rfind_of(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length) {
        --last;
        if (std::any_of(substr_first, substr_last, [last](Char c) {
            return *last == c;
        })) {
            return last;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 rfind_not_of(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length) {
        --last;
        if (std::none_of(substr_first, substr_last, [last](Char c) {
            return *last == c;
        })) {
            return last;
        }
    }
    return nullptr;
}

PYCPP_END_NAMESPACE
