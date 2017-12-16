//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <iostream> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/istream.h>
#include <pycpp/stl/ostream.h>
#include <pycpp/stl/streambuf.h>
#include <iostream>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::cin;
using std::cout;
using std::cerr;
using std::clog;
using std::wcin;
using std::wcout;
using std::wcerr;
using std::wclog;

PYCPP_END_NAMESPACE
