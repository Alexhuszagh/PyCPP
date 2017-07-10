//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Socket adaptors.
 */

#pragma once

#include <pycpp/os.h>
#include <pycpp/lattice/adaptor/openssl.h>
#include <pycpp/lattice/adaptor/nossl.h>
#include <pycpp/lattice/adaptor/posix.h>
#include <pycpp/lattice/adaptor/windows.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------


#if defined(PYCPP_HAVE_OPENSSL)
#   define PYCPP_HAVE_SSL
#endif

// OBJECTS
// -------

// HTTP ADAPTERS
#if defined(OS_WINDOWS)
    typedef win32_socket_adaptor_t http_adaptor_t;
#else
    typedef posix_socket_adaptor_t http_adaptor_t;
#endif

// HTTPS ADAPTERS
#if defined(PYCPP_HAVE_SSL) && defined(PYCPP_HAVE_OPENSSL)
    typedef open_ssl_adaptor_t<http_adaptor_t> ssl_adaptor_t;
#else
    typedef no_ssl_adaptor_t<http_adaptor_t> ssl_adaptor_t;
#endif

PYCPP_END_NAMESPACE
