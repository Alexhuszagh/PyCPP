//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/exception.h>

PYCPP_BEGIN_NAMESPACE

// EXCEPTIONS
// ----------


compression_error::compression_error(compression_code code) noexcept:
    code_(code)
{}


compression_code compression_error::code() const noexcept
{
    return code_;
}


const char* compression_error::what() const noexcept
{
    switch (code_) {
        case compression_no_error:
            return "No error.";
        case compression_config_error:
            return "Configuration error in compression library.";
         case compression_invalid_parameter:
            return "Invalid parameter to compression routine.";
        case compression_out_of_memory:
            return "Out of memory.";
        case compression_data_error:
            return "Invalid stream detected.";
        case compression_io_error:
            return "Unknown I/O error.";
        case compression_unexpected_eof:
            return "Unexpected EOF for file.";
        case compression_internal_error:
            return "Internal error";
        case compression_unexpected_error:
        default:
            return "Unexpected error in compression/decompression.";
    }
}

PYCPP_END_NAMESPACE
