//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Intrusive doubly-linked list.
 */

#pragma once

#include <pycpp/stl/iterator.h>
#include <pycpp/stl/limits.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief POD base class for a node of an `intrusive_list`.
 *
 *  You should subclass this class to create the custom data type
 * for your intrusive forward list.
 */
struct intrusive_list_node
{
    intrusive_list_node* next = nullptr;
    intrusive_list_node* prev = nullptr;
};


/**
 *  \brief Iterator type to wrap nodes.
 */
template <typename T>
struct intrusive_list_iterator: iterator<bidirectional_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = intrusive_list_iterator<T>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    intrusive_list_iterator(T* node = nullptr);
    intrusive_list_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
//    template <typename U>
//    enable_if<is_same<T, add_const<U>::type>::value, self_t&>
//    operator=(const intrusive_list_iterator<U, DifferenceType, Pointer, Reference>&);

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    self_t& operator--();
    self_t operator--(int);
    pointer operator->();
    const_pointer operator->() const;
    reference operator*();
    const_reference operator*() const;

//    // OTHER
//    void swap(self_t&);
//
//    // RELATIONAL
//    bool operator==(const self_t&) const;
//    bool operator!=(const self_t&) const;

    // TODO: here...
private:
    template <typename U>
    friend struct intrusive_list;

    T* node_;
};


/**
 *  \brief Intrusive list container.
 */
template <typename T>
struct intrusive_list
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = intrusive_list<T>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    // TODO: need conversion to and from iterators
    using iterator = intrusive_list_iterator<T>;
    using const_iterator = intrusive_list_iterator<const T>;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // CONSTRUCTORS
    intrusive_list() noexcept;
    ~intrusive_list() noexcept;

//    // construct/copy/destroy:
//    intrusive_list() noexcept;
//    template <class Iter>
//    intrusive_list(Iter first, Iter last);
//    intrusive_list(intrusive_list&& x) noexcept;
//    ~intrusive_list() noexcept;
//    intrusive_list& operator=(intrusive_list&& x) noexcept;
//    template <class Iter>
//      void assign(Iter first, Iter last);

    // ITERATORS
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    // rbegin
    // crbegin
    // rend
    // crend

    // CAPACITY
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // ELEMENT ACCESS
    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;

    // MODIFIERS
    void push_front(value_type& x) noexcept;
    void push_back(value_type& x) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;

    // LOTS MORE SHIT TO ADD

private:
    intrusive_list_node sentinel_;
};

// IMPLEMENTATION
// --------------

// ITERATOR

// LIST

PYCPP_END_NAMESPACE

