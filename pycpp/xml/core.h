//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core XML definitions.
 */

#pragma once

#include <pycpp/stl/deque.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/unordered_map.h>
#include <stdint.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct xml_node_t;
struct xml_node_impl_t;
struct xml_node_list_t;
struct xml_node_iterator_t;

// ALIAS
// -----

using xml_string_t = std::string;
using xml_string_list_t = std::deque<xml_string_t>;
using xml_attr_t = std::unordered_map<xml_string_t, xml_string_t>;

// OBJECTS
// -------

/**
 *  \brief Iterator over XML nodes.
 */
struct xml_node_iterator_t: iterator<bidirectional_iterator_tag, xml_node_t>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = xml_node_iterator_t;
    using base_t = iterator<bidirectional_iterator_tag, xml_node_t>;
    using typename base_t::value_type;
    using typename base_t::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // CONSTRUCTORS
    xml_node_iterator_t();
    xml_node_iterator_t(const self_t&);
    self_t& operator=(const self_t&);
    xml_node_iterator_t(self_t&&);
    self_t& operator=(self_t&&);
    ~xml_node_iterator_t();

    // RELATIONAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

    // INCREMENTORS
    self_t& operator++();
    self_t operator++(int);
    self_t& operator--();
    self_t operator--(int);

    // DEREFERENCE
    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;

    // OTHER
    void swap(self_t&);

private:
    friend struct xml_node_t;
    friend struct xml_node_list_t;

    void *ptr_ = nullptr;
};


/**
 *  \brief Collection of ordered elements with name-based lookups.
 */
struct xml_node_list_t
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = xml_node_list_t;
    using value_type = xml_node_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = xml_node_iterator_t;
    using const_iterator = iterator;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // MEMBER FUNCTIONS
    // ----------------
    xml_node_list_t();
    xml_node_list_t(const self_t&);
    self_t& operator=(const self_t&);
    xml_node_list_t(self_t&&);
    self_t& operator=(self_t&&);
    ~xml_node_list_t();

    // ITERATORS
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    // RELATIONAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

    // LOOKUP
    // TODO: this should really be a string view....
    iterator find(const xml_string_t&) const;
    pair<iterator, iterator> findall(const xml_string_t&) const;

    // I/O
    xml_string_list_t tostringlist() const;
    static xml_node_list_t fromstringlist(const xml_string_list_t&);

    // CAPACITY
    bool empty() const;
    size_t size() const;
    size_t max_size() const;

    // ACCESS
    const_reference front() const;
    const_reference back() const;

    // MODIFIERS
    pair<iterator, bool> push_front(const value_type& x);
    pair<iterator, bool> push_front(value_type&& x);
    void pop_front();
    pair<iterator, bool> push_back(const value_type& x);
    pair<iterator, bool> push_back(value_type&& x);
    void pop_back();
    pair<iterator, bool> insert(iterator position, const value_type&);
    pair<iterator, bool> insert(iterator position, value_type&&);
    void clear();
    void swap(self_t&);

private:
    friend struct xml_node_t;

    void *ptr_ = nullptr;
};


/**
 *  \brief XML node type.
 */
struct xml_node_t
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = xml_node_t;
    using iterator = xml_node_iterator_t;
    using const_iterator = iterator;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    xml_node_t();
    xml_node_t(const xml_node_t&) = default;
    xml_node_t & operator=(const xml_node_t&) = default;
    xml_node_t(xml_node_t&&) = default;
    xml_node_t & operator=(xml_node_t&&) = default;

    // I/O
    xml_string_t tostring() const;
    static xml_node_t fromstring(const xml_string_t&);

    // ITERATORS
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    // GETTERS
    const xml_string_t& get_tag() const;
    const xml_string_t& get_text() const;
    xml_attr_t& get_attrs();
    const xml_attr_t& get_attrs() const;
    xml_node_list_t& get_children();
    const xml_node_list_t& get_children() const;
    uintptr_t get_id() const;

    // SETTERS
    void set_tag(const xml_string_t&);
    void set_text(const xml_string_t&);
    void set_attrs(const xml_attr_t&);
    void set_children(const xml_node_list_t&);
    void set_tag(xml_string_t&&);
    void set_text(xml_string_t&&);
    void set_attrs(xml_attr_t&&);
    void set_children(xml_node_list_t&&);

    // RELATIONAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

    // OTHER
    void swap(self_t&);

private:
    friend struct xml_node_list_t;

    xml_node_t(xml_node_impl_t*);
    shared_ptr<xml_node_impl_t> ptr_;
};

PYCPP_END_NAMESPACE
