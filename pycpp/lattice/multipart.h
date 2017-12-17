//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Multipart requests (file uploads).
 */

#pragma once

#include <pycpp/stl/deque.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/memory.h>
#include <pycpp/string/string.h>

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
    part_value_t(const string_wrapper& filename, const string_wrapper& content_type);

    std::string basename() const;
    std::string name() const;
    string_wrapper content_type() const;
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
    buffer_value_t(const string_wrapper& filename,
                   const string_wrapper& buffer,
                   const string_wrapper& content_type);

    std::string buffer() const;
    std::string string() const override;
};

}   /* detail */

// ALIAS
// -----

using part_value_t = detail::part_value_t;
using file_value_t = detail::file_value_t;
using buffer_value_t = detail::buffer_value_t;

using part_ptr_t = shared_ptr<part_value_t>;
using file_ptr_t = shared_ptr<file_value_t>;
using buffer_ptr_t = shared_ptr<buffer_value_t>;

// HELPERS
// -------

template <typename... Ts>
file_ptr_t create_file(Ts&&... ts)
{
    return make_shared<file_value_t>(forward<Ts>(ts)...);
}


template <typename... Ts>
buffer_ptr_t create_buffer(Ts&&... ts)
{
    return make_shared<buffer_value_t>(forward<Ts>(ts)...);
}

// OBJECTS
// -------


/**
 *  \brief Data for multipart requests.
 */
class multipart_t: public deque<part_ptr_t>
{
public:
    multipart_t() = default;
    multipart_t(const multipart_t&) = default;
    multipart_t& operator=(const multipart_t&) = default;
    multipart_t(multipart_t&&) = default;
    multipart_t& operator=(multipart_t&&) = default;
    multipart_t(initializer_list<typename multipart_t::value_type> list);

    void add(const part_ptr_t& part);
    void add(part_ptr_t&& part);
    string_wrapper boundary() const;
    std::string header() const;

    std::string string() const;
    explicit operator bool() const;

private:
    std::string boundary_ = detail::get_boundary();
};

PYCPP_END_NAMESPACE
