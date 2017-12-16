//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Generic lattice utilities.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/sfinae/is_specialization.h>
#include <pycpp/stl/mutex.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

extern mutex LATTICE_MUTEX;

// FUNCTIONS
// ---------

/**
 *  \brief Warn user about user encryption methods without SSL/TLS support.
 */
void encryption_warning();

// FUNCTIONS
// ---------

PYCPP_HAS_MEMBER_FUNCTION(set_reuse_address, has_set_reuse_address);
PYCPP_HAS_MEMBER_FUNCTION(set_timeout, has_set_timeout);
PYCPP_HAS_MEMBER_FUNCTION(set_certificate_file, has_set_certificate_file);
PYCPP_HAS_MEMBER_FUNCTION(set_revocation_lists, has_set_revocation_lists);
PYCPP_HAS_MEMBER_FUNCTION(set_ssl_protocol, has_set_ssl_protocol);
PYCPP_HAS_MEMBER_FUNCTION(set_verify_peer, has_set_verify_peer);

PYCPP_END_NAMESPACE
