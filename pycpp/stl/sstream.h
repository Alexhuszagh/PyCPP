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
#include <sstream>

// TODO: need a competing series of stringstream implementations...
// Lols

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::swap;
using std::basic_stringbuf;
using std::basic_istringstream;
using std::basic_ostringstream;
using std::basic_stringstream;
using std::stringbuf;
using std::wstringbuf;
using std::istringstream;
using std::wistringstream;
using std::ostringstream;
using std::wostringstream;
using std::stringstream;
using std::wstringstream;

PYCPP_END_NAMESPACE
