//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Random-access file-backed stream definitions.
 */

#pragma once

#include <pycpp/stream/fd.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Stream wrapping a random-access I/O file.
 */
class random_access_fstream: public std::iostream
{
public:

private:
    fd_streambuf buffer;
};

PYCPP_END_NAMESPACE