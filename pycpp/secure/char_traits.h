//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure char_traits implementation.
 *
 *  Provides wrappers assign(), copy(), and move() to call
 *  memory-safe routines.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename Char>
using secure_char_traits = std::char_traits<Char>;

PYCPP_END_NAMESPACE
