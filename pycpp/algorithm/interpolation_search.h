//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Implementation of a generic interpolation sort.
 *
 *  Performs an interpolation search on a sorted range, from
 *  [first, last), and returns the iterator to the nearest item
 *  below (`lower_interpolation_bound`) or above
 *  (`upper_interpolation_bound`) the expected value.
 *
 *  For uniformly-spaced or randomly-spaced data, interpolation searches
 *  perform much better than binary searches on sorted data, with
 *  `O(log log n)` performance compared to `O(logn)`. However, with
 *  exponentially-increasing data provides an asymptotically worst-case
 *  time complexity of `O(n)` for the interpolation search, much'
 *  slower than a binary search.
 *
 *  The algorithm is loosely based off of Keith Schwarz's
 *  implementation, with significant modifications to find
 *  lower and upper bounds.
 *  Keith Schwarz's implementation may be found here:
 *      http://www.keithschwarz.com/interesting/code/?dir=interpolation-search
 */

#pragma once

#include <pycpp/config.h>
#include <iterator>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

//// LOWER
//
///**
// *
// */
//template <typename Iter, typename T, typename Compare>
//Iter lower_interpolation_bound(Iter first, Iter last, const T& value, Compare comp)
//{
//    using value_type = typename std::iterator_traits<Iter>::value_type;
//    using diff_t = typename std::iterator_traits<Iter>::difference_type;
//    static_assert(std::is_arithmetic<value_type>::value, "Interpolation search must use numeric values.");
//
//    // Empty range, no input.
//    // Return last to signify bound not found
//    if (first == last) {
//        return last;
//    }
//}
//
//
///**
// *  \brief Interpolation search using `operator<` to order the elements.
// */
//template <typename Iter, typename T, typename Compare>
//Iter lower_interpolation_bound(Iter first, Iter last, const T& value)
//{
//    return lower_interpolation_bound(first, last, value, std::less<T>());
//}
//
//
///**
// *
// */
//template <typename Iter, typename T, typename Compare>
//Iter upper_interpolation_bound(Iter first, Iter last, const T& value, Compare comp)
//{
//    using value_type = typename std::iterator_traits<Iter>::value_type;
//    using diff_t = typename std::iterator_traits<Iter>::difference_type;
//    static_assert(std::is_arithmetic<value_type>::value, "Interpolation search must use numeric values.");
//
//    // Empty range, no input.
//    // Return last to signify bound not found
//    if (first == last) {
//        return last;
//    }
//
//    /* Continue looping while the value could feasibly be in the range and
//    * the iterators haven't crossed.
//    */
//    while (*first <= value && value <= *(end - 1) && first != end) {
//        /*
//         *  Interpolate between the endpoints to guess where the element should
//         *  lie. This works by computing the range [min, max] of the elements,
//         *  then seeing what fraction of the way up elem is.
//         */
//        double interpolation = (double(value) - *first) / (double(*(end - 1)) - double(*first));
//
//        /* Scale this position to an index by multiplying by the number of elements
//         * in the range by the fraction up to search.
//         */
//        Iter mid = first + diff_t(interpolation * (double(end - first) - 1));
//
//        // TODO: need to consider the logic....
//
//        /* Apply standard binary search logic at this point.  If we found the element,
//         * we're done.
//         */
//        if (*mid == value) {
//            return mid;
//        }
//        /* Otherwise, if the element is smaller than what we're looking for, look
//         * to the right.
//         */
//        else if (*mid < elem) first = mid + 1;
//        /* Otherwise, look to the right. */
//        else end = mid;
//    }
//
//    return last;
//}
//
//
///**
// *  \brief Interpolation search using `operator<` to order the elements.
// */
//template <typename Iter, typename T, typename Compare>
//Iter upper_interpolation_bound(Iter first, Iter last, const T& value)
//{
//    return upper_interpolation_bound(first, last, value, std::less<T>());
//}


PYCPP_END_NAMESPACE

// TODO: need to modify this significantly...
///**
// * Function: InterpolationSearch(Iter begin, Iter end,
// *                               Element elem);
// * ------------------------------------------------------------------------
// * Performs interpolation search on the sorted range [begin, end).  It is
// * assumed that this range consists of finite integral values and that the
// * input is sorted in ascending order.  Returns whether the element was
// * located.
// */
//template <typename Iter, typename Element>
//bool InterpolationSearch(Iter begin, Iter end,
//                                   Element elem) {
//  /* Get a type holding the distance between iterators in the range. */
//  typedef typename std::iterator_traits<Iter>::difference_type diff_t;
//
//  /* Edge-case: If there is no input, the element can't exist. */
//  if (begin == end) return false;
//
//  /* Continue looping while the value could feasibly be in the range and
//   * the iterators haven't crossed.
//   */
//  while (*begin <= elem && elem <= *(end - 1) && begin != end) {
//    /* Interpolate between the endpoints to guess where the element should
//     * lie.  This works by computing the range [min, max] of the elements,
//     * then seeing what fraction of the way up elem is.
//     */
//    const double interpolation = (double(elem) - *begin) / (double(*(end - 1)) - double(*begin));
//
//    /* Scale this position to an index by multiplying by the number of elements
//     * in the range by the fraction up to search.
//     */
//    Iter mid = begin + diff_t(interpolation * (double(end - begin) - 1));
//
//    /* Apply standard binary search logic at this point.  If we found the element,
//     * we're done.
//     */
//    if (*mid == elem) return true;
//    /* Otherwise, if the element is smaller than what we're looking for, look
//     * to the right.
//     */
//    else if (*mid < elem) begin = mid + 1;
//    /* Otherwise, look to the right. */
//    else end = mid;
//  }
//
//  /* If we're here, we didn't find the element in question. */
//  return false;
//}
