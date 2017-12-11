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
 *  This is analogous to mkstemp, and the file is opened
 *  with read/write access that only the current user can access.
 *  The file is opened exclusively, and cannot be shared by other
 *  processes for security reasons.
 */
fd_stream temporary_file(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());

/**
 *  \brief Create a temporary directory using the given prefix and directory.
 *
 *  The resulting directory's name will be returned so you may remove it.
 *  This is analogous to mkdtemp, and the directory may only be accessed
 *  by the current user.
 */
path_t temporary_directory(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());

/**
 * \brief Get path to temp directory.
 */
path_view_t gettempdir();

/**
 * \brief Get the prefix used for named temporary files.
 */
path_view_t gettempprefix();

/**
 *  \brief Create a temporary filepath, similar to `tempnam`.
 *
 *  This uses the temp directory and temp prefix to create the
 *  full path for the temporary file.
 *
 *  \warning This function is not secure, and should not be used.
 */
path_t gettempnam(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());

#if defined(OS_WINDOWS)          // BACKUP PATH

fd_stream temporary_filew(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());
path_t temporary_directoryw(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());
path_view_t gettempdirw();
path_view_t gettempprefixw();
path_t gettempnamw(const path_view_t& dir = path_view_t(), const path_view_t& prefix = path_view_t());
fd_stream temporary_filea(const backup_path_t& dir = backup_path_t(), const backup_path_t& prefix = backup_path_t());
backup_path_t temporary_directorya(const backup_path_view_t& dir = backup_path_view_t(), const backup_path_view_t& prefix = backup_path_view_t());
backup_path_view_t gettempdira();
backup_path_view_t gettempprefixa();
backup_path_t gettempnama(const backup_path_view_t& dir = backup_path_view_t(), const backup_path_view_t& prefix = backup_path_view_t());

#endif

PYCPP_END_NAMESPACE
