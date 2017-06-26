//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Filesystem and path utilities.
 */

#pragma once

#include "filesystem/path.h"
#include "filesystem/stat.h"


// FUNCTIONS
// ---------

// STAT

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const path_t& path);

/** \brief Get access time of file, as if by stat.
 */
time_t getatime(const path_t& path);

/** \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const path_t& path);

/** \brief Get created time of file, as if by stat.
 */
time_t getctime(const path_t& path);

/** \brief Get size of file, as if by stat.
 */
off_t getsize(const path_t& path);

/** \brief Check if path points to file.
 */
bool isfile(const path_t& path);

/** \brief Check if path points to directory.
 */
bool isdir(const path_t& path);

/** \brief Check if path points to symbolic link.
 */
bool islink(const path_t& path);

/** \brief Check if path points to a mount point.
 */
//bool ismount(const path_t& path);

/** \brief Check if path exists on filesystem.
 */
bool exists(const path_t& path);

#if defined(backup_path_t)          // BACKUP PATH

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const backup_path_t& path);

/** \brief Get access time of file, as if by stat.
 */
time_t getatime(const backup_path_t& path);

/** \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const backup_path_t& path);

/** \brief Get created time of file, as if by stat.
 */
time_t getctime(const backup_path_t& path);

/** \brief Get size of file, as if by stat.
 */
off_t getsize(const backup_path_t& path);

/** \brief Check if path points to file.
 */
bool isfile(const backup_path_t& path);

/** \brief Check if path points to directory.
 */
bool isdir(const backup_path_t& path);

/** \brief Check if path points to symbolic link.
 */
bool islink(const backup_path_t& path);

/** \brief Check if path points to a mount point.
 */
//bool ismount(const backup_path_t& path);

/** \brief Check if path exists on filesystem.
 */
bool exists(const backup_path_t& path);

#endif
