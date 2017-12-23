//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Total ordering implemented via less and equal_to.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

namespace ordering
{

// FUNCTIONS
// ---------

/**
 *  \brief `!=` implemented as `==`.
 */
template <typename T>
static bool not_equal_to(const T& lhs, const T& rhs) noexcept(noexcept(lhs == rhs))
{
    return !(lhs == rhs);
}


/**
 *  \brief `<=` implemented as `<`.
 */
template <typename T>
static bool less_equal(const T& lhs, const T& rhs) noexcept(noexcept(lhs < rhs))
{
    return !(rhs < lhs);
}


/**
 *  \brief `>` implemented as `<`.
 */
template <typename T>
static bool greater(const T& lhs, const T& rhs) noexcept(noexcept(lhs < rhs))
{
    return rhs < lhs;
}


/**
 *  \brief `>=` implemented as `<`.
 */
template <typename T>
static bool greater_equal(const T& lhs, const T& rhs) noexcept(noexcept(lhs < rhs))
{
    return !(lhs < rhs);
}

}   /* ordering */

PYCPP_END_NAMESPACE
