#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindBZIP2
# ---------
#
# Find BZIP2 include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(BZIP2
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if BZIP2 is not found
#     )
#
# You may also set `BZIP2_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `BZIP2_FOUND` will be set to true, and `BZIP2_LIBRARIES`
# and `BZIP2_INCLUDE_DIRS` will both be set.
#
# You may optionally set `BZIP2_ROOT` to specify a custom root directory
# for the BZIP2 installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(BZIP2_SEARCH_PATHS)

if(BZIP2_ROOT)
    list(APPEND BZIP2_SEARCH_PATHS ${BZIP2_ROOT})
endif()

if(WIN32)
    list(APPEND BZIP2_SEARCH_PATHS
#        "$ENV{PROGRAMFILES}/bzip2"
    )
endif()

unset(BZIP2_SYSTEM_ROOT)
unset(BZIP2_CUSTOM_ROOT)
unset(BZIP2_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(BZIP2)
foreach(search ${BZIP2_SEARCH_PATHS})
    FIND_PATH(BZIP2_INCLUDE_DIR
        NAMES bzlib.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT BZIP2_INCLUDE_DIR)
    FIND_PATH(BZIP2_INCLUDE_DIR bzlib.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(BZIP2_LIBRARY_NAMES bz2 bzip2 bz2-1.0)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND BZIP2_LIBRARY_NAMES bz2d bzip2d)
endif()

foreach(search ${BZIP2_SEARCH_PATHS})
    FIND_LIBRARY(BZIP2_LIBRARY
        NAMES ${BZIP2_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT BZIP2_LIBRARY)
    FIND_LIBRARY(BZIP2_LIBRARY NAMES ${BZIP2_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(BZIP2_INCLUDE_DIRS ${BZIP2_INCLUDE_DIR})
set(BZIP2_LIBRARIES ${BZIP2_LIBRARY})

CheckFound(BZIP2)
FindStaticLibs(BZIP2)

# VERSION
# -------

if(BZIP2_FOUND)
    file(STRINGS "${BZIP2_INCLUDE_DIRS}/zlib.h" BZIP2_VERSION_CONTENTS REGEX "Version [0-9]+\\.[0-9]+(\\.[0-9]+)?")
    string(REGEX REPLACE ".*bzip2/libbzip2 version ([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" BZIP2_VERSION_MAJOR "${BZIP2_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*bzip2/libbzip2 version [0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" BZIP2_VERSION_MINOR "${BZIP2_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*bzip2/libbzip2 version [0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" BZIP2_VERSION_PATCH "${BZIP2_VERSION_CONTENTS}")
    set(BZIP2_VERSION_STRING "${BZIP2_VERSION_MAJOR}.${BZIP2_VERSION_MINOR}.${BZIP2_VERSION_PATCH}")
    set(BZIP2_VERSION ${BZIP2_VERSION_STRING})

    MatchVersion(BZIP2)
endif()

# COMPILATION
# -----------

set(BZIP2_CODE "
#include <bzlib.h>
int main(void)
{
    const char *version = BZ2_bzlibVersion();
    return 0;
}
"
)

if(BZIP2_FOUND)
    CheckCompiles(BZIP2)
endif()
RequiredPackageFound(BZIP2)