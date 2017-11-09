#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindLZMA
# ---------
#
# Find LZMA include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(LZMA
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if LZMA is not found
#     )
#
# You may also set `LZMA_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `LZMA_FOUND` will be set to true, and `LZMA_LIBRARIES`
# and `LZMA_INCLUDE_DIRS` will both be set.
#
# You may optionally set `LZMA_ROOT` to specify a custom root directory
# for the LZMA installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(LZMA_SEARCH_PATHS)

if(LZMA_ROOT)
    list(APPEND LZMA_SEARCH_PATHS ${LZMA_ROOT})
endif()

if(WIN32)
    list(APPEND LZMA_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/xz"
        "$ENV{PROGRAMFILES}/lzma"
    )
endif()

unset(LZMA_SYSTEM_ROOT)
unset(LZMA_CUSTOM_ROOT)
unset(LZMA_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(LZMA)
foreach(search ${LZMA_SEARCH_PATHS})
    FIND_PATH(LZMA_INCLUDE_DIR
        NAMES lzma.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT LZMA_INCLUDE_DIR)
    FIND_PATH(LZMA_INCLUDE_DIR lzma.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(LZMA_LIBRARY_NAMES lzma)

foreach(search ${LZMA_SEARCH_PATHS})
    FIND_LIBRARY(LZMA_LIBRARY
        NAMES ${LZMA_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT LZMA_LIBRARY)
    FIND_LIBRARY(LZMA_LIBRARY NAMES ${LZMA_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(LZMA_INCLUDE_DIRS ${LZMA_INCLUDE_DIR})
set(LZMA_LIBRARIES ${LZMA_LIBRARY})

CheckFound(LZMA)
FindStaticLibs(LZMA)

# VERSION
# -------

if(LZMA_FOUND)
    file(STRINGS "${LZMA_INCLUDE_DIRS}/lzma/version.h" LZMA_VERSION_CONTENTS REGEX "#define LZMA_VERSION_[A-Z]+")
    string(REGEX REPLACE ".*LZMA_VERSION_MAJOR ([0-9]+).*" "\\1" LZMA_VERSION_MAJOR "${LZMA_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*LZMA_VERSION_MINOR ([0-9]+).*" "\\1" LZMA_VERSION_MINOR "${LZMA_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*LZMA_VERSION_PATCH ([0-9]+).*" "\\1" LZMA_VERSION_PATCH "${LZMA_VERSION_CONTENTS}")
    set(LZMA_VERSION_STRING "${LZMA_VERSION_MAJOR}.${LZMA_VERSION_MINOR}.${LZMA_VERSION_PATCH}")
    set(LZMA_VERSION ${LZMA_VERSION_STRING})

    MatchVersion(LZMA)
endif()

# COMPILATION
# -----------

set(LZMA_CODE "
#include <lzma.h>
int main(int argc, char *argv[])
{
    const char *version;
    version = lzma_version_string();
    return 0;
}
"
)

if(LZMA_FOUND)
    CheckCompiles(LZMA)
endif()
RequiredPackageFound(LZMA)