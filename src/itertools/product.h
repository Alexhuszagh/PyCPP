//  :copyright: (c) Alex Huszagh 2017.
//  :license: Boost,see licenses/boost.md for more details.
/**
 *  \addtogroup funcxx
 *  \brief Cartesian product algorithm.
 *
 *  Lazily calculates the cartesian product from a container of containers,
 *  either list-like (array, vector, set, unordered_set) or map-like
 *  (map, unordered_map, multimap, unordered_multimap).
 *
 *  The code can be used as follows:
 *
 *      std::vector<std::vector<int>> aa = {{1, 2, 3}, {4, 5, 6}};
 *      product(aa, [](const auto &i) {
 *          std::cout << "[";
 *          for (auto j : i) {
 *              std::cout << j << ", ";
 *          }
 *          std::cout << "]" << std::endl;
 *          return false;
 *      });
 *
 *  The above example would print:
 *      [1, 4, ]
 *      [1, 5, ]
 *      [1, 6, ]
 *      [2, 4, ]
 *      [2, 5, ]
 *      [2, 6, ]
 *      [3, 4, ]
 *      [3, 5, ]
 *      [3, 6, ]
 */

#pragma once

#include <safe/stdlib.h>
#include <algorithm>
#include <iterator>
#include <functional>
#include <type_traits>
#include <vector>

// DETAIL
// ------

template <bool B, typename T, typename F>
using conditional_t = typename std::conditional<B, T, F>::type;

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename...>
using void_t = void;

template <typename T>
struct is_reference_wrapper: std::false_type
{};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>>: std::true_type
{};

/**
 *  \brief Helper demangle and product reference types.
 *
 *  Due to the various levels of indirection with our wrapper classes,
 *  we can have the inner container be a reference, or a value, and the
 *  `value_type` of that container be a reference, or a value.
 *
 *  Since we want the output to be a vector of reference wrappers,
 *  appropriately detect when we are using references and when we
 *  are using values.
 */
template <typename T>
struct iterator_reference
{
    typedef conditional_t<is_reference_wrapper<T>::value, T, std::reference_wrapper<const T>> type;
    typedef typename type::type container_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::const_iterator iterator;

    typedef conditional_t<
        is_reference_wrapper<value_type>::value,
        value_type,
        std::reference_wrapper<const value_type>
    > reference_type;

    static iterator begin(const container_type &t)
    {
        return t.begin();
    }

    static iterator end(const container_type &t)
    {
        return t.end();
    }
};


/**
 *  \brief Calculate cartesian product with zero data copies.
 */
template < typename BidirIter, typename F>
void product_(BidirIter first, BidirIter last, F &f)
{
    typedef std::iterator_traits<BidirIter> traits_type;
    typedef typename traits_type::value_type value_type;
    typedef iterator_reference<value_type> helper;
    typedef typename helper::iterator iterator_type;
    typedef typename helper::reference_type reference_type;

    size_t size, k;
    std::vector<iterator_type> buf;
    std::vector<reference_type> val;

    // sanity check
    if (first == last) {
        return;
    }

    // fill vector for function calls
    size = std::distance(first, last);
    k = 0;
    buf.reserve(size);
    val.reserve(size);

    std::for_each(first, last, [&](const value_type& value) {
        buf.emplace_back(--helper::begin(value));
        val.emplace_back(*helper::begin(value));
        ++k;
    });

    // iterate over all elements
    auto it = first;
    while (it >= first) {
        k = it - first;
        if (it == last) {
            if (f(val.data(), val.data()+val.size())) {
                // early, manually-specified break
                break;
            }
            --it;
        } else if (buf[k] != --helper::end(*it)) {
            val[k] = *++buf[k];
            ++it;
        } else {
            buf[k] = --helper::begin(*it);
            --it;
        }
    }
}


// LIST - LIST
// -----------


/**
 *  \brief Type detection for list of lists.
 */
template <typename T>
struct list_list_product
{
    typedef typename T::value_type first_type;
    typedef typename first_type::value_type second_type;
    typedef std::reference_wrapper<const second_type> reference_type;
    typedef std::vector<reference_type> list_type;
    typedef std::vector<list_type> matrix_type;
};


/**
 *  \brief Product wrapper for list of lists.
 */
template <typename T, typename F>
void list_list(const T& t, F &f)
{
    typedef list_list_product<T> helper;
    typedef typename helper::list_type list_type;
    typedef typename helper::matrix_type matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.size());
        for (const auto &inner: outer) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(std::move(list));
    }

    product_(matrix.begin(), matrix.end(), f);
}


// LIST - MAP
// ----------

/**
 *  \brief Type detection for list of maps.
 */
template <typename T>
struct list_map_product
{
    typedef typename T::value_type first_type;
    typedef typename first_type::value_type second_type;
    typedef std::reference_wrapper<const second_type> reference_type;
    typedef std::vector<reference_type> list_type;
    typedef std::vector<list_type> matrix_type;
};


/** \brief Product wrapper for list of maps.
 */
template <typename T, typename F>
void list_map(const T& t, F &f)
{
    typedef list_map_product<T> helper;
    typedef typename helper::list_type list_type;
    typedef typename helper::matrix_type matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.size());
        for (const auto &inner: outer) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(std::move(list));
    }

    product_(matrix.begin(), matrix.end(), f);
}


// MAP - LIST
// ------------


/**
 *  \brief Type detection for map of lists.
 */
template <typename T>
struct map_list_product
{
    typedef typename T::mapped_type first_type;
    typedef typename first_type::value_type second_type;
    typedef std::reference_wrapper<const second_type> reference_type;
    typedef std::vector<reference_type> list_type;
    typedef std::vector<list_type> matrix_type;
};


/**
 *  \brief Product wrapper for map of lists.
 */
template <typename T, typename F>
void map_list(const T& t, F &f)
{
    typedef map_list_product<T> helper;
    typedef typename helper::list_type list_type;
    typedef typename helper::matrix_type matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.second.size());
        for (const auto &inner: outer.second) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(std::move(list));
    }

    product_(matrix.begin(), matrix.end(), f);
}


// MAP - MAP
// ---------


/**
 8  \brief Type detection for map of maps.
 */
template <typename T>
struct map_map_product
{
    typedef typename T::mapped_type first_type;
    typedef typename first_type::value_type second_type;
    typedef std::reference_wrapper<const second_type> reference_type;
    typedef std::vector<reference_type> list_type;
    typedef std::vector<list_type> matrix_type;
};


/**
 *  \brief Product wrapper for map of maps.
 */
template <typename T, typename F>
void map_map(const T& t, F &f)
{
    typedef map_map_product<T> helper;
    typedef typename helper::list_type list_type;
    typedef typename helper::matrix_type matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.second.size());
        for (const auto &inner: outer.second) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(std::move(list));
    }

    product_(matrix.begin(), matrix.end(), f);
}

// SFINAE
// ------


template<typename, template <typename> class, typename = void>
struct is_detected: std::false_type
{};

template<typename T, template <typename> class F>
struct is_detected<T, F, void_t<F<T>>>: std::true_type
{};

template <typename T>
using mapped_t = typename T::mapped_type;


template <typename T>
struct map_type
{
    typedef typename T::value_type first_type;
    typedef typename T::value_type value_type;
    typedef typename T::value_type mapped_type;
};


template <typename T>
struct is_map
{
    static constexpr bool outer = is_detected<T, mapped_t>::value;
    typedef mapped_t<conditional_t<outer, T, map_type<T>>> U;
    static constexpr bool inner = is_detected<U, mapped_t>::value;
};


template <typename T>
struct is_list_list
{
    static constexpr bool value = !(is_map<T>::outer || is_map<T>::inner);
};


template <typename T>
struct is_list_map
{
    static constexpr bool value = !is_map<T>::outer && is_map<T>::inner;
};


template <typename T>
struct is_map_list
{
    static constexpr bool value = is_map<T>::outer && !is_map<T>::inner;
};


template <typename T>
struct is_map_map
{
    static constexpr bool value = is_map<T>::outer && is_map<T>::inner;
};


// PRODUCT
// -------


/**
 *  \brief Deduce container type and invoke correct dispatcher.
 */
struct cartesian_product
{
    template <typename T, typename F>
    enable_if_t<is_list_list<T>::value, void> operator()(const T& t, F &f)
    {
        list_list(t, f);
    }

    template <typename T, typename F>
    enable_if_t<is_list_map<T>::value, void> operator()(const T& t, F &f)
    {
        list_map(t, f);
    }

    template <typename T, typename F>
    enable_if_t<is_map_list<T>::value, void> operator()(const T& t, F &f)
    {
        map_list(t, f);
    }

    template <typename T, typename F>
    enable_if_t<is_map_map<T>::value, void> operator()(const T& t, F &f)
    {
        map_map(t, f);
    }
};


/**
 *  \brief Call cartesian product for container.
 */
template <typename T, typename F>
void product(const T& t, F f)
{
    cartesian_product()(t, f);
}
