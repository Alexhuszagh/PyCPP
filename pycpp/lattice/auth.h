//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Authentification for requests.
 */

#pragma once

#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Authorization string for requests.
 */
struct authentication_t
{
    string username;
    string password;

    authentication_t() = default;
    authentication_t(const authentication_t&) = default;
    authentication_t & operator=(const authentication_t&) = default;
    authentication_t(authentication_t&&) = default;
    authentication_t & operator=(authentication_t&&) = default;

    authentication_t(const char* username, const char* password);
    authentication_t(string&& username, string&& password);
    authentication_t(const string_wrapper& username, const string_wrapper& password);

    const string str() const noexcept;
    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
