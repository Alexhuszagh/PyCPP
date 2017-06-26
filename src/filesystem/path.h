//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Path type definitions.
 */

#pragma once

#include "codec.h"
#include "os.h"

#include <string>

// ALIAS
// -----

#if defined(OS_WINDOWS)             // WINDOWS
#   define path_t std::u16string
#   define backup_path_t std::string
#   define path_to_string(s) codec_utf16_utf8(s)
#   define backup_path_to_string(s) (s)
#else                               // POSIX
#   define path_t std::string
#   undef backup_path_t
#   define path_to_string(s) (s)
#endif
