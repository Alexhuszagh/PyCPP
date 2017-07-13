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
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// TODO: need to implement...
// TODO: this should be std::input_iterator

struct directory_entry
{
private:
};


/**
 *  \brief Iterate over all nodes in the current directory.
 */
struct directory_iterator: std::iterator<std::input_iterator_tag, directory_entry>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, directory_entry> base;
    typedef directory_iterator self;
    using typename base::value_type;

    // MEMBER FUNCTIONS
    // ----------------
    directory_iterator() = default;
    ~directory_iterator();

private:
    void *ptr_ = nullptr;
};


/**
 *  \brief Recursively iterate over elements starting from the current directory.
 *
 *  Recursively iterate over all items in the current directory and
 *  subdirectories. Internally, recursive_directory_iterator is implemented
 *  as directory_iterator while descending into every child directory on
 *  incrementing the iterator.
 */
struct recursive_directory_iterator: std::iterator<std::input_iterator_tag, directory_entry>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, directory_entry> base;
    typedef recursive_directory_iterator self;
    using typename base::value_type;

    // MEMBER FUNCTIONS
    // ----------------
    recursive_directory_iterator() = default;
    ~recursive_directory_iterator();

private:
    void *ptr_ = nullptr;
};

PYCPP_END_NAMESPACE
