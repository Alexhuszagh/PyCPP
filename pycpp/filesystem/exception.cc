//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem/exception.h>

PYCPP_BEGIN_NAMESPACE

// EXCEPTIONS
// ----------


filesystem_error::filesystem_error(filesystem_code code):
    code_(code)
{}


filesystem_code filesystem_error::code() const
{
    return code_;
}


const char* filesystem_error::what() const throw ()
{
    switch (code_) {
        case filesystem_no_error:
            return "No error.";
        case filesystem_file_not_found:
            return "File not found.";
         case filesystem_invalid_parameter:
            return "Invalid parameter to _stat.";
        case filesystem_out_of_memory:
            return "Out of memory.";
        case filesystem_not_a_symlink:
            return "Not a symbolic link.";
        case filesystem_not_a_file:
            return "Not a file.";
        case filesystem_no_such_directory:
            return "No such directory.";
        case filesystem_destination_exists:
            return "Destination already exists.";
        case filesystem_permissions_error:
            return "Permissions error, cannot open file.";
        case filesystem_too_many_file_descriptors:
            return "Too many open file descriptors by the process.";
        case filesystem_file_descriptor_not_open:
            return "File descriptor is not open.";
        case filesystem_seek_offset_not_valid:
            return "Seek value is negative or otherwise invalid.";
        case filesystem_seek_offset_beyond_file:
            return "Illegal seek past end of file.";
        case filesystem_seek_offset_overflow:
            return "Resulting value cannot be represented by off_t.";
        case filesystem_invalid_seek:
            return "Illegal seek on a non-seekable resource.";
        case filesystem_page_fault:
            return "Page fault on accessing memory-mapped file.";
        case filesystem_unexpected_error:
        default:
            return "Unexpected error in stat.";
    }
}

PYCPP_END_NAMESPACE
