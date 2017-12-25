//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Virtual interface for SQL database APIs.
 */

#pragma once

#include <pycpp/sql/driver.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Get the default connection name for a new database.
 */
string_wrapper default_connection_name();

// DECLARATIONS
// ------------

/**
 *  \brief
 */
struct sql_database
{
public:
    sql_database();
    sql_database(const string_wrapper& connection_name);
    ~sql_database();
    // TODO: need copy/etc constructors....

// TODO: get open connection by name
//    static sql_database database(const string_wrapper& connection_name);
    // DRIVERS
    virtual sql_driver& driver() = 0;

private:
    string connection_name_;
};
// TODO: implement...


PYCPP_END_NAMESPACE
