//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Cross-compiler thread-local storage.
 */

#pragma once

// MACROS
// ------

//#if defined(__MINGW32__) || defined(__APPLE__)
//#   define USE_CUSTOM_TLS
////#   include <boost/thread/tss.hpp>
//#elif (_MSC_VER >= 1400)
//# ifndef thread_local
//#   define thread_local __declspec(thread)
//# endif
//#else
//#   define USE_CXX11_TLS
//#endif

// CLEANUP
// -------

#undef USE_CUSTOM_TLS
#undef USE_CXX11_TLS
