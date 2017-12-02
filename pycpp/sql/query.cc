//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/sql/query.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

sql_query::sql_query(sql_database& db)
    db_(&db)
{}


bool sql_query::exec(const string_view& statement)
{
    return db_->driver().exec(statement);
}


bool sql_query::prepare(const string_view& statement)
{
    return db_->driver().prepare(statement);
}


bool sql_query::exec()
{
    return db_->driver().exec();
}


PYCPP_END_NAMESPACE
