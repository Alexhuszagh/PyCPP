//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Temporary file and directory utilities.
 */

#pragma once

#include <pycpp/filesystem/path.h>
#include <pycpp/stream/fd.h>

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

/**
 *  \brief Maximum number of attempted temp paths to get an unused path (100).
 *
 *  \warning This is not like TMP_MAX. If half of the default 3e21
 *  file paths are already exhausted, which would take more than
 *  a 10,000 times the largest storage data array (120 petabytes)
 *  in existence, the chance of failing to produce a unique file path
 *  is 1e-31, practically 0. This is not a bug, it's a feature.
 */
extern size_t TMP_MAX_PATHS;

/**
 *  \brief Number of bytes for the temporary path suffix (15).
 */
extern size_t TMP_SUFFIX_LENGTH;

/**
 *  \brief Characters used for temporary path suffixes.
 */
extern path_t TMP_SUFFIX_CHARACTERS;

// FUNCTIONS
// ---------

/**
 *  \brief Create a temporary file using the given prefix and directory.
 *
 *  The file will automatically be removed when the stream is closed.
 */
fd_stream temporary_file(const path_t& dir = path_t(), const path_t& prefix = path_t());

/**
 *  \brief Create a temporary directory using the given prefix and directory.
 *
 *  The resulting directory's name will be returned so you may remove it.
 */
path_t temporary_directory(const path_t& dir = path_t(), const path_t& prefix = path_t());

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
path_t gettempnam(const path_t& dir = path_t(), const path_t& prefix = path_t());

// tempdir

#if defined(OS_WINDOWS)          // BACKUP PATH

path_t gettempdirw();
path_t gettempprefixw();
path_t gettempnamw(const path_t& dir = path_t(), const path_t& prefix = path_t());
backup_path_t gettempdira();
backup_path_t gettempprefixa();
backup_path_t gettempnama(const backup_path_t& dir = backup_path_t(), const backup_path_t& prefix = backup_path_t());

#endif

PYCPP_END_NAMESPACE
