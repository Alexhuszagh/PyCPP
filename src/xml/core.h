//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core XML definitions.
 */

#pragma once

#include <cstdint>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>


//// ENUMS
//// -----
//
//
///**
// *  \brief Enumerated values for a XML object type.
// */
//enum xml_node_type: uint8_t
//{
//    xml_none_type = 0,
//    xml_boolean_type,
//    xml_number_type,
//    xml_string_type,
//    xml_array_type,
//    xml_object_type,
//};


// FORWARD
// -------

struct xml_node_t;
struct xml_node_impl_t;
struct xml_node_list_t;
struct xml_node_iterator_t;

// ALIAS
// -----

typedef uint32_t xml_id_t;
typedef std::string xml_string_t;
typedef std::unordered_map<xml_string_t, xml_string_t> xml_attr_t;

// OBJECTS
// -------

// TODO: need boost::multi_index with the PimPl idiom for this...
// TODO: need an iterator wrapper too...

/**
 *  \brief Iterator over XML nodes.
 */
struct xml_node_iterator_t: std::iterator<std::bidirectional_iterator_tag, xml_node_t>
{
public:
    // MEMBER TYPES
    // ------------
    typedef xml_node_iterator_t self;
    typedef std::iterator<std::bidirectional_iterator_tag, xml_node_t> base;
    using typename base::value_type;
    using typename base::difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // CONSTRUCTORS
    xml_node_iterator_t() = delete;
    xml_node_iterator_t(const self&);
    self & operator=(const self&);
    xml_node_iterator_t(self&&);
    self & operator=(self&&);
    ~xml_node_iterator_t();

    // RELATIONAL OPERATORS
    bool operator==(const self&) const;
    bool operator!=(const self&) const;

    // INCREMENTORS
    self& operator++();
    self operator++(int);
    self& operator--();
    self operator--(int);

    // DEREFERENCE
    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;

    // OTHER
    void swap(self&);

private:
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
    typedef xml_node_list_t self;
    typedef xml_node_iterator_t iterator;
    typedef iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    xml_node_list_t();
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
    bool operator==(const self&) const;
    bool operator!=(const self&) const;

    // MODIFIERS
//    void clear();
    void swap(self&);

    // TODO:
private:
    friend class xml_node_t;

    void *ptr_ = nullptr;
};


/**
 *  \brief XML node type.
 *
 *  // TODO: document the interface
 */
struct xml_node_t
{
public:
    // MEMBER TYPES
    // ------------
    typedef xml_node_t self;
    typedef xml_node_iterator_t iterator;
    typedef iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    xml_node_t();
    xml_node_t(xml_node_list_t&);
    xml_node_t(const xml_node_t&) = default;
    xml_node_t & operator=(const xml_node_t&) = default;
    xml_node_t(xml_node_t&&) = default;
    xml_node_t & operator=(xml_node_t&&) = default;

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
    const xml_id_t& get_id() const;

    // SETTERS
    void set_tag(const xml_string_t&);
    void set_text(const xml_string_t&);
    void set_attrs(const xml_attr_t&);
    void set_children(const xml_node_list_t&);

    // RELATIONAL OPERATORS
    bool operator==(const self&) const;
    bool operator!=(const self&) const;

private:
    friend class xml_node_list_t;

    std::shared_ptr<xml_node_impl_t> ptr_;
};
