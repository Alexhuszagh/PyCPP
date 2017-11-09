#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindBLOSC
# ---------
#
# Find BLOSC include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(BLOSC
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if BLOSC is not found
#     )
#
# You may also set `BLOSC_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `BLOSC_FOUND` will be set to true, and `BLOSC_LIBRARIES`
# and `BLOSC_INCLUDE_DIRS` will both be set.
#
# You may optionally set `BLOSC_ROOT` to specify a custom root directory
# for the BLOSC installation.
#

# PATHS
# -----

set(BLOSC_SEARCH_PATHS)

if(BLOSC_ROOT)
    list(APPEND BLOSC_SEARCH_PATHS ${BLOSC_ROOT})
endif()

if(WIN32)
    list(APPEND BLOSC_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/xz"
        "$ENV{PROGRAMFILES}/lzma"
    )
endif()

unset(BLOSC_SYSTEM_ROOT)
unset(BLOSC_CUSTOM_ROOT)
unset(BLOSC_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(BLOSC)
foreach(search ${BLOSC_SEARCH_PATHS})
    FIND_PATH(BLOSC_INCLUDE_DIR
        NAMES blosc.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT BLOSC_INCLUDE_DIR)
    FIND_PATH(BLOSC_INCLUDE_DIR blosc.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(BLOSC_LIBRARY_NAMES blosc)

foreach(search ${BLOSC_SEARCH_PATHS})
    FIND_LIBRARY(BLOSC_LIBRARY
        NAMES ${BLOSC_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT BLOSC_LIBRARY)
    FIND_LIBRARY(BLOSC_LIBRARY NAMES ${BLOSC_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(BLOSC_INCLUDE_DIRS ${BLOSC_INCLUDE_DIR})
set(BLOSC_LIBRARIES ${BLOSC_LIBRARY})

CheckFound(BLOSC)
FindStaticLibs(BLOSC)

# VERSION
# -------

if(BLOSC_FOUND)
    file(STRINGS "${BLOSC_INCLUDE_DIRS}/blosc.h" BLOSC_VERSION_CONTENTS REGEX "#define BLOSC_VERSION_[A-Z]+")
    string(REGEX REPLACE ".*BLOSC_VERSION_MAJOR +([0-9]+).*" "\\1" BLOSC_VERSION_MAJOR "${BLOSC_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*BLOSC_VERSION_MINOR +([0-9]+).*" "\\1" BLOSC_VERSION_MINOR "${BLOSC_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*BLOSC_VERSION_PATCH +([0-9]+).*" "\\1" BLOSC_VERSION_PATCH "${BLOSC_VERSION_CONTENTS}")
    set(BLOSC_VERSION_STRING "${BLOSC_VERSION_MAJOR}.${BLOSC_VERSION_MINOR}.${BLOSC_VERSION_PATCH}")
    set(BLOSC_VERSION ${BLOSC_VERSION_STRING})

    MatchVersion(BLOSC)
endif()

# COMPILATION
# -----------

set(BLOSC_CODE "
#include <blosc.h>
int main(int argc, char *argv[])
{
    const char *version = BLOSC_VERSION_STRING;
"
)

if(BLOSC_FOUND)
    CheckCompiles(BLOSC)
endif()
RequiredPackageFound(BLOSC)