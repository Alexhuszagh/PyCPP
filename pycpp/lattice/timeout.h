//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Timeout for requests.
 */

#pragma once

#include <pycpp/stl/chrono.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/stdexcept.h>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// OBJECTS
// -------


/**
 *  \brief Timeout for a request object.
 */
class timeout_t
{
public:
    timeout_t() = default;
    timeout_t(const timeout_t &other) = default;
    timeout_t & operator=(const timeout_t&) = default;
    timeout_t(timeout_t&&) = default;
    timeout_t & operator=(timeout_t&&) = default;

    timeout_t(const std::chrono::milliseconds &timeout);
    timeout_t(long ms);

    long seconds() const;
    long milliseconds() const;
    long microseconds() const;

    explicit operator bool() const;

protected:
    std::chrono::milliseconds timeout = std::chrono::milliseconds(0);

    template <typename Duration>
    void check_limits(const Duration &duration) const;
};


// IMPLEMENTATION
// --------------


/**
 *  \brief Throw overflow or underflow if outside of numeric limits.
 */
template <typename Duration>
void timeout_t::check_limits(const Duration &duration) const
{
    if (duration.count() > std::numeric_limits<long>::max()) {
        throw std::overflow_error("Timeout value overflow.\n");
    }
    if (duration.count() < std::numeric_limits<long>::min()) {
        throw std::overflow_error("Timeout value underflow.\n");
    }
}

PYCPP_END_NAMESPACE
