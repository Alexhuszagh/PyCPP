//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Socket adaptors.
 */

#pragma once

#include <pycpp/lattice/adaptor/openssl.h>
#include <pycpp/lattice/adaptor/nossl.h>
#include <pycpp/lattice/adaptor/posix.h>
#include <pycpp/lattice/adaptor/windows.h>
#include <pycpp/preprocessor/os.h>

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
    using http_adaptor_t = win32_socket_adaptor_t;
#else
    using http_adaptor_t = posix_socket_adaptor_t;
#endif

// HTTPS ADAPTERS
#if defined(PYCPP_HAVE_SSL) && defined(PYCPP_HAVE_OPENSSL)
    using ssl_adaptor_t = open_ssl_adaptor_t<http_adaptor_t>;
#else
    using ssl_adaptor_t = no_ssl_adaptor_t<http_adaptor_t>;
#endif

PYCPP_END_NAMESPACE
