#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindLIBXML2
# -----------
#
# Find LIBXML2 include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(LIBXML2
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if LIBXML2 is not found
#     )
#
# You may also set `LIBXML2_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `LIBXML2_FOUND` will be set to true, and `LIBXML2_LIBRARIES`
# and `LIBXML2_INCLUDE_DIRS` will both be set.
#
# You may optionally set `LIBXML2_ROOT` to specify a custom root directory
# for the LIBXML2 installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(LIBXML2_SEARCH_PATHS)

if(LIBXML2_ROOT)
    list(APPEND LIBXML2_SEARCH_PATHS ${LIBXML2_ROOT})
endif()

if(WIN32)
    list(APPEND LIBXML2_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/libxml2"
    )
endif()

unset(LIBXML2_SYSTEM_ROOT)
unset(LIBXML2_CUSTOM_ROOT)
unset(LIBXML2_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(LIBXML2)
foreach(search ${LIBXML2_SEARCH_PATHS})
    FIND_PATH(LIBXML2_INCLUDE_DIR
        NAMES libxml/xpath.h
        PATHS ${search}
        PATH_SUFFIXES include libxml2
    )
endforeach(search)

if(NOT LIBXML2_INCLUDE_DIR)
    FIND_PATH(LIBXML2_INCLUDE_DIR libxml/xpath.h PATH_SUFFIXES include libxml2)
endif()

# LIBRARY PATHS
set(LIBXML2_LIBRARY_NAMES xml2 libxml2)

foreach(search ${LIBXML2_SEARCH_PATHS})
    FIND_LIBRARY(LIBXML2_LIBRARY
        NAMES ${LIBXML2_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT LIBXML2_LIBRARY)
    FIND_LIBRARY(LIBXML2_LIBRARY NAMES ${LIBXML2_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR})
set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY})

CheckFound(LIBXML2)
FindStaticLibs(LIBXML2)

# VERSION
# -------

if(LIBXML2_FOUND)
    file(STRINGS "${LIBXML2_INCLUDE_DIRS}/libxml/xmlversion.h" LIBXML2_VERSION_CONTENTS REGEX "#define LIBXML_DOTTED_VERSION")
    string(REGEX REPLACE ".*#define LIBXML_DOTTED_VERSION \"([0-9]+)\.[0-9]+.[0-9]+\".*" "\\1" LIBXML2_VERSION_MAJOR "${LIBXML2_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*#define LIBXML_DOTTED_VERSION \"[0-9]+\.([0-9]+).[0-9]+\".*" "\\1" LIBXML2_VERSION_MINOR "${LIBXML2_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*#define LIBXML_DOTTED_VERSION \"[0-9]+\.[0-9]+.([0-9]+)\".*" "\\1" LIBXML2_VERSION_PATCH "${LIBXML2_VERSION_CONTENTS}")
    set(LIBXML2_VERSION_STRING "${LIBXML2_VERSION_MAJOR}.${LIBXML2_VERSION_MINOR}.${LIBXML2_VERSION_PATCH}")
    set(LIBXML2_VERSION ${LIBXML2_VERSION_STRING})

    MatchVersion(LIBXML2)
endif()

# COMPILATION
# -----------

set(LIBXML2_CODE "
#include <libxml/xmlversion.h>
int main(int argc, char *argv[])
{
    const char *version = LIBXML_DOTTED_VERSION;
    return 0;
}
"
)

if(LIBXML2_FOUND)
    CheckCompiles(LIBXML2)
endif()
RequiredPackageFound(LIBXML2)