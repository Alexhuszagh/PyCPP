//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML SAX parser.
 */

#pragma once

#include <view/string.h>

// OBJECTS
// -------


/**
 *  \brief SAX handler for an XML document.
 */
class XmlSaxHandler
{
    virtual void start_document();
    virtual void end_document();
// TODO: need to consider how to handle element attributes...
//    virtual void start_element(const string_view& content);
//    virtual void end_element(const string_view& content);
//    virtual void characters(const string_view& content);
};
