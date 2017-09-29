//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Filesystem and path utilities.
 *
 *  Filesystem normalization and manipulation routines provided
 *  for the default path type (UTF-8 strings on POSIX-like systems,
 *  UTF-16 strings on Windows), as well as for the backup path
 *  type (UTF-8 strings on Windows). The functions are documented
 *  for the default path type, and are otherwise identical except
 *  for the string encoding. You should use the native path
 *  type whenever possible for Unicode support: narrow paths
 *  on Windows do not support Unicode characters.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/filesystem/fd.h>
#include <pycpp/filesystem/home.h>
#include <pycpp/filesystem/iterator.h>
#include <pycpp/filesystem/path.h>
#include <pycpp/filesystem/stat.h>
#include <pycpp/filesystem/tmp.h>
#include <pycpp/iterator/range.h>
#include <ios>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// RUNTIME

/**
 * \brief Get the current working directory.
 */
path_t getcwd();

/** \brief Join POSIX-compliant paths to create path to full file.
 *
 *  \warning Any intermediate elements from starting from the root
 *  will reset the current root.
 *
 *  \code
 *      join("/tmp", "path", "to")          // "/tmp/path/to"
 *      join("/tmp", "/path", "to")         // "/path/to"
 *
 *  \param paths            Paths to join
 *  \return                 Joined paths
 */
path_t join(const path_list_t &paths);

// STAT

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const path_t& path);

/**
 * \brief POSIX-like lstat call.
 */
stat_t lstat(const path_t& path);

/**
 *  \brief Get access time of file, as if by stat.
 */
time_t getatime(const stat_t& s);

/**
 *  \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const stat_t& s);

/**
 *  \brief Get created time of file, as if by stat.
 */
time_t getctime(const stat_t& s);

/**
 *  \brief Get size of file, as if by stat.
 */
off_t getsize(const stat_t& s);

/**
 *  \brief Check if path points to file.
 */
bool isfile(const stat_t& s);

/**
 *  \brief Check if path points to directory.
 */
bool isdir(const stat_t& s);

/**
 *  \brief Check if path points to symbolic link.
 */
bool islink(const stat_t& s);

/**
 *  \brief Check if path exists on filesystem.
 */
bool exists(const stat_t& s);

/**
 *  \brief Check if two stat results point to same file.
 */
bool samestat(const stat_t& s1, const stat_t& s2);

/**
 *  \brief Get access time of file, as if by stat.
 */
time_t getatime(const path_t& path);

/**
 *  \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const path_t& path);

/**
 *  \brief Get created time of file, as if by stat.
 */
time_t getctime(const path_t& path);

/**
 *  \brief Get size of file, as if by stat.
 */
off_t getsize(const path_t& path);

/**
 *  \brief Check if path points to file.
 */
bool isfile(const path_t& path);

/**
 *  \brief Check if path points to directory.
 */
bool isdir(const path_t& path);

/**
 *  \brief Check if path points to symbolic link.
 */
bool islink(const path_t& path);

/**
 *  \brief Check if path exists on filesystem.
 */
bool exists(const path_t& path);

/**
 *  \brief Check if path exists on filesystem as if by lstat.
 */
bool lexists(const path_t& path);

/**
 *  \brief Check if path is absolute.
 */
bool isabs(const path_t& path);

/**
 *  \brief Check if two paths point to same file.
 */
bool samefile(const path_t& p1, const path_t& p2);

// DIRECTORY

/**
 *  \brief Iterate over all items in the directory.
 */
range<directory_iterator> iterdir(const path_t& path);

/**
 *  \brief List all items in the directory.
 */
path_list_t listdir(const path_t& path);

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
path_t read_link(const path_t& path);

/**
 *  \brief Return the absolute path relative to the base.
 */
path_t abspath(const path_t& path);

/**
 *  \brief Return name of file, relative to parent directory.
 */
path_t base_name(const path_t& path);

/**
 *  \brief Return the path of the parent directory.
 */
path_t dir_name(const path_t& path);

/**
 *  \brief Convert first ~ to username if followed by path separator.
 */
path_t expanduser(const path_t& path);

/**
 *  \brief Perform variable expansion for `$name`, `${name}`, and `%name%`.
 */
path_t expandvars(const path_t& path);

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

/**
 *  \brief Convert path to a realpath, converting any symlinks in the process.
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

// MANIPULATION

/**
 *  \brief Copy file metadata from src to dst.
 */
bool copystat(const path_t& src, const path_t& dst);

/**
 *  \brief Make symbolic link pointing to target at dst.
 */
bool mklink(const path_t& target, const path_t& dst, bool replace = false);

/**
 *  \brief Make directory at path, return if successful.
 */
bool mkdir(const path_t& path, int = 0777);

/**
 *  \brief Make directory at path including all parents, return if successful.
 */
bool makedirs(const path_t& path, int = 0777);

/**
 *  \brief Move symbolic link, as if by rename, and return if rename was successful.
 */
bool move_link(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Move file, as if by rename, and return if rename was successful.
 */
bool move_file(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Move directory, as if by rename, and return if rename was successful.
 */
bool move_dir(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Move generic path, and return if move was successful.
 */
bool move_path(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Copy symbolic link from src to dst.
 */
bool copy_link(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Copy file, and copy was successful.
 *
 *  \param replace          Replace dst if it exists.
 */
bool copy_file(const path_t& src, const path_t& dst, bool replace = false);

/**
 *  \brief Copy directory, and copy was successful.
 */
bool copy_dir(const path_t& src, const path_t& dst, bool recursive = true, bool replace = false);

/**
 *  \brief Copy generic path, and return if copy was successful.
 */
bool copy_path(const path_t& src, const path_t& dst, bool recursive = true, bool replace = false);

/**
 *  \brief Remove symbolic link, return if link was successfully removed.
 */
bool remove_link(const path_t& path);

/**
 *  \brief Remove file, return if file was successfully removed.
 */
bool remove_file(const path_t& path);

/**
 *  \brief Remove directory, return if directory was successfully removed.
 */
bool remove_dir(const path_t& path, bool recursive = true);

/**
 *  \brief Remove generic path, and return if successfully removed.
 */
bool remove_path(const path_t& path, bool recursive = true);

// FILE UTILS

/**
 *  \brief Open descriptor to file, as if by POSIX `open()`.
 */
fd_t file_open(const path_t& path, std::ios_base::openmode mode);

/**
 *  \brief Read from descriptor to file, as if by POSIX `read()`.
 */
int64_t file_read(fd_t fd, void* buf, size_t count);

/**
 *  \brief Close descriptor to file, as if by POSIX `close()`.
 */
void file_close(fd_t fd);

/**
 *  \brief Allocate file size to `size` (n bytes), as if by posix_fallocate.
 */
bool file_allocate(fd_t fd, size_t size);

/**
 *  \brief Truncate file size to `size` (n bytes).
 */
bool file_truncate(fd_t fd, size_t size);

/**
 *  \brief Allocate file size to `size` (n bytes), as if by posix_fallocate.
 */
bool file_allocate(const path_t& path, size_t size);

/**
 *  \brief Truncate file size to `size` (n bytes).
 */
bool file_truncate(const path_t& path, size_t size);

#if defined(OS_WINDOWS)          // BACKUP PATH

// RUNTIME

backup_path_t join(const backup_path_list_t &paths);

// STAT

stat_t stat(const backup_path_t& path);
stat_t lstat(const backup_path_t& path);
time_t getatime(const backup_path_t& path);
time_t getmtime(const backup_path_t& path);
time_t getctime(const backup_path_t& path);
off_t getsize(const backup_path_t& path);
bool isfile(const backup_path_t& path);
bool isdir(const backup_path_t& path);
bool islink(const backup_path_t& path);
bool exists(const backup_path_t& path);
bool lexists(const backup_path_t& path);
bool isabs(const backup_path_t& path);
bool samefile(const backup_path_t& p1, const backup_path_t& p2);

// DIRECTORY

range<directory_iterator> iterdir(const backup_path_t& path);
backup_path_list_t listdir(const backup_path_t& path);

// SPLIT

backup_path_list_t split(const backup_path_t& path);
backup_path_list_t splitdrive(const backup_path_t& path);
backup_path_list_t splitext(const backup_path_t& path);
backup_path_list_t splitunc(const backup_path_t& path);

// NORMALIZATION

backup_path_t read_link(const backup_path_t& path);
backup_path_t abspath(const backup_path_t& path);
backup_path_t base_name(const backup_path_t& path);
backup_path_t dir_name(const backup_path_t& path);
backup_path_t expanduser(const backup_path_t& path);
backup_path_t expandvars(const backup_path_t& path);
backup_path_t normcase(const backup_path_t& path);
backup_path_t normpath(const backup_path_t& path);
backup_path_t realpath(const backup_path_t& path);
backup_path_t relpath(const backup_path_t& path);
backup_path_t relpath(const backup_path_t& path, const backup_path_t& start);

// MANIPULATION

bool copystat(const backup_path_t& src, const backup_path_t& dst);
bool mklink(const backup_path_t& target, const backup_path_t& dst, bool replace = false);
bool mkdir(const backup_path_t& path, int = 0777);
bool makedirs(const backup_path_t& path, int = 0777);
bool move_link(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool move_file(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool move_dir(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool move_path(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool copy_link(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool copy_file(const backup_path_t& src, const backup_path_t& dst, bool replace = false);
bool copy_dir(const backup_path_t& src, const backup_path_t& dst, bool recursive = true, bool replace = false);
bool copy_path(const backup_path_t& src, const backup_path_t& dst, bool recursive = true, bool replace = false);
bool remove_link(const backup_path_t& path);
bool remove_file(const backup_path_t& path);
bool remove_dir(const backup_path_t& path, bool recursive = true);
bool remove_path(const backup_path_t& path, bool recursive = true);

// FILE UTILS

fd_t file_open(const backup_path_t& path, std::ios_base::openmode mode);
bool file_allocate(const backup_path_t& path, size_t size);
bool file_truncate(const backup_path_t& path, size_t size);

#endif

PYCPP_END_NAMESPACE
