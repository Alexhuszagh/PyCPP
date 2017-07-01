//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Detect processor for build.
 *
 *  Most of these macros can be found from:
 *      https://sourceforge.net/p/predef/wiki/Architectures/
 *      https://people.csail.mit.edu/jaffer/scm/Automatic-C-Preprocessor-Definitions.html
 */

#pragma once

// MACROS
// ------

// ITANIUM
#if !defined (PROCESSOR_DETECTED)
#   if defined(__ia64) || defined(__itanium__) || defined(_M_IA64)
#      define HAVE_ITANIUM
#      define HAVE_ITANIUM64
#      define PROCESSOR_DETECTED
#   endif
#endif

// POWERPC
#if !defined (PROCESSOR_DETECTED)
#   if defined(ppc) || defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
#      define HAVE_POWERPC
#      define PROCESSOR_DETECTED
#       if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(__64BIT__) || defined(_LP64) || defined(__LP64__)
#           define HAVE_POWERPC64
#       else
#           define HAVE_POWERPC32
#       endif
#   endif
#endif

// SPARC
#if !defined (PROCESSOR_DETECTED)
#   if defined(__sparc)
#       define HAVE_SPARC
#       define PROCESSOR_DETECTED
#       if defined(__sparc_v9__) || defined(__sparcv9) || defined(__arch64__)
#          define HAVE_SPARC64
#       else
#          define HAVE_SPARC32
#       endif
#   endif
#endif


// X86
#if !defined (PROCESSOR_DETECTED)
#   if defined(__x86_64__) || defined(_M_X64)
#       define HAVE_X86
#       define HAVE_X8664
#       define PROCESSOR_DETECTED
#   elif defined(__i386) || defined(_M_IX86)
#       define HAVE_X86
#       define HAVE_X8632
#       define PROCESSOR_DETECTED
#   endif
#endif

// DEC ALPHA
#if !defined (PROCESSOR_DETECTED)
#   if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#       define HAVE_ALPHA
#       define HAVE_ALPHA64
#       define PROCESSOR_DETECTED
#   endif
#endif

// ARM
#if !defined (PROCESSOR_DETECTED)
#   if defined(__arm__) || defined(__thumb__) || defined(__arm) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__aarch64__)
#       define HAVE_ARM
#       if defined(__aarch64__)
#          define HAVE_ARM64
#       else
#          define HAVE_ARM32
#       endif
#       define PROCESSOR_DETECTED
#   endif
#endif

// BLACKFIN
#if !defined (PROCESSOR_DETECTED)
#   if defined(__bfin) || defined(__BFIN__)
#       define HAVE_BLACKFIN
#       define HAVE_BLACKFIN32
#       define PROCESSOR_DETECTED
#   endif
#endif

// CONVEX
#if !defined (PROCESSOR_DETECTED)
#   if defined(__bfin) || defined(__BFIN__)
#       define HAVE_CONVEX
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define HAVE_CONVEX32
#       elif SYSTEM_ARCHITECTURE == 64
#          define HAVE_CONVEX64
#       endif
#   endif
#endif

// EPIPHANY
#if !defined (PROCESSOR_DETECTED)
#   if defined(__epiphany__)
#       define HAVE_EPIPHANY
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define HAVE_EPIPHANY32
#       elif SYSTEM_ARCHITECTURE == 64
#          define HAVE_EPIPHANY64
#       endif
#   endif
#endif

// HPPA RISC
#if !defined (PROCESSOR_DETECTED)
#   if defined(__hppa__) || defined(__HPPA__) || defined(__hppa)
#       define HAVE_HPPA
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define HAVE_HPPA32
#       elif SYSTEM_ARCHITECTURE == 64
#          define HAVE_HPPA64
#       endif
#   endif
#endif

// MOTOROLA 68K
#if !defined (PROCESSOR_DETECTED)
#   if defined(__m68k__) || defined(M68000) || defined(__MC68K__)
#       define HAVE_68K
#       define HAVE_68K32
#       define PROCESSOR_DETECTED
#   endif
#endif

// MIPS
#if !defined (PROCESSOR_DETECTED)
#   if defined(__mips__) || defined(mips) || defined(__mips) || defined(__MIPS__)
#       define HAVE_MIPS
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define HAVE_MIPS32
#       elif SYSTEM_ARCHITECTURE == 64
#          define HAVE_MIPS64
#       endif
#   endif
#endif

// SUPERH
#if !defined (PROCESSOR_DETECTED)
#   if defined(__sh__)
#       define HAVE_SUPERH
#       define PROCESSOR_DETECTED
#       if defined(__SH5__)
#           define HAVE_SUPERH64
#       else
#           define HAVE_SUPERH32
#       endif
#   endif
#endif

// SYSTEMZ
#if !defined (PROCESSOR_DETECTED)
#   if defined(__370__) || defined(__THW_370__) || defined(__s390__) || defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#       define HAVE_SYSTEMZ
#       define PROCESSOR_DETECTED
#       if defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#           define HAVE_SYSTEMZ64
#       else
#           define HAVE_SYSTEMZ32
#       endif
#   endif
#endif

// TAHOE
#if !defined(PROCESSOR_DETECTED)
#   if defined(tahoe)
#       define HAVE_TAHOE
#       define HAVE_TAHOE32
#       define PROCESSOR_DETECTED
#   endif
#endif

// VAX
#if !defined(PROCESSOR_DETECTED)
#   if defined(__vax__) || defined(vax) || defined(vaxc) || defined(VAXC) || defined(vax11c) || defined(VAX11)
#       define HAVE_VAX
#       define HAVE_VAX32
#       define PROCESSOR_DETECTED
#   endif
#endif
