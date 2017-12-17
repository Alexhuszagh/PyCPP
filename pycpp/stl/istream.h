//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <istream> aliases.
 */

#pragma once

#include <pycpp/stl/ios.h>
#include <pycpp/stl/streambuf.h>
#include <istream>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::ws;
using std::basic_istream;
using std::basic_iostream;
using std::istream;
using std::wistream;
using std::iostream;
using std::wiostream;

PYCPP_END_NAMESPACE
