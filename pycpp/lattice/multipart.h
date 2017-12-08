//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Multipart requests (file uploads).
 */

#pragma once

#include <pycpp/config.h>
#include <deque>
#include <memory>
#include <string>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// FUNCTIONS
// ---------

/**
 *  \brief Get boundary value for MIME message.
 */
std::string get_boundary();

// OBJECTS
// -------


/**
 *  \brief Base class for a multipart item.
 */
struct part_value_t
{
    std::string filename;
    std::string content_type_;

    part_value_t() = default;
    part_value_t(const part_value_t &other) = default;
    part_value_t & operator=(const part_value_t&) = default;
    part_value_t(part_value_t&&) = default;
    part_value_t & operator=(part_value_t&&) = default;

    part_value_t(const char* filename, const char* content_type);
    part_value_t(std::string&& filename, std::string&& content_type);
    part_value_t(const string_view& filename, const string_view& content_type);

    std::string basename() const;
    std::string name() const;
    string_view content_type() const;
    virtual std::string string() const;
};


/**
 *  \brief Data for a multipart message from file.
 */
struct file_value_t: part_value_t
{
    using part_value_t::part_value_t;

    std::string buffer() const;
    std::string string() const override;
};


/**
 *  \brief Data for a multipart message from buffer.
 */
struct buffer_value_t: part_value_t
{
    std::string buffer_;

    buffer_value_t() = default;
    buffer_value_t(const buffer_value_t &other) = default;
    buffer_value_t & operator=(const buffer_value_t&) = default;
    buffer_value_t(buffer_value_t&&) = default;
    buffer_value_t & operator=(buffer_value_t&&) = default;

    buffer_value_t(const char* filename,
                   const char* buffer,
                   const char* content_type = "");
    buffer_value_t(std::string&& filename,
                   std::string&& buffer,
                   std::string&& content_type);
    buffer_value_t(const string_view& filename,
                   const string_view& buffer,
                   const string_view& content_type);

    std::string buffer() const;
    std::string string() const override;
};

}   /* detail */

// ALIAS
// -----

using part_value_t = detail::part_value_t;
using file_value_t = detail::file_value_t;
using buffer_value_t = detail::buffer_value_t;

using part_ptr_t = std::shared_ptr<part_value_t>;
using file_ptr_t = std::shared_ptr<file_value_t>;
using buffer_ptr_t = std::shared_ptr<buffer_value_t>;

// HELPERS
// -------

template <typename... Ts>
file_ptr_t create_file(Ts&&... ts)
{
    return std::make_shared<file_value_t>(std::forward<Ts>(ts)...);
}


template <typename... Ts>
buffer_ptr_t create_buffer(Ts&&... ts)
{
    return std::make_shared<buffer_value_t>(std::forward<Ts>(ts)...);
}

// OBJECTS
// -------


/**
 *  \brief Data for multipart requests.
 */
class multipart_t: public std::deque<part_ptr_t>
{
public:
    using base = std::deque<part_ptr_t>;
    using base::base;

    void add(const part_ptr_t& part);
    void add(part_ptr_t&& part);
    string_view boundary() const;
    std::string header() const;

    std::string string() const;
    explicit operator bool() const;

private:
    std::string boundary_ = detail::get_boundary();
};

PYCPP_END_NAMESPACE
