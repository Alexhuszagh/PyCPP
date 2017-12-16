//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <ostream> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/ios.h>
#include <ostream>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::ends;
using std::flush;
using std::endl;

template <
    typename Char,
    typename Traits = std::char_traits<Char>
>
using basic_ostream = std::basic_ostream<Char, Traits>;

using ostream = basic_ostream<char>;
using wostream = basic_ostream<wchar_t>;

PYCPP_END_NAMESPACE
