//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "filesystem/exception.h"

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
         case filesystem_unexpected_error:
         default:
            return "Unexpected error in stat.";
    }
}
