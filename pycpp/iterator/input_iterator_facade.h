//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Input iterator facade to wrap existing generators.
 *
 *  This facade is "fat", like Boost.Range, that is, both the first
 *  and last iterator in the range will hold "value_type". However, on
 *  modern compilers, this should mostly be optimized away (since the last
 *  "value_type" is never used), and therefore provide comparable performance
 *  to an optimized range.
 */

#pragma once

#include <pycpp/config.h>

#include <cassert>
#include <iterator>
#include <tuple>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Facade for an input iterator.
 *
 *  The generator must implement `operator()()` to fetch the next value,
 *  and must implement `operator bool()` to check if the generator
 *  can return any more values. These are not implemented in terms of
 *  callbacks for performance reasons: if you need to wrap code using
 *  another generator interface, use an adaptor.
 */
template <typename G, typename V = typename G::value_type>
struct input_iterator_facade: std::iterator<std::input_iterator_tag, V>
{
public:
    // MEMBER TYPES
    // ------------
    using base_t = std::iterator<std::input_iterator_tag, V>;
    using self_t = input_iterator_facade<G, V>;
    using typename base_t::value_type;
    using typename base_t::reference;

    // MEMBER FUNCTIONS
    // ----------------
    input_iterator_facade();
    input_iterator_facade(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    input_iterator_facade(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
    input_iterator_facade(G&);

    // OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;
    value_type& operator*();
    const value_type& operator*() const;
    value_type* operator->();
    const value_type* operator->() const;
    self_t& operator++();
    self_t operator++(int);

private:
    G* generator_ = nullptr;
    value_type value_ = value_type();
};

// DEFINITION
// ----------

template <typename G, typename V>
input_iterator_facade<G, V>::input_iterator_facade()
{}


template <typename G, typename V>
input_iterator_facade<G, V>::input_iterator_facade(G& generator):
    generator_(&generator),
    value_(generator())
{}


template <typename G, typename V>
bool input_iterator_facade<G, V>::operator==(const self_t &rhs) const
{
    return std::tie(generator_, value_) == std::tie(rhs.generator_, rhs.value_);
}


template <typename G, typename V>
bool input_iterator_facade<G, V>::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator*() -> value_type&
{
    return value_;
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator*() const -> const value_type&
{
    return value_;
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator->() -> value_type*
{
    return &value_;
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator->() const -> const value_type*
{
    return &value_;
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator++() -> self_t&
{
    if (generator_ && *generator_) {
        value_ = (*generator_)();
    } else {
        generator_ = nullptr;
        value_ = value_type();
    }
    return *this;
}


template <typename G, typename V>
auto input_iterator_facade<G, V>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}

PYCPP_END_NAMESPACE
