//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform getline implementation.
 */

#pragma once

#include <istream>


// FUNCTIONS
// ---------

/** \brief Getline implementation for files from all platforms.
 *
 *  Getline implementation which stops at "\r", "\n", or both.
 */
std::istream& getline(std::istream& stream, std::string& line);
