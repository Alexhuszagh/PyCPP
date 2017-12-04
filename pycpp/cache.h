//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Database cache objects.
 */

#pragma once

#include <pycpp/cache/lri.h>
#include <pycpp/cache/lru.h>
#if BUILD_KEYVALUE
#   include <pycpp/cache/kv.h>
#endif
