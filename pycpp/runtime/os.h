//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect runtime system settings.
 *
 *  Detect the system environment at runtime, such as code execution
 *  in a container (such as Docker, or Heroku), or if the code
 *  is running on WINE.
 *
 *  \synopsis
 *      bool is_wine() noexcept;
 *      bool is_container() noexcept;
 *      bool is_docker() noexcept;
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Check if running on WINE.
 */
bool is_wine() noexcept;

/**
 *  \brief Check if running in a general container.
 */
bool is_container();

#if BUILD_FILESYSTEM                // BUILD_FILESYSTEM

/**
 *  \brief Check if running in Docker container.
 */
bool is_docker();

#endif                              // BUILD_FILESYSTEM

PYCPP_END_NAMESPACE
