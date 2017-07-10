//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP/HTTPS request.
 */

#include <pycpp/lattice/url.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Proxy to tunnel communications to server.
 */
struct proxy_t: url_t
{
    using url_t::url_t;

    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
