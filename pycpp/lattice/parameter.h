//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP request parameters.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief request_t parameter.
 */
struct parameter_t
{
    std::string key;
    std::string value;

    parameter_t() = default;
    parameter_t(const parameter_t &other) = default;
    parameter_t & operator=(const parameter_t&) = default;
    parameter_t(parameter_t&&) = default;
    parameter_t & operator=(parameter_t&&) = default;

    parameter_t(std::string&& key, std::string&& value);
    parameter_t(const std::string& key, const std::string& value);
};


/**
 *  \brief HTTP parameters object.
 */
struct parameters_t: std::string
{
    typedef std::string base;

    parameters_t() = default;
    parameters_t(const parameters_t &other) = default;
    parameters_t & operator=(const parameters_t&) = default;
    parameters_t(parameters_t&&) = default;
    parameters_t & operator=(parameters_t&&) = default;

    using base::base;
    parameters_t(const std::initializer_list<parameter_t>& parameters);

    parameters_t & add(const parameter_t &parameter);

    std::string get() const;
    const std::string& post() const;
    explicit operator bool() const;
};


/**
 *  \brief HTTP parameters object for a POST request.
 */
struct body_t: parameters_t
{
    using parameters_t::parameters_t;
};


/**
 *  \brief HTTP parameters object for a POST request.
 */
struct payload_t: parameters_t
{
    using parameters_t::parameters_t;
};

PYCPP_END_NAMESPACE
