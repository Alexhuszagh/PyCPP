//  :copyright: (c) Alex Huszagh 2017.
//  :license: MIT,see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cartesian product algorithm.
 *
 *  Lazily calculates the cartesian product from a container of containers,
 *  either list-like (array, vector, set, unordered_set) or map-like
 *  (map, unordered_map, multimap, unordered_multimap).
 *
 *  The code can be used as follows:
 *
 *      vector<vector<int>> aa = {{1, 2, 3}, {4, 5, 6}};
 *      product(aa, [](const auto &i) {
 *          cout << "[";
 *          for (auto j : i) {
 *              cout << j << ", ";
 *          }
 *          cout << "]" << endl;
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

#include <pycpp/misc/safe_stdlib.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/vector.h>
#include <pycpp/sfinae/is_reference_wrapper.h>

PYCPP_BEGIN_NAMESPACE

namespace prod_detail
{
// ALIAS
// -----

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
    using type = conditional_t<is_reference_wrapper<T>::value, T, reference_wrapper<const T>>;
    using container_type = typename type::type;
    using value_type = typename container_type::value_type;
    using iterator = typename container_type::const_iterator;

    using reference_type = conditional_t<
        is_reference_wrapper<value_type>::value,
        value_type,
        reference_wrapper<const value_type>
    >;

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
    using traits_type = iterator_traits<BidirIter>;
    using value_type = typename traits_type::value_type;
    using helper = iterator_reference<value_type>;
    using iterator_type = typename helper::iterator;
    using reference_type = typename helper::reference_type;

    size_t size, k;
    vector<iterator_type> buf;
    vector<reference_type> val;

    // sanity check
    if (first == last) {
        return;
    }

    // fill vector for function calls
    size = distance(first, last);
    k = 0;
    buf.reserve(size);
    val.reserve(size);

    for_each(first, last, [&](const value_type& value) {
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
    using first_type = typename T::value_type;
    using second_type = typename first_type::value_type;
    using reference_type = reference_wrapper<const second_type>;
    using list_type = vector<reference_type>;
    using matrix_type = vector<list_type>;
};


/**
 *  \brief Product wrapper for list of lists.
 */
template <typename T, typename F>
void list_list(const T& t, F &f)
{
    using helper = list_list_product<T>;
    using list_type = typename helper::list_type;
    using matrix_type = typename helper::matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.size());
        for (const auto &inner: outer) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(move(list));
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
    using first_type = typename T::value_type;
    using second_type = typename first_type::value_type;
    using reference_type = reference_wrapper<const second_type>;
    using list_type = vector<reference_type>;
    using matrix_type = vector<list_type>;
};


/**
 *  \brief Product wrapper for list of maps.
 */
template <typename T, typename F>
void list_map(const T& t, F &f)
{
    using helper = list_map_product<T>;
    using list_type = typename helper::list_type;
    using matrix_type = typename helper::matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.size());
        for (const auto &inner: outer) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(move(list));
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
    using first_type = typename T::mapped_type;
    using second_type = typename first_type::value_type;
    using reference_type = reference_wrapper<const second_type>;
    using list_type = vector<reference_type>;
    using matrix_type = vector<list_type>;
};


/**
 *  \brief Product wrapper for map of lists.
 */
template <typename T, typename F>
void map_list(const T& t, F &f)
{
    using helper = map_list_product<T>;
    using list_type = typename helper::list_type;
    using matrix_type = typename helper::matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.second.size());
        for (const auto &inner: outer.second) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(move(list));
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
    using first_type = typename T::mapped_type;
    using second_type = typename first_type::value_type;
    using reference_type = reference_wrapper<const second_type>;
    using list_type = vector<reference_type>;
    using matrix_type = vector<list_type>;
};


/**
 *  \brief Product wrapper for map of maps.
 */
template <typename T, typename F>
void map_map(const T& t, F &f)
{
    using helper = map_map_product<T>;
    using list_type = typename helper::list_type;
    using matrix_type = typename helper::matrix_type;

    matrix_type matrix;
    matrix.reserve(t.size());
    for (const auto &outer: t) {
        list_type list;
        list.reserve(outer.second.size());
        for (const auto &inner: outer.second) {
            list.emplace_back(inner);
        }
        matrix.emplace_back(move(list));
    }

    product_(matrix.begin(), matrix.end(), f);
}

// SFINAE
// ------


template<typename, template <typename> class, typename = void>
struct is_detected: false_type
{};

template<typename T, template <typename> class F>
struct is_detected<T, F, void_t<F<T>>>: true_type
{};

template <typename T>
using mapped_t = typename T::mapped_type;


template <typename T>
struct map_type
{
    using first_type = typename T::value_type;
    using value_type = typename T::value_type;
    using mapped_type = typename T::value_type;
};


template <typename T>
struct is_map
{
    static constexpr bool outer = is_detected<T, mapped_t>::value;
    using U = mapped_t<conditional_t<outer, T, map_type<T>>>;
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

}   /* prod_detail */

/**
 *  \brief Call cartesian product for container.
 */
template <typename T, typename F>
void product(const T& t, F f)
{
    prod_detail::cartesian_product()(t, f);
}

PYCPP_END_NAMESPACE
