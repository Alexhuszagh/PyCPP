//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform getline implementation.
 */

#pragma once

#include <pycpp/stl/istream.h>
#include <pycpp/stl/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Getline implementation for files from all platforms.
 *
 *  Getline implementation which stops at "\r", "\n", or both.
 *
 *  \param stream               Stream to get characters from
 *  \param line                 Pointer to the string to return
 *  \param initial_size         Estimated size of the line for allocation
 */
istream& getline(istream& stream, string& line, size_t initial_size = SIZE_MAX);

PYCPP_END_NAMESPACE
