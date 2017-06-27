//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Path type definitions.
 */

#pragma once

#include "codec.h"
#include "os.h"

#include <deque>
#include <string>

// ALIAS
// -----

#if defined(OS_WINDOWS)             // WINDOWS
#   define native_char_type wchar_t
#   define path_t std::u16string
#   define backup_path_t std::string
#   define path_list_t std::deque<path_t>
#   define backup_path_list_t std::deque<backup_path_t>
#   define path_to_string(s) codec_utf16_utf8(s)
#   define backup_path_to_string(s) (s)
#   define path_to_backup_path(s) codec_utf16_utf8(s)
#   define backup_path_to_path(s) codec_utf8_utf16(s)
#   define string_to_path(s) codec_utf8_utf16(s)
#   define string_to_backup_path(s) (s)
#   define path_prefix(p) u##p
#else                               // POSIX
#   define native_char_type char
#   define path_t std::string
#   define path_list_t std::deque<path_t>
#   undef backup_path_t
#   undef backup_path_list_t
#   define path_to_string(s) (s)
#   define string_to_path(s) (s)
#   define path_prefix(p) (p)
#endif

#define path_extension path_prefix('.')
#define current_directory "."
#define parent_directory ".."
#if defined(OS_WINDOWS)                             // WINDOWS
#   define path_separator path_prefix('\\')
#   define path_separators path_t(path_prefix("/\\"))
#elif defined(OS_MSDOS)                             // MSDOS
#   define path_separator path_prefix('\\')
#   define path_separators path_t(path_prefix("\\"))
#elif defined(OS_MACOS) && OS_VERSION_MAJOR == 9    // MAC OS9
#   define path_separator path_prefix(':')
#   define path_separators path_t(path_prefix(":"))
#elif defined(OS_OS2)                               // OS2
#   define path_separator path_prefix('\\')
#   define path_separators path_t(path_prefix("/\\"))
#elif defined(OS_SYMBIAN)                           // SYMBIAN
#   define path_separator path_prefix('\\')
#   define path_separators path_t(path_prefix("\\"))
#elif defined(OS_VMS)                               // VMS
#   define path_separator path_prefix('.')
#   define path_separators path_t(path_prefix("."))
#elif defined(OS_VOS)                               // VOS
#   define path_separator path_prefix('>')
#   define path_separators path_t(path_prefix(">"))
#elif defined(OS_NONSTOP)                           // NONSTOP
#   define path_separator path_prefix('.')
#   define path_separators path_t(path_prefix("."))
#else                                               // POSIX, other
#   define path_separator path_prefix('/')
#   define path_separators path_t(path_prefix("/"))
#endif
