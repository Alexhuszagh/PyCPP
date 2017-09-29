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
struct csvpunct_impl
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
 *  \brief CSV punctuation for comma-separated values.
 */
struct csvpunct: csvpunct_impl
{};


/**
 *  \brief CSV punctuation for tab-delimited values.
 */
struct tabpunct: csvpunct_impl
{
protected:
    virtual char do_delimiter() const override;
};


/**
 *  \brief CSV punctuation for pipe-delimited values.
 */
struct pipepunct: csvpunct_impl
{
protected:
    virtual char do_delimiter() const override;
};


PYCPP_END_NAMESPACE
