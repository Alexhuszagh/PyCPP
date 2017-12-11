//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/sql/sql.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/unordered_map.h>

PYCPP_BEGIN_NAMESPACE

// 1. I'm not sure how to implement this...
//      If the connection name already exists....

// I need some way of opening the database if it doesn't exist.
// I need some way of closing it when the last ref goes to 0.

//// ALIAS
//// -----
//
//using string_ref = std::reference_wrapper<const std::string>;
//using sql_database_cache = std::unordered_map<string_ref, sql_database*>;
//
//// GLOBALS
//// -------
//
//sql_database_cache OPEN_DATABASES;

// OBJECTS
// -------


PYCPP_END_NAMESPACE
