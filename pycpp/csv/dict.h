//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hashmap-based CSV readers and writers.
 */

#pragma once

#include <pycpp/collections/ordereddict.h>
#include <pycpp/csv/reader.h>
#include <pycpp/csv/writer.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using csv_indexes = ordered_map<std::string, size_t>;

// TODO: need csv_indexes for the...

PYCPP_END_NAMESPACE
