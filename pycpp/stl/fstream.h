//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <fstream> aliases.
 */

#pragma once

#include <pycpp/stl/detail/fstream.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Don't include wfilebuf, since we don't
// provide a wide-stream API.
using std::basic_filebuf;
using std::filebuf;

PYCPP_END_NAMESPACE
