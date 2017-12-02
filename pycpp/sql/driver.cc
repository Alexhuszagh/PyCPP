//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/sql/driver.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

sql_driver::sql_driver(void* db):
    db_(db)
{}

PYCPP_END_NAMESPACE
