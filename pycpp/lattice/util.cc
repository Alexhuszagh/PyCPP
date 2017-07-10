//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/util.h>

#include <iostream>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

std::mutex LATTICE_MUTEX;

// OBJECTS
// -------


void encryption_warning()
{
    std::lock_guard<std::mutex> lock(LATTICE_MUTEX);
    std::cerr << "Warning: HTTP requests do not support SSL/TLS."
              << "Do NOT send sensitive data without SSL/TLS."
              << std::endl;
}

PYCPP_END_NAMESPACE
