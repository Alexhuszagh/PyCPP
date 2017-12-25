//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cookie support for persistent sessions.
 */

#pragma once

#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/map.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Cookies to store user data for persistent sessions.
 */
struct cookies_t: map<string, string>
{
    cookies_t() = default;
    cookies_t(const cookies_t&) = default;
    cookies_t& operator=(const cookies_t&) = default;
    cookies_t(cookies_t&&) = default;
    cookies_t& operator=(cookies_t&&) = default;
    cookies_t(initializer_list<typename cookies_t::value_type> list);

    string encode() const;
    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
