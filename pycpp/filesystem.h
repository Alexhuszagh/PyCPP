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
 *
 *  UTF-8 strings are represented by `std::string`, while UTF-16
 *  strings are represented by `std::u16string`. However,
 *  for compatibility reasons with the Win32 API, all stream
 *  constructors also accept `std::wstring` as paths.
 */

#pragma once

#include <pycpp/filesystem/access.h>
#include <pycpp/filesystem/fd.h>
#include <pycpp/filesystem/home.h>
#include <pycpp/filesystem/iterator.h>
#include <pycpp/filesystem/path.h>
#include <pycpp/filesystem/stat.h>
#include <pycpp/filesystem/tmp.h>
#include <pycpp/iterator/range.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/ios.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

/**
 *  \brief Read/write permission for user/group.
 */
extern mode_t S_IWR_USR_GRP;

/**
 *  \brief Read/write/execute permission for user/group.
 */
extern mode_t S_IWRX_USR_GRP;

/**
 *  \brief Read/write permission for user/group/other.
 */
extern mode_t S_IWR_USR_GRP_OTH;

/**
 *  \brief Read/write/execute permission for user/group/other.
 */
extern mode_t S_IWRX_USR_GRP_OTH;

// FUNCTIONS
// ---------

// RUNTIME

/**
 * \brief Get the current working directory.
 */
path_t getcwd();

/**
 *  \brief Join POSIX-compliant paths to create path to full file.
 *
 *  \warning Any intermediate elements from starting from the root
 *  will reset the current root.
 *
 *  \code
 *      join_path("/tmp", "path", "to")         // "/tmp/path/to"
 *      join_path("/tmp", "/path", "to")        // "/path/to"
 *
 *  \param paths            Paths to join
 *  \return                 Joined paths
 */
path_t join_path(initializer_list<path_view_t> paths);
path_t join_path(const path_list_t &paths);
path_t join_path(const path_view_list_t &paths);

// STAT

/**
 * \brief POSIX-like stat call.
 */
stat_t stat(const path_view_t& path);

/**
 * \brief POSIX-like lstat call.
 */
stat_t lstat(const path_view_t& path);

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
time_t getatime(const path_view_t& path);

/**
 *  \brief Get modified time of file, as if by stat.
 */
time_t getmtime(const path_view_t& path);

/**
 *  \brief Get created time of file, as if by stat.
 */
time_t getctime(const path_view_t& path);

/**
 *  \brief Get size of file, as if by stat.
 */
off_t getsize(const path_view_t& path);

/**
 *  \brief Check if path points to file.
 */
bool isfile(const path_view_t& path);

/**
 *  \brief Check if path points to directory.
 */
bool isdir(const path_view_t& path);

/**
 *  \brief Check if path points to symbolic link.
 */
bool islink(const path_view_t& path);

/**
 *  \brief Check if path exists on filesystem.
 */
bool exists(const path_view_t& path);

/**
 *  \brief Check if path exists on filesystem as if by lstat.
 */
bool lexists(const path_view_t& path);

/**
 *  \brief Check if path is absolute.
 */
bool isabs(const path_view_t& path);

/**
 *  \brief Check if two paths point to same file.
 */
bool samefile(const path_view_t& p1, const path_view_t& p2);

// DIRECTORY

/**
 *  \brief Iterate over all items in the directory.
 */
range<directory_iterator> iterdir(const path_view_t& path);

/**
 *  \brief List all items in the directory.
 */
path_list_t listdir(const path_view_t& path);

// SPLIT

/**
 *  \brief Split path into head and tail using last pathname component.
 */
path_view_list_t path_split(const path_view_t& path);

/**
 *  \brief Split path into drive and tail components.
 */
path_view_list_t path_splitdrive(const path_view_t& path);

/**
 *  \brief Split path into root and filename extension components.
 */
path_view_list_t path_splitext(const path_view_t& path);

/**
 *  \brief Split path into UNC mount point and tail components.
 */
path_view_list_t path_splitunc(const path_view_t& path);

// NORMALIZATION

/**
 *  \brief Read value of a symlink.
 */
path_t read_link(const path_view_t& path);

/**
 *  \brief Return the absolute path relative to the base.
 */
path_t abspath(const path_view_t& path);

/**
 *  \brief Return name of file, relative to parent directory.
 */
path_view_t base_name(const path_view_t& path);

/**
 *  \brief Return the path of the parent directory.
 */
path_view_t dir_name(const path_view_t& path);

/**
 *  \brief Convert first ~ to username if followed by path separator.
 */
path_t expanduser(const path_view_t& path);

/**
 *  \brief Perform variable expansion for `$name`, `${name}`, and `%name%`.
 */
path_t expandvars(const path_view_t& path);

/**
 *  \brief Collapse redundant relative and normalize case.
 *  \warning This method will not preserve path containing symlinks.
 */
path_t normcase(const path_view_t& path);

/**
 *  \brief Collapse redundant relative references.
 *
 *  Convert a path so redundant "." and ".." references are removed,
 *  as well as converting any secondary directory separators to
 *  the primary directory separator ("/" to "\" on Windows).
 *
 *  \warning This method will not preserve path containing symlinks.
 */
path_t normpath(const path_view_t& path);

/**
 *  \brief Convert path to a realpath, converting any symlinks in the process.
 *
 *  In order for relative path operators to properly work ("." and ".."),
 *  the path is read from the topmost directory down.
 */
path_t realpath(const path_view_t& path);

/**
 *  \brief Get a relative path from the current working directory.
 */
path_t relpath(const path_view_t& path);

/**
 *  \brief Get a relative path from start.
 */
path_t relpath(const path_view_t& path, const path_view_t& start);

// MANIPULATION

/**
 *  \brief Copy file metadata from src to dst.
 */
bool copystat(const path_view_t& src, const path_view_t& dst);

/**
 *  \brief Make symbolic link pointing to target at dst.
 */
bool mklink(const path_view_t& target, const path_view_t& dst, bool replace = false);

/**
 *  \brief Make directory at path, return if successful.
 */
bool mkdir(const path_view_t& path, int = 0777);

/**
 *  \brief Make directory at path including all parents, return if successful.
 */
bool makedirs(const path_view_t& path, int = 0777);

/**
 *  \brief Move symbolic link, as if by rename, and return if rename was successful.
 */
bool move_link(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Move file, as if by rename, and return if rename was successful.
 */
bool move_file(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Move directory, as if by rename, and return if rename was successful.
 */
bool move_dir(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Move generic path, and return if move was successful.
 */
bool move_path(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Copy symbolic link from src to dst.
 */
bool copy_link(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Copy file, and copy was successful.
 *
 *  \param replace          Replace dst if it exists.
 */
bool copy_file(const path_view_t& src, const path_view_t& dst, bool replace = false);

/**
 *  \brief Copy directory, and copy was successful.
 */
bool copy_dir(const path_view_t& src, const path_view_t& dst, bool recursive = true, bool replace = false);

/**
 *  \brief Copy generic path, and return if copy was successful.
 */
bool copy_path(const path_view_t& src, const path_view_t& dst, bool recursive = true, bool replace = false);

/**
 *  \brief Remove symbolic link, return if link was successfully removed.
 */
bool remove_link(const path_view_t& path);

/**
 *  \brief Remove file, return if file was successfully removed.
 */
bool remove_file(const path_view_t& path);

/**
 *  \brief Remove directory, return if directory was successfully removed.
 */
bool remove_dir(const path_view_t& path, bool recursive = true);

/**
 *  \brief Remove generic path, and return if successfully removed.
 */
bool remove_path(const path_view_t& path, bool recursive = true);

// FILE UTILS

/**
 *  \brief Open descriptor to file, as if by POSIX `open()`.
 */
fd_t fd_open(const path_view_t& path, ios_base::openmode openmode, mode_t permission = S_IWR_USR_GRP, io_access_pattern access = access_normal);

/**
 *  \brief Read from descriptor, as if by POSIX `read()`.
 */
streamsize fd_read(fd_t fd, void* buf, streamsize count);

/**
 *  \brief Write to descriptor, as if by POSIX `write()`.
 */
streamsize fd_write(fd_t fd, const void* buf, streamsize count);

/**
 *  \brief Seek position in stream, as if by POSIX `lseek()`.
 */
streampos fd_seek(fd_t fd, streamoff off, ios_base::seekdir way = ios_base::beg);

/**
 *  \brief Tell current position in stream, as if by POSIX `lseek()`.
 */
streampos fd_tell(fd_t fd);

/**
 *  \brief Close descriptor, as if by POSIX `close()`.
 *  \warning `fd_close` returns any errors in closing the file, but
 *  you are recommended to general ignore them.
 *  See: https://lwn.net/Articles/576478/
 */
int fd_close(fd_t fd);

/**
 *  \brief Change file permissions, as if by `fchmod()`.
 */
int fd_chmod(fd_t fd, mode_t permissions);

/**
 *  \brief Allocate file size to `size` (n bytes), as if by posix_fallocate.
 */
int fd_allocate(fd_t fd, streamsize size);

/**
 *  \brief Truncate file size to `size` (n bytes).
 */
int fd_truncate(fd_t fd, streamsize size);

/**
 *  \brief Change file permissions, as if by `fchmod()`.
 */
int fd_chmod(const path_view_t& path, mode_t permissions);

/**
 *  \brief Allocate file size to `size` (n bytes), as if by posix_fallocate.
 */
int fd_allocate(const path_view_t& path, streamsize size);

/**
 *  \brief Truncate file size to `size` (n bytes).
 */
int fd_truncate(const path_view_t& path, streamsize size);

#if defined(OS_WINDOWS)          // BACKUP PATH

// RUNTIME

backup_path_t join_path(initializer_list<backup_path_view_t> paths);
backup_path_t join_path(const backup_path_list_t &paths);
backup_path_t join_path(const backup_path_view_list_t &paths);

// STAT

stat_t stat(const backup_path_view_t& path);
stat_t lstat(const backup_path_view_t& path);
time_t getatime(const backup_path_view_t& path);
time_t getmtime(const backup_path_view_t& path);
time_t getctime(const backup_path_view_t& path);
off_t getsize(const backup_path_view_t& path);
bool isfile(const backup_path_view_t& path);
bool isdir(const backup_path_view_t& path);
bool islink(const backup_path_view_t& path);
bool exists(const backup_path_view_t& path);
bool lexists(const backup_path_view_t& path);
bool isabs(const backup_path_view_t& path);
bool samefile(const backup_path_view_t& p1, const backup_path_view_t& p2);

// DIRECTORY

range<directory_iterator> iterdir(const backup_path_view_t& path);
backup_path_list_t listdir(const backup_path_view_t& path);

// SPLIT

backup_path_view_list_t path_split(const backup_path_view_t& path);
backup_path_view_list_t path_splitdrive(const backup_path_view_t& path);
backup_path_view_list_t path_splitext(const backup_path_view_t& path);
backup_path_view_list_t path_splitunc(const backup_path_view_t& path);

// NORMALIZATION

backup_path_t read_link(const backup_path_view_t& path);
backup_path_t abspath(const backup_path_view_t& path);
backup_path_view_t base_name(const backup_path_view_t& path);
backup_path_view_t dir_name(const backup_path_view_t& path);
backup_path_t expanduser(const backup_path_view_t& path);
backup_path_t expandvars(const backup_path_view_t& path);
backup_path_t normcase(const backup_path_view_t& path);
backup_path_t normpath(const backup_path_view_t& path);
backup_path_t realpath(const backup_path_view_t& path);
backup_path_t relpath(const backup_path_view_t& path);
backup_path_t relpath(const backup_path_view_t& path, const backup_path_view_t& start);

// MANIPULATION

bool copystat(const backup_path_view_t& src, const backup_path_view_t& dst);
bool mklink(const backup_path_view_t& target, const backup_path_view_t& dst, bool replace = false);
bool mkdir(const backup_path_view_t& path, int = 0777);
bool makedirs(const backup_path_view_t& path, int = 0777);
bool move_link(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool move_file(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool move_dir(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool move_path(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool copy_link(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool copy_file(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace = false);
bool copy_dir(const backup_path_view_t& src, const backup_path_view_t& dst, bool recursive = true, bool replace = false);
bool copy_path(const backup_path_view_t& src, const backup_path_view_t& dst, bool recursive = true, bool replace = false);
bool remove_link(const backup_path_view_t& path);
bool remove_file(const backup_path_view_t& path);
bool remove_dir(const backup_path_view_t& path, bool recursive = true);
bool remove_path(const backup_path_view_t& path, bool recursive = true);

// FILE UTILS

fd_t fd_open(const backup_path_view_t& path, ios_base::openmode openmode, mode_t permission = S_IWR_USR_GRP, io_access_pattern access = access_normal);
int fd_chmod(const backup_path_view_t& path, mode_t permissions);
int fd_allocate(const backup_path_view_t& path, streamsize size);
int fd_truncate(const backup_path_view_t& path, streamsize size);

#endif

PYCPP_END_NAMESPACE
