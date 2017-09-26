//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV punctuation.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Vocabulary for CSV punctuation.
 */
struct csvpunct
{
public:
    char delimiter() const;
    char quote() const;
    char escape() const;

protected:
    virtual char do_delimiter() const;
    virtual char do_quote() const;
    virtual char do_escape() const;
};

PYCPP_END_NAMESPACE
