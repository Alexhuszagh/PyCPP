//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <ios> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <ios>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::fpos;
using std::streampos;
using std::wstreampos;
using std::ios_base;
using std::basic_ios;
using std::ios;
using std::wios;
using std::streamoff;
using std::streamsize;
using std::boolalpha;
using std::noboolalpha;
using std::showbase;
using std::noshowbase;
using std::showpoint;
using std::noshowpoint;
using std::showpos;
using std::noshowpos;
using std::skipws;
using std::noskipws;
using std::uppercase;
using std::nouppercase;
using std::unitbuf;
using std::nounitbuf;
using std::internal;
using std::left;
using std::right;
using std::dec;
using std::hex;
using std::oct;
using std::fixed;
using std::scientific;

#if !defined(CPP11_PARTIAL_IOS)

using std::io_errc;
using std::is_error_code_enum;
using std::iostream_category;
using std::make_error_code;
using std::make_error_condition;
using std::hexfloat;
using std::defaultfloat;

#endif          // !CPP11_PARTIAL_IOS

PYCPP_END_NAMESPACE
