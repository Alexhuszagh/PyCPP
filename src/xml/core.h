//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core XML definitions.
 */

#pragma once

#include <cstdint>
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
struct xml_node_list_t;

// ALIAS
// -----

typedef std::string xml_string_t;
typedef std::unordered_map<xml_string_t, xml_string_t> xml_attr_t;

// OBJECTS
// -------

// TODO: need boost::multi_index with the PimPl idiom for this...

// TODO: need an iterator wrapper too...

struct xml_node_list_t
{
public:
private:
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
    // TODO: document the interface
    xml_node_t();

    // TODO:....
    void clear();

private:
    xml_string_t tag_;
    xml_string_t text_;
    xml_attr_t attr_;
    xml_node_list_t children_;
};
