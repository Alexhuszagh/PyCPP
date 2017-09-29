//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief User home directory utilities.
 */

#pragma once

#include <pycpp/filesystem/path.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

/**
 *  \brief Path for the home directory, returned by gethomedir();
 */
extern path_t homedir;

// FUNCTIONS
// ---------

/**
 * \brief Get path to home directory.
 */
path_t gethomedir();

#if defined(OS_WINDOWS)          // BACKUP PATH

path_t gethomedirw();
backup_path_t gethomedira();

#endif

PYCPP_END_NAMESPACE
