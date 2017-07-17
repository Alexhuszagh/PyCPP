//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Configurations for PyCPP.
 */

#pragma once

// MACROS
// ------

#ifndef PYCPP_NAMESPACE
#   define PYCPP_NAMESPACE
#endif

#define PYCPP_DO_EXPAND(VAL)  VAL ## 1
#define PYCPP_EXPAND(VAL)     PYCPP_DO_EXPAND(VAL)


#if PYCPP_EXPAND(PYCPP_NAMESPACE) == 1      // NO NAMESPACE
#   define PYCPP_BEGIN_NAMESPACE
#   define PYCPP_END_NAMESPACE
#   define PYCPP_USING_NAMESPACE
#   undef PYCPP_HAVE_NAMESPACE
#else                                       // HAVE NAMESPACE
#   define PYCPP_BEGIN_NAMESPACE namespace PYCPP_NAMESPACE {
#   define PYCPP_END_NAMESPACE }
#   define PYCPP_USING_NAMESPACE using namespace PYCPP_NAMESPACE;
#   define PYCPP_HAVE_NAMESPACE
#endif
