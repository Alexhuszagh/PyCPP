//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Directory iterator implementation.
 *
 *  Iterator facade over the contents of a directory. Analogous to
 *  std::filesystem::directory_iterator.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/filesystem/path.h>
#include <pycpp/filesystem/stat.h>
#include <iterator>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct directory_data;
struct directory_entry;
struct directory_iterator;
struct recursive_directory_data;
struct recursive_directory_entry;
struct recursive_directory_iterator;

// ALIAS
// -----

using directory_data_ptr = std::shared_ptr<directory_data>;
using recursive_directory_data_ptr = std::shared_ptr<recursive_directory_data>;

// OBJECTS
// -------


/**
 *  \brief Entry for item in directory.
 */
struct directory_entry
{
public:
    // MEMBER TYPES
    // ------------
    typedef directory_entry self_t;

    // MEMBER FUNCTIONS
    // ----------------

    // PATHS
    path_t path() const;
    path_t basename() const;
    const path_t& dirname() const;

    // STAT
    const stat_t& stat() const;
    bool isfile() const;
    bool isdir() const;
    bool islink() const;
    bool exists() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    friend struct directory_data;
    friend struct directory_iterator;

    directory_data_ptr ptr_;
};


/**
 *  \brief Iterate over all nodes in the current directory.
 *
 *  Internally, a shared_ptr is used to simplify the copy mechanics
 *  required for input iterators.
 */
struct directory_iterator: std::iterator<std::input_iterator_tag, directory_entry>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, directory_entry> base_t;
    typedef directory_iterator self_t;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    directory_iterator() = default;
    directory_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    directory_iterator(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
    ~directory_iterator();

    directory_iterator(const path_view_t& path);
#if defined(OS_WINDOWS)
    directory_iterator(const backup_path_view_t& path);
#endif

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    pointer operator->();
    const_pointer operator->() const;
    reference operator*();
    const_reference operator*() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    directory_entry entry_;
};


/**
 *  \brief Entry for item in directory using recursive semantiocs.
 */
struct recursive_directory_entry
{
public:
    // MEMBER TYPES
    // ------------
    typedef recursive_directory_entry self_t;

    // MEMBER FUNCTIONS
    // ----------------

    // PATHS
    path_t path() const;
    path_t basename() const;
    const path_t& dirname() const;

    // STAT
    const stat_t& stat() const;
    bool isfile() const;
    bool isdir() const;
    bool islink() const;
    bool exists() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    friend struct recursive_directory_data;
    friend struct recursive_directory_iterator;

    recursive_directory_data_ptr ptr_;
};


/**
 *  \brief Recursively iterate over elements starting from the current directory.
 *
 *  Recursively iterate over all items in the current directory and
 *  subdirectories. Internally, recursive_directory_iterator is implemented
 *  as directory_iterator while descending into every child directory on
 *  incrementing the iterator.
 */
struct recursive_directory_iterator: std::iterator<std::input_iterator_tag, recursive_directory_entry>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, recursive_directory_entry> base_t;
    typedef recursive_directory_iterator self_t;
    using typename base_t::value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------
    recursive_directory_iterator() = default;
    recursive_directory_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    recursive_directory_iterator(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
    ~recursive_directory_iterator();

    recursive_directory_iterator(const path_view_t& path);
#if defined(OS_WINDOWS)
    recursive_directory_iterator(const backup_path_view_t& path);
#endif

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    pointer operator->();
    const_pointer operator->() const;
    reference operator*();
    const_reference operator*() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    recursive_directory_entry entry_;
};

PYCPP_END_NAMESPACE
