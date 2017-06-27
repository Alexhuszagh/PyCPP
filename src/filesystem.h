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

// RUNTIME

/**
 * \brief Get the current working directory.
 */
path_t getcwd();

// STAT

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const path_t& path);

/**
 * \brief POSIX-like lstat call.
 */
stat_t lstat(const path_t& path);

/** \brief Get access time of file, as if by stat.
 */
time_t getatime(const stat_t& s);

/** \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const stat_t& s);

/** \brief Get created time of file, as if by stat.
 */
time_t getctime(const stat_t& s);

/** \brief Get size of file, as if by stat.
 */
off_t getsize(const stat_t& s);

/** \brief Check if path points to file.
 */
bool isfile(const stat_t& s);

/** \brief Check if path points to directory.
 */
bool isdir(const stat_t& s);

/** \brief Check if path points to symbolic link.
 */
bool islink(const stat_t& s);

/** \brief Check if path exists on filesystem.
 */
bool exists(const stat_t& s);

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

/** \brief Check if path exists on filesystem.
 */
bool exists(const path_t& path);

/**
 *  \brief Check if path is absolute.
 */
bool isabs(const path_t& path);

/**
 *  \brief Check if two paths point to same file.
 */
bool samefile(const path_t& p1, const path_t& p2);

/**
 *  \brief Check if two stat results point to same file.
 */
bool samestat(const stat_t& s1, const stat_t& s2);

// SPLIT

/**
 *  \brief Split path into head and tail using last pathname component.
 */
path_list_t split(const path_t& path);

/**
 *  \brief Split path into drive and tail components.
 */
path_list_t splitdrive(const path_t& path);

/**
 *  \brief Split path into root and filename extension components.
 */
path_list_t splitext(const path_t& path);

/**
 *  \brief Split path into UNC mount point and tail components.
 */
path_list_t splitunc(const path_t& path);

// NORMALIZATION

/**
 *  \brief Read value of a symlink.
 */
path_t readlink(const path_t& path);

/**
 *  \brief Return the absolute path relative to the base.
 */
path_t abspath(const path_t& path);

/**
 *  \brief Return name of file, relative to parent directory.
 */
path_t basename(const path_t& path);

/**
 *  \brief Return the path of the parent directory.
 */
path_t dirname(const path_t& path);

/**
 *  \brief Convert first ~ to username if followed by path separator.
 */
path_t expanduser(const path_t& path);

// TODO: need to implement...
//path_t expandvars(const path_t& path);

/**
 *  \brief Collapse redundant relative and normalize case.
 *  \warning This method will not preserve path containing symlinks.
 */
path_t normcase(const path_t& path);

/**
 *  \brief Collapse redundant relative references.
 *
 *  Convert a path so redundant "." and ".." references are removed,
 *  as well as converting any secondary directory separators to
 *  the primary directory separator ("/" to "\" on Windows).
 *
 *  \warning This method will not preserve path containing symlinks.
 */
path_t normpath(const path_t& path);

/** \brief Convert path to a realpath, converting any symlinks in the process.
 *
 *  In order for relative path operators to properly work ("." and ".."),
 *  the path is read from the topmost directory down.
 */
path_t realpath(const path_t& path);

/**
 *  \brief Get a relative path from the current working directory.
 */
path_t relpath(const path_t& path);

/**
 *  \brief Get a relative path from start.
 */
path_t relpath(const path_t& path, const path_t& start);

#if defined(backup_path_t)          // BACKUP PATH

// STAT

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const backup_path_t& path);

/**
 * \brief POSIX-like lstat call.
 */
stat_t lstat(const backup_path_t& path);

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

/** \brief Check if path exists on filesystem.
 */
bool exists(const backup_path_t& path);

// NORMALIZATION

/**
 *  \brief Read value of a symlink.
 */
backup_path_t readlink(const backup_path_t& path);

// TODO: restore
///** \brief Return the absolute path relative to the base.
// */
//backup_path_t abspath(const backup_path_t& path);
//
///** \brief Return name of file, relative to parent directory.
// */
//backup_path_t basename(const backup_path_t& path);
//
///** \brief Return the path of the parent directory.
// */
//backup_path_t dirname(const backup_path_t& path);
//
//// TODO: need to have tge home and temp directories
//// Need to access them reliably, in a cross-platform manner
////backup_path_t expanduser(const backup_path_t& path);
////backup_path_t expandvars(const backup_path_t& path);
//
////backup_path_t normcase(const backup_path_t& path);
//
///** \brief Collapse redundant relative references.
// *
// *  Convert a path so redundant "." and ".." references are removed,
// *  as well as converting any secondary directory separators to
// *  the primary directory separator ("/" to "\" on Windows).
// *
// *  \warning This method will not preserve path containing symlinks.
// */
//backup_path_t normpath(const backup_path_t& path);
//
//// TODO: this is going to need to identify any symbolic links...
///** \brief Convert path to a realpath, converting any symlinks in the process.
// *
// *  In order for relative path operators to properly work ("." and ".."),
// *  the path is read from the topmost directory down.
// */
//backup_path_t realpath(const backup_path_t& path);
//
////backup_path_t relpath(const backup_path_t& path);
////backup_path_t relpath(const backup_path_t& path, const backup_path_t& start);
//// samefile         -- Use equivalent
//// samestat
//// sameopenfile
//// split
//// splitdrive
//// splitunc
//// TODO:

#endif


