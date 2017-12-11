//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Compression exception definitions.
 */

#pragma once

#include <pycpp/stl/stdexcept.h>

PYCPP_BEGIN_NAMESPACE

// ENUMS
// ----

/**
 *  \brief Enumerated values for errors on the filesystem.
 */
enum compression_code
{
    compression_no_error = 0,
    compression_config_error,
    compression_invalid_parameter,
    compression_out_of_memory,
    compression_data_error,
    compression_io_error,
    compression_unexpected_eof,
    compression_internal_error,
    compression_unexpected_error,
};


/**
 *  \brief Enumerated return types for a compression/decompression routine.
 */
enum compression_status
{
    compression_ok = 0,
    compression_eof,
    compression_need_input,
    compression_need_output,
};


// EXCEPTIONS
// ----------

/**
 *  \brief Base exception class for compression errors.
 */
class compression_error: std::exception
{
public:
    compression_error(compression_code code);
    virtual const char* what() const throw ();

    compression_code code() const;

private:
    compression_code code_;
};

PYCPP_END_NAMESPACE
