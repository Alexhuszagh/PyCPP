//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator utilities.
 */

#pragma once

#include <ordering.h>
#include <iterator>
#include <tuple>
#include <type_traits>

// DECLARATION
// -----------

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

/**
 *  \brief Transformation iterator implied base class.
 */
template <typename Iterator, typename UnaryFunction>
struct traits_impl
{
    // MEMBER TYPES
    // ------------
    typedef std::iterator_traits<Iterator> traits_type;
    typedef typename std::result_of<const UnaryFunction(typename traits_type::reference)>::type value_type;
    typedef typename traits_type::iterator_category iterator_category;
    typedef typename traits_type::difference_type difference_type;
    typedef value_type& reference;
    typedef value_type* pointer;
};


/**
 *  \brief Base class for transform iterator.
 */
template <typename Iterator, typename UnaryFunction>
struct transform_iterator_base:
    std::iterator<
        typename traits_impl<Iterator, UnaryFunction>::iterator_category,
        typename traits_impl<Iterator, UnaryFunction>::value_type,
        typename traits_impl<Iterator, UnaryFunction>::difference_type,
        typename traits_impl<Iterator, UnaryFunction>::pointer,
        typename traits_impl<Iterator, UnaryFunction>::reference
    >
{
    // MEMBER TYPES
    // ------------
    typedef traits_impl<Iterator, UnaryFunction> traits_type;

    // SFINAE
    // ------
    // TYPES
    static constexpr bool is_input = std::is_same<typename traits_type::iterator_category, std::input_iterator_tag>::value;
    static constexpr bool is_output = std::is_same<typename traits_type::iterator_category, std::output_iterator_tag>::value;
    static constexpr bool is_forward = std::is_same<typename traits_type::iterator_category, std::forward_iterator_tag>::value;
    static constexpr bool is_bidirectional = std::is_same<typename traits_type::iterator_category, std::bidirectional_iterator_tag>::value;
    static constexpr bool is_random_access = std::is_same<typename traits_type::iterator_category, std::random_access_iterator_tag>::value;
    // FEATURES
    static constexpr bool has_total = is_random_access;
    static constexpr bool has_decrement = is_bidirectional || is_random_access;
    static constexpr bool has_indexing = is_random_access;
    static constexpr bool has_arithmetic = is_random_access;
};


/**
 *  \brief Iterator transformations.
 */
template <typename Iterator, typename UnaryFunction>
struct transform_iterator: transform_iterator_base<Iterator, UnaryFunction>
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename It, typename F>
    using rebind = transform_iterator<It, F>;

    // MEMBER TYPES
    // ------------
    typedef transform_iterator_base<Iterator, UnaryFunction> base_t;
    typedef transform_iterator<Iterator, UnaryFunction> self_t;
    using typename base_t::value_type;
    using typename base_t::reference;
    using typename base_t::pointer;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    transform_iterator();
    transform_iterator(const Iterator& it, UnaryFunction function);

    // BASE
    UnaryFunction functor() const;
    const Iterator& base() const;
    void swap(self_t&);

    // GLOBAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;
    self_t& operator++();
    self_t operator++(int);
    value_type operator*();
    value_type operator*() const;

    // CONDITIONAL OPERATORS

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator<(const rebind<It, F>& other) const
    {
        return it < other.it;
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator<=(const rebind<It, F>& other) const
    {
        return less_equal(*this, other);
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator>(const rebind<It, F>& other) const
    {
        return greater(*this, other);
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator>=(const rebind<It, F>& other) const
    {
        return greater_equal(*this, other);
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_decrement, rebind<It, F>&> operator--()
    {
        --it;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_decrement, rebind<It, F>> operator--(int)
    {
        self_t copy(*this);
        operator--();
        return copy;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_indexing, typename rebind<It, F>::value_type> operator[](size_t pos) const
    {
        return function(it[pos]);
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>&> operator+=(difference_type n)
    {
        it += n;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>&> operator-=(difference_type n)
    {
        it -= n;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>> operator+(difference_type n) const
    {
        self_t copy(*this);
        copy += n;
        return copy;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>> operator-(difference_type n) const
    {
        self_t copy(*this);
        copy -= n;
        return copy;
    }

private:
    Iterator it;
    UnaryFunction function;
};


// FUNCTIONS
// ---------


template <typename It, typename F>
transform_iterator<It, F> make_transform_iterator(const It& it, F function)
{
    return transform_iterator<It, F>(it, function);
}


// IMPLEMENTATION
// --------------

template <typename It, typename F>
transform_iterator<It, F>::transform_iterator()
{}


template <typename It, typename F>
transform_iterator<It, F>::transform_iterator(const It& it, F function):
    it(it),
    function(function)
{}


template <typename It, typename F>
F transform_iterator<It, F>::functor() const
{
    return function;
}


template <typename It, typename F>
const It& transform_iterator<It, F>::base() const
{
    return it;
}


template <typename It, typename F>
void transform_iterator<It, F>::swap(self_t& other)
{
    std::swap(it, other.it);
    std::swap(function, other.function);
}


template <typename It, typename F>
bool transform_iterator<It, F>::operator==(const self_t& other) const
{
    return std::tie(it, function) == std::tie(other.it, other.function);
}

template <typename It, typename F>
bool transform_iterator<It, F>::operator!=(const self_t& other) const
{
    return not_equal_to(*this, other);
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator++() -> self_t&
{
    ++it;
    return *this;
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator*() -> value_type
{
    return function(*it);
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator*() const -> value_type
{
    return function(*it);
}