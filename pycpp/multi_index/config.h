//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Custom defaults for multi_index containers.
 */

#pragma once

#include <pycpp/stl/functional.h>

#define MULTI_INDEX_HASH        PYCPP_NAMESPACE::hash
#define MULTI_INDEX_EQUAL_TO    PYCPP_NAMESPACE::equal_to
#define MULTI_INDEX_LESS        PYCPP_NAMESPACE::less
#define MULTI_INDEX_GREATER     PYCPP_NAMESPACE::greater
