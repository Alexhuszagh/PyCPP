//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV punctuation.
 */

#pragma once

#include <pycpp/config.h>

#include <string>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using csv_row = std::vector<std::string>;

// ENUMS
// -----

/**
 *  \brief Quoting styles for CSV files.
 *
 *  CSV_QUOTE_ALL:          Quote all values.
 *  CSV_QUOTE_MINIMAL:      Quote only values containing control characters.
 *  CSV_QUOTE_NONE:         Never quote (may produce malformed CSV output).
 *
 *  QUOTE_NONNUMERIC is not present in the C++ API, due to C++'s static
 *  typing.
 */
enum csv_quoting
{
    CSV_QUOTE_ALL = 0,
    CSV_QUOTE_MINIMAL,
    CSV_QUOTE_NONE,
};

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
