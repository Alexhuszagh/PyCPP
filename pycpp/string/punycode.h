//  :copyright: (c) 2011 by Ben Noordhuis <info@bnoordhuis.nl>.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Punycode conversions.
 */

#pragma once

#include <pycpp/config.h>
#include <functional>
#include <string>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using punycode_lowlevel_callback = std::function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen)
>;

using punycode_highlevel_callback = std::function<std::string(const std::string&)>;

// FUNCTIONS
// ---------

/**
 *  \brief Convert UTF8-encoded data to Punycode.
 */
void utf8_to_punycode(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF8-encoded data to Punycode.
 */
std::string utf8_to_punycode(const std::string &str);

/**
 *  \brief Convert UTF16-encoded data to Punycode.
 */
void utf16_to_punycode(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF16-encoded data to Punycode.
 */
std::string utf16_to_punycode(const std::string &str);

/**
 *  \brief Convert UTF32-encoded data to Punycode.
 */
void utf32_to_punycode(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF32-encoded data to Punycode.
 */
std::string utf32_to_punycode(const std::string &str);

/**
 *  \brief Convert Punycode to UTF-8.
 */
void punycode_to_utf8(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert Punycode to UTF-8.
 */
std::string punycode_to_utf8(const std::string &str);

/**
 *  \brief Convert Punycode to UTF-16.
 */
void punycode_to_utf16(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert Punycode to UTF-16.
 */
std::string punycode_to_utf16(const std::string &str);

/**
 *  \brief Convert Punycode to UTF-32.
 */
void punycode_to_utf32(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert Punycode to UTF-32.
 */
std::string punycode_to_utf32(const std::string &str);

PYCPP_END_NAMESPACE
