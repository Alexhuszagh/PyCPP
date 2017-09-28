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
 *
 *  Override the delimiter (','), quote ('"'), and escape
 *  ("\\") methods to subclass. Common CSV dialects are provided.
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


/**
 *  \brief CSV punctuation for tab-delimited text files.
 */
struct tabpunct: csvpunct
{
protected:
    virtual char do_delimiter() const;
};


/**
 *  \brief CSV punctuation for pipe-delimited text files.
 */
struct pipepunct: csvpunct
{
protected:
    virtual char do_delimiter() const;
};


PYCPP_END_NAMESPACE
