//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <typeinfo> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <typeinfo>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using type_info = std::type_info;
using bad_typeid = std::bad_typeid;
using bad_cast = std::bad_cast;

PYCPP_END_NAMESPACE
