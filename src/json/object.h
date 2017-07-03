//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON object definitions.
 */

#pragma once

#include <json/core.h>
#include <unordered_map>


// OBJECTS
// -------

typedef std::unordered_map<json_string_t, json_value_t*> json_object_t;

/**
 *  \brief Wrapper providing by-value access for JSON object values.
 */
struct json_object_t_impl
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::unordered_map<json_string_t, json_value_t*> base;
    typedef json_object_t_impl self;
    typedef json_string_t key_type;
    typedef json_value_t mapped_type;
    typedef std::pair<const key_type&, json_value_t&> value_type;
    typedef std::hash<key_type> hasher;
    typedef std::equal_to<key_type> key_equal;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;

    // MEMBER FUNCTIONS
    // ----------------
    ~json_object_t_impl();

    // CAPACITY
    size_type size() const;
    size_type max_size() const;
    bool empty() const;

    // ELEMENT ACCESS
    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;
    // TODO: a lot of bullshit here...

private:
    base map_;
};
// TODO: write wrapper around JSON object
// TODO: needs to dereference upon get
// TODO: needs to cleanup on destructor
