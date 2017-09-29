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
// TODO: tmpnam??
// Implement mkstemp, mkdtemp in terms of tmpnam, but use open(O_EXCL)
// for exclusive to ensure no one can get the file name...

/**
 * \brief Get path to temp directory.
 */
path_t gettempdir();

/**
 * \brief Get the prefix used for named temporary files.
 */
path_t gettempprefix();

/**
 *  \brief Create a temporary filepath, similar to `tempnam`.
 *
 *  This uses the temp directory and temp prefix to create the
 *  full path for the temporary file.
 *
 *  \warning This function is not secure, and should not be used.
 */
path_t gettempnam();

// tempdir

#if defined(OS_WINDOWS)          // BACKUP PATH

path_t gettempdirw();
path_t gettempprefixw();
path_t gettempnamw();
backup_path_t gettempdira();
backup_path_t gettempprefixa();
backup_path_t gettempnama();

#endif

PYCPP_END_NAMESPACE
