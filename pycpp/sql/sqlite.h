//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SQLite wrapper.
 */

#pragma once

#include <pycpp/sql/database.h>
#include <pycpp/stl/exception.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct sqlite3;
struct sqlite3_stmt;

// CONSTANTS
// ---------

extern const int sqlite_ok;
extern const int sqlite_open_readonly;

// DECLARATIONS
// ------------


/**
 *  \brief Exception for SQLite connections.
 */
struct sqlite_exception: public exception
{
public:
    sqlite_exception(int code);
    virtual const char* what() const noexcept override;

private:
    int code_;
};

// TODO: need options....
// TODO: need to find the proper way to share connections
// or databases

/**
 *  \brief SQLite driver wrapper.
 */
struct sqlite_driver: sql_driver
{
public:
    sqlite_driver(sqlite3* db);
    ~sqlite_driver();

    // STATEMENTS
    virtual bool exec(const string_wrapper& statement) override;
    virtual bool prepare(const string_wrapper& statement) override;
    virtual bool exec() override;
    // TODO: step()

private:
    // TODO: I might need to be more careful with this...
    // Can't you make multiple statements and then process them async?
    sqlite3_stmt* stmt_ = nullptr;
    int status_ = sqlite_ok;
};


/**
 *  \brief SQLite database wrapper.
 */
struct sqlite_database: sql_database
{
public:
    sqlite_database(const string_wrapper& file_name,
                    const int flags = sqlite_open_readonly,
                    const string_wrapper& vfs = string_wrapper());
    ~sqlite_database();

    // DRIVERS
    virtual sql_driver& driver() override;

private:
    sqlite3* db_ = nullptr;
    sqlite_driver driver_;
};


PYCPP_END_NAMESPACE
