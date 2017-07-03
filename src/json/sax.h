//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON SAX parser.
 */

#pragma once

#include <json/core.h>
#include <view/string.h>


// OBJECTS
// -------


/**
 *  \brief SAX handler for a JSON document.
 */
struct json_sax_handler
{
    virtual void start_document();
    virtual void end_document();
    virtual void start_object();
    virtual void end_object();
    virtual void start_array();
    virtual void end_array();
//    virtual void key();
//    virtual void value();
//    virtual void element();
};
