//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP URL object.
 */

#pragma once

#include <pycpp/string/string.h>
#include <pycpp/stl/initializer_list.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct punycode_idna_t;
struct unicode_idna_t;
struct url_t;

// OBJECTS
// -------


/**
 *  \brief Base class for a URL.
 */
struct url_impl_t: std::string
{
    url_impl_t() = default;
    url_impl_t(const url_impl_t&) = default;
    url_impl_t & operator=(const url_impl_t&) = default;
    url_impl_t(url_impl_t&&) = default;
    url_impl_t & operator=(url_impl_t&&) = default;
    using std::string::string;

    // GETTERS
    std::string service() const noexcept;
    std::string host() const noexcept;
    std::string path() const noexcept;
    std::string directory() const noexcept;
    std::string file() const noexcept;

    // PROPERTIES
    bool relative() const noexcept;
    bool absolute() const noexcept;

    explicit operator bool() const;
};


/**
 *  \brief Punycode-encoded international domain name.
 */
struct punycode_idna_t: url_impl_t
{
    punycode_idna_t() = default;
    punycode_idna_t(const punycode_idna_t&) = default;
    punycode_idna_t & operator=(const punycode_idna_t&) = default;
    punycode_idna_t(punycode_idna_t&&) = default;
    punycode_idna_t & operator=(punycode_idna_t&&) = default;

    punycode_idna_t(const char*, size_t);
    punycode_idna_t(const string_wrapper&);
    punycode_idna_t(initializer_list<char> list);

    // SETTERS
    void set_service(const string_wrapper& service);
    void set_host(const string_wrapper& host);
    void set_path(const string_wrapper& path);
    void set_directory(const string_wrapper& directory);
    void set_file(const string_wrapper& file);

    unicode_idna_t to_unicode() const;
};


/**
 *  \brief Unicode-encoded international domain name.
 */
struct unicode_idna_t: url_impl_t
{
    unicode_idna_t() = default;
    unicode_idna_t(const unicode_idna_t &other) = default;
    unicode_idna_t & operator=(const unicode_idna_t&) = default;
    unicode_idna_t(unicode_idna_t&&) = default;
    unicode_idna_t & operator=(unicode_idna_t&&) = default;

    unicode_idna_t(const char*, size_t);
    unicode_idna_t(const string_wrapper&);
    unicode_idna_t(initializer_list<char> list);

    // SETTERS
    void set_service(const string_wrapper& service);
    void set_host(const string_wrapper& host);
    void set_path(const string_wrapper& path);
    void set_directory(const string_wrapper& directory);
    void set_file(const string_wrapper& file);

    punycode_idna_t to_punycode() const;
};


/**
 *  \brief URL class instance.
 */
struct url_t: punycode_idna_t
{
    using punycode_idna_t::punycode_idna_t;
};


PYCPP_END_NAMESPACE
