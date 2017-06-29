//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Filesystem exception definitions.
 */

#pragma once

#include <stdexcept>


// ENUMS
// ----

/**
 *  \brief Enumerate values for errors on the filesystem.
 */
enum filesystem_code
{
    filesystem_no_error = 0,
    filesystem_file_not_found,
    filesystem_invalid_parameter,
    filesystem_out_of_memory,
    filesystem_not_a_symlink,
    filesystem_not_a_file,
    filesystem_no_such_directory,
    filesystem_destination_exists,
    filesystem_unexpected_error,
};

// EXCEPTIONS
// ----------

/**
 *  \brief Base exception class for filesystem errors.
 */
class filesystem_error: std::exception
{
public:
    filesystem_error(filesystem_code code);
    virtual const char* what() const throw ();

    filesystem_code code() const;

private:
    filesystem_code code_;
};
