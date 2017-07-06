//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <xml/core.h>
#include <stdexcept>

// OBJECTS
// -------


xml_node_t::xml_node_t():
    parent_(nullptr)
{}


xml_node_t::xml_node_t(xml_node_list_t& parent):
    parent_(&parent)
{}


const xml_string_t& xml_node_t::get_tag() const
{
    return tag_;
}


const xml_string_t& xml_node_t::get_text() const
{
    return text_;
}


void xml_node_t::set_tag(const xml_string_t& tag)
{
    if (parent_) {
        // TODO: need to signify to the parent the element has been modified.
    } else {
        tag_ = tag;
    }
}


void xml_node_t::set_text(const xml_string_t& text)
{
    text_ = text;
}
