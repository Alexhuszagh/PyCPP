//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows API definitions.
 */

#pragma once

#include <windows.h>

// MACROS
// ------

// winerror.h
#ifndef ERROR_ELEVATION_REQUIRED
#   define ERROR_ELEVATION_REQUIRED 740
#endif

#ifndef ERROR_SYMLINK_NOT_SUPPORTED
#   define ERROR_SYMLINK_NOT_SUPPORTED 1464
#endif

#ifndef ERROR_MUI_FILE_NOT_FOUND
#   define ERROR_MUI_FILE_NOT_FOUND 15100
#endif

#ifndef ERROR_MUI_INVALID_FILE
#   define ERROR_MUI_INVALID_FILE 15101
#endif

#ifndef ERROR_MUI_INVALID_RC_CONFIG
#   define ERROR_MUI_INVALID_RC_CONFIG 15102
#endif

#ifndef ERROR_MUI_INVALID_LOCALE_NAME
#   define ERROR_MUI_INVALID_LOCALE_NAME 15103
#endif

#ifndef ERROR_MUI_INVALID_ULTIMATEFALLBACK_NAME
#   define ERROR_MUI_INVALID_ULTIMATEFALLBACK_NAME 15104
#endif

#ifndef ERROR_MUI_FILE_NOT_LOADED
#   define ERROR_MUI_FILE_NOT_LOADED 15105
#endif
