//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/sql/sqlite.h>
#include <pycpp/stl/functional.h>
#include <sqlite3.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

//using string_ref = reference_wrapper<const std::string>;
//using sql_database_cache = unordered_map<string_ref, sql_database*>;

// CONSTANTS
// ---------

const int sqlite_ok = SQLITE_OK;
//extern const int sqlite_open_readonly;

// FUNCTIONS
// ---------

static int prepare_impl(sqlite3* db, const string_wrapper& query, sqlite3_stmt*& stmt)
{
    return sqlite3_prepare_v3(db, query.data(), query.size(), 0, &stmt, nullptr)
}

// TODO: here...

// OBJECTS
// -------

// EXCEPTION

sqlite_exception::sqlite_exception(int code) noexcept:
    code_(code)
{}


const char* sqlite_exception::what() const noexcept
{
    return sqlite3_errstr(code_);
}

// DRIVER

sqlite_driver::sqlite_driver(sqlite3* db) noexcept:
    sql_driver(db)
{}


bool sqlite_driver::exec(const string_wrapper& query)
{
    status_ = prepare_impl(db_, query, stmt_);
    if (status_ != sqlite_ok) {
        sqlite_exception error(status_);
        sqlite3_finalize(stmt_);
        throw error;
    }
    // TODO: here...
//    return exec();
}


bool sqlite_driver::prepare(const string_wrapper& query)
{
    // TODO: use the v2 interface...
    // TODO: change
    status_ = prepare_impl(db_, query, stmt_);
    return status_ == sqlite_ok;
}


bool sqlite_driver::exec()
{
    // TODO: store status...
//    sqlite3_step(stmt_);
//    sqlite3_finalize(stmt_);
    // TODO: implement
    return false;
}


// DATABASE

sqlite_database::sqlite_database(const string_wrapper& file_name, const int flags, const string_wrapper& vfs)
{
    int status = sqlite3_open_v2(file_name.data(), &db_, flags, vfs.data());
    if (status != sqlite_ok) {
        // create then throw error
        sqlite_exception error(status);
        sqlite3_close(db_);
        throw error;
    }
}


sqlite_database::~sqlite_database() noexcept
{
    // ignore the database error.
    sqlite3_close(db_);
}


sql_driver& sqlite_database::driver() noexcept
{
    return driver_;
}


PYCPP_END_NAMESPACE
