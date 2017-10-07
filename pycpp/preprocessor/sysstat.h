//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform <sys/stat.h> definitions.
 */

#pragma once

#include <sys/stat.h>

// MACROS
// ------

#ifndef S_IRUSR
#   define S_IRUSR 00400
#endif

#ifndef S_IWUSR
#   define S_IWUSR 00200
#endif
