//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief String manipulation utilities.
 */

#pragma once

#include <string>
#include <vector>

// FUNCTIONS
// ---------

/**
 *  \brief Split characters by delimiters into dst.
 */
std::vector<std::string> split(const std::string& str, const std::string& sep);
