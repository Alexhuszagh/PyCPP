//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Path type definitions.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/string_view.h>
#include <pycpp/string/codec.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(OS_WINDOWS)             // WINDOWS
   using native_char_type = wchar_t;
   using backup_char_type = char;
   using path_t = std::u16string;
   using path_view_t = u16string_view;
   using backup_path_t = std::string;
   using backup_path_view_t = string_view;
   using path_list_t = deque<path_t>;
   using path_view_list_t = deque<path_view_t>;
   using backup_path_list_t = deque<backup_path_t>;
   using backup_path_view_list_t = deque<backup_path_view_t>;
   std::u16string ansi_to_utf16(const string_view&);
   std::string utf16_to_ansi(const u16string_view&);
#   define path_to_string(s) codec_utf16_utf8(s)
#   define backup_path_to_string(s) std::string(s)
#   define path_to_backup_path(s) utf16_to_ansi(s)
#   define backup_path_to_path(s) ansi_to_utf16(s)
#   define string_to_path(s) codec_utf8_utf16(s)
#   define string_to_backup_path(s) (s)
#   define path_prefix(p) u##p
#else                               // POSIX
   using native_char_type = char;
   using path_t = std::string;
   using path_view_t = string_view;
   using path_list_t = deque<path_t>;
   using path_view_list_t = deque<path_view_t>;
#   define path_to_string(s) std::string(s)
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

PYCPP_END_NAMESPACE
