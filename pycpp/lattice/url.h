//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP URL object.
 */

#pragma once

#include <pycpp/config.h>
#include <string>
#include <initializer_list>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief URL class instance.
 */
struct url_t: std::string
{
    url_t() = default;
    url_t(const url_t &other) = default;
    url_t & operator=(const url_t&) = default;
    url_t(url_t&&) = default;
    url_t & operator=(url_t&&) = default;

    url_t(const char *cstring);
    url_t(const char *array, size_t size);
    url_t(const std::string &string);
    url_t(std::initializer_list<char> list);

    // GETTERS
    std::string service() const noexcept;
    std::string host() const noexcept;
    std::string path() const noexcept;
    std::string directory() const noexcept;
    std::string file() const noexcept;

    // SETTERS
    void set_service(const std::string &service);
    void set_host(const std::string &host);
    void set_path(const std::string &path);
    void set_directory(const std::string &directory);
    void set_file(const std::string &file);

    // PROPERTIES
    bool relative() const noexcept;
    bool absolute() const noexcept;

    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
