//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Virtual interface for SQL query APIs.
 */

#pragma once

#include <pycpp/sql/database.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief
 */
struct sql_query
{
public:
    sql_query(sql_database& db);
    sql_query(const sql_query&) = delete;
    sql_query & operator=(const sql_query&) = delete;
    // TODO: move???

    // STATEMENTS
    bool exec(const string_view& statement);
    bool prepare(const string_view& statement);
    bool exec();

private:
    sql_database *db_;
    std::string statement_;
};
// TODO: implement...


PYCPP_END_NAMESPACE
