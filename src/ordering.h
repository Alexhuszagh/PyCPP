//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Total ordering implemented via std::less and std::equal_to.
 */

#pragma once


// FUNCTIONS
// ---------


/** \brief `!=` implemented as `==`.
 */
template <typename T>
bool not_equal_to(const T& left, const T& right)
{
    return !(left == right);
}


/** \brief `<=` implemented as `<`.
 */
template <typename T>
bool less_equal(const T& left, const T& right)
{
    return !(right < left);
}


/** \brief `>` implemented as `<`.
 */
template <typename T>
bool greater(const T& left, const T& right)
{
    return right < left;
}


/** \brief `>=` implemented as `<`.
 */
template <typename T>
bool greater_equal(const T& left, const T& right)
{
    return !(left < right);
}
