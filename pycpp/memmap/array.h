//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Memory-mapped file for larger-than-memory arrays.
 *
 *  Internally, the memory-mapped file is implemented like a deque,
 *  with an LRU-cache.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


// FUNCTIONS
// ---------

// TODO: this should be a NumPy-style array, so it can
// be a matrix....
// TODO: how do I estimate the blocksize then???

PYCPP_END_NAMESPACE
