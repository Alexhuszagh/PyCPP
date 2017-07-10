//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Generic lattice utilities.
 */

#pragma once

#include <pycpp/config.h>
#include <mutex>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

extern std::mutex LATTICE_MUTEX;

// FUNCTIONS
// ---------

/** \brief Warn user about user encryption methods without SSL/TLS support.
 */
void encryption_warning();

// TYPES
// -----


/**
 *  \brief Checks if a class is derived from a template class.
 */
template <template <typename...> class C, typename... T>
class is_derived
{
protected:
    template <typename... U> static char &test(C<U...> const &);
    template <typename... U> static long &test(...);

public:
    enum {
        value = sizeof(test(std::declval<T...>())) == sizeof(char)
    };
};


// MEMBERS
// -------


/**
 *  \brief Check if class has generic method.
 *
 *  Uses Substitution Failure Is Not An Error (SFINAE) to determine
 *  if the class has a member function with the given name.
 */
#define PYCPP_HAS_MEMBER_FUNCTION(func, name)                                \
    template<typename T>                                                     \
    class name                                                               \
    {                                                                        \
    protected:                                                               \
        template <typename C> static char &test(decltype(&C::func));         \
        template <typename C> static long &test(...);                        \
                                                                             \
    public:                                                                  \
        enum {                                                               \
            value = sizeof(test<T>(0)) == sizeof(char)                       \
        };                                                                   \
    };                                                                       \

// FUNCTIONS
// ---------


PYCPP_HAS_MEMBER_FUNCTION(set_reuse_address, has_set_reuse_address);
PYCPP_HAS_MEMBER_FUNCTION(set_timeout, has_set_timeout);
PYCPP_HAS_MEMBER_FUNCTION(set_certificate_file, has_set_certificate_file);
PYCPP_HAS_MEMBER_FUNCTION(set_revocation_lists, has_set_revocation_lists);
PYCPP_HAS_MEMBER_FUNCTION(set_ssl_protocol, has_set_ssl_protocol);
PYCPP_HAS_MEMBER_FUNCTION(set_verify_peer, has_set_verify_peer);

// CLEANUP
// -------

#undef PYCPP_HAS_MEMBER_FUNCTION

PYCPP_END_NAMESPACE
