//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/util.h>
#include <pycpp/stl/iostream.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

mutex LATTICE_MUTEX;

// OBJECTS
// -------


void encryption_warning()
{
    lock_guard<mutex> lock(LATTICE_MUTEX);
    cerr << "Warning: HTTP requests do not support SSL/TLS."
              << "Do NOT send sensitive data without SSL/TLS."
              << endl;
}

PYCPP_END_NAMESPACE
