//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Safe(r) functions for malloc/calloc/realloc/free.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

/**
 *  \brief Throws std::bad_alloc if memory cannot be allocated.
 */
void* safe_malloc(size_t size);

/**
 *  \brief Throws std::bad_alloc if memory cannot be allocated.
 */
void* safe_realloc(void* ptr, size_t size);

/**
 *  \brief Throws std::bad_alloc if memory cannot be allocated.
 */
void* safe_calloc(size_t num, size_t size);

/**
 *  \brief Throws std::bad_alloc if memory cannot be allocated.
 */
void safe_free(void* ptr);
