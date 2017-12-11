//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cookie support for persistent sessions.
 */

#pragma once

#include <pycpp/stl/map.h>
#include <pycpp/stl/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Cookies to store user data for persistent sessions.
 */
struct cookies_t: std::map<std::string, std::string>
{
    typedef std::map<std::string, std::string> base;
    using base::base;

    std::string encode() const;
    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
