//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON array definitions.
 */

#pragma once

#include <json/core.h>
#include <json/dom.h>
#include <vector>


// OBJECTS
// -------


//typedef std::vector<json_value_t*> json_array_t;

/**
 *  \brief Wrapper providing by-value access for JSON array values.
 */
struct json_array_t
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::vector<json_value_t*> base;
    typedef json_array_t self;
    typedef json_value_t value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    // TODO: need iterators
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;

    // MEMBER FUNCTIONS
    // ----------------
    ~json_array_t();
    // TODO: a lot of bullshit here...

    // CAPACITY
    size_type size() const;
    size_type max_size() const;
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type);
    void shrink_to_fit();

    // ELEMENT ACCESS
    reference operator[](size_type);
    const_reference operator[](size_type) const;
    reference at(size_type);
    const_reference at(size_type) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // MODIFIERS
    // TODO: add emplace modifiers since I now have that...
    void push_back(value_type*);
    void pop_back();
    void clear();

private:
    base list_;
};


// TODO: write wrapper around JSON array
// TODO: needs to dereference upon get
// TODO: needs to cleanup on destructor
