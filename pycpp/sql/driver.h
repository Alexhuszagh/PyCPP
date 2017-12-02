//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Virtual interface for SQL drivers.
 */

#pragma once

#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief
 */
struct sql_driver
{
public:
    sql_driver(void* db);

    // STATEMENTS
    virtual bool exec(const string_view& statement) = 0;
    virtual bool prepare(const string_view& statement) = 0;
    virtual bool exec() = 0;

    // TODO: add a lot of shit...
    // TODO: need iteration

protected:
    void* db_;
};

PYCPP_END_NAMESPACE
