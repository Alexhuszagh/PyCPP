//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect runtime system settings.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Check if running on WINE.
 */
bool is_wine();

/**
 *  \brief Check if running in docker container.
 */
bool is_docker();

/**
 *  \brief Check if running in a general container.
 */
bool is_container();

PYCPP_END_NAMESPACE
