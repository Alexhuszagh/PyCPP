//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <istream> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/ios.h>
#include <istream>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::ws;

template <
    typename Char,
    typename Traits = std::char_traits<Char>
>
using basic_istream = std::basic_istream<Char, Traits>;

template <
    typename Char,
    typename Traits = std::char_traits<Char>
>
using basic_iostream = std::basic_iostream<Char, Traits>;

using istream = basic_istream<char>;
using wistream = basic_istream<wchar_t>;
using iostream = basic_iostream<char>;
using wiostream = basic_iostream<wchar_t>;

PYCPP_END_NAMESPACE