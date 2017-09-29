//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Temporary file and directory utilities.
 */

#pragma once

#include <pycpp/filesystem/path.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

/**
 *  \brief Path for the temporary directory, returned by gettempdir();
 */
extern path_t tempdir;

/**
 *  \brief Path for the temporary prefix, returned by gettempprefix();
 */
extern path_t tempprefix;

// FUNCTIONS
// ---------

// mkstemp
// mkdtemp

/**
 * \brief Get path to temp directory.
 */
path_t gettempdir();

/**
 * \brief Get the prefix used for named temporary files.
 */
path_t gettempprefix();

// tempdir

#if defined(OS_WINDOWS)          // BACKUP PATH

path_t gettempdirw();
path_t gettempprefixw();
backup_path_t gettempdira();
backup_path_t gettempprefixa();

#endif

// TODO: tmpnam??

PYCPP_END_NAMESPACE
