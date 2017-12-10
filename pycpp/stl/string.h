//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL string alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename Char,
    typename Traits = std::char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_string = std::string<Char, Traits, Alloc>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

PYCPP_END_NAMESPACE
