//  :copyright: (c) 2011 by Ben Noordhuis <info@bnoordhuis.nl>.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Punycode conversions.
 */

#pragma once

#include <string>


// FUNCTIONS
// ---------

/** \brief Convert UTF8-encoded data to Punycode.
 */
std::string utf8_to_punycode(const std::string &string);

/** \brief Convert UTF16-encoded data to Punycode.
 */
std::string utf16_to_punycode(const std::string &string);

/** \brief Convert UTF32-encoded data to Punycode.
 */
std::string utf32_to_punycode(const std::string &string);

/** \brief Convert Punycode to UTF-8.
 */
std::string punycode_to_utf8(const std::string &string);

/** \brief Convert Punycode to UTF-16.
 */
std::string punycode_to_utf16(const std::string &string);

/** \brief Convert Punycode to UTF-32.
 */
std::string punycode_to_utf32(const std::string &string);
