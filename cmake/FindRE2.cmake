#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindRE2
# --------
#
# Find RE2 include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(RE2
#     [REQUIRED]             # Fail with error if RE2 is not found
#     )
#
# You may also set `RE2_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `RE2_FOUND` will be set to true, and `RE2_LIBRARIES`
# and `RE2_INCLUDE_DIRS` will both be set.
#
# You may optionally set `RE2_ROOT` to specify a custom root directory
# for the RE2 installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(RE2_SEARCH_PATHS)

if(RE2_ROOT)
    list(APPEND RE2_SEARCH_PATHS ${RE2_ROOT})
endif()

if(WIN32)
    list(APPEND RE2_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/re2"
    )
endif()

unset(RE2_SYSTEM_ROOT)
unset(RE2_CUSTOM_ROOT)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(RE2)
foreach(search ${RE2_SEARCH_PATHS})
    FIND_PATH(RE2_INCLUDE_DIR
        NAMES re2/re2.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT RE2_INCLUDE_DIR)
    FIND_PATH(RE2_INCLUDE_DIR re2/re2.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(RE2_LIBRARY_NAMES re2)

foreach(search ${RE2_SEARCH_PATHS})
    FIND_LIBRARY(RE2_LIBRARY
        NAMES ${RE2_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT RE2_LIBRARY)
    FIND_LIBRARY(RE2_LIBRARY NAMES ${RE2_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(RE2_INCLUDE_DIRS ${RE2_INCLUDE_DIR})
set(RE2_LIBRARIES ${RE2_LIBRARY})

CheckFound(RE2)
FindStaticLibs(RE2)

# COMPILATION
# -----------

set(RE2_CODE "
#include <re2/re2.h>
int main(int argc, char *argv[])
{
    re2::RE2* re2 = nullptr;
    return 0;
}
"
)

if(RE2_FOUND)
    CheckCompiles(RE2)
endif()
RequiredPackageFound(RE2)