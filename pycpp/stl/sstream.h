//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <sstream> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/istream.h>
#include <pycpp/stl/ostream.h>
#include <pycpp/stl/string.h>
#include <sstream>

// TODO: need a competing series of stringstream implementations...
// Lols

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::swap;

template <
    typename Char,
    typename Traits = char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_stringbuf = std::basic_stringbuf<Char, Traits, Alloc>;

template <
    typename Char,
    typename Traits = char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_istringstream = std::basic_istringstream<Char, Traits, Alloc>;

template <
    typename Char,
    typename Traits = char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_ostringstream = std::basic_ostringstream<Char, Traits, Alloc>;

template <
    typename Char,
    typename Traits = char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_stringstream = std::basic_stringstream<Char, Traits, Alloc>;

using stringbuf = basic_stringbuf<char>;
using wstringbuf = basic_stringbuf<wchar_t>;
using istringstream = basic_istringstream<char>;
using wistringstream = basic_istringstream<wchar_t>;
using ostringstream = basic_ostringstream<char>;
using wostringstream = basic_ostringstream<wchar_t>;
using stringstream = basic_stringstream<char>;
using wstringstream = basic_stringstream<wchar_t>;

PYCPP_END_NAMESPACE
