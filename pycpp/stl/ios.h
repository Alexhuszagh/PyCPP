//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <ios> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <ios>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename State>
using fpos = std::fpos<State>;

using ios_base = std::ios_base;
using streamsize = std::streamsize;
using streampos = std::streampos;
using wstreampos = std::wstreampos;
using streamoff = std::streamoff;

PYCPP_END_NAMESPACE
