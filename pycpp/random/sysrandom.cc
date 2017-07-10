//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  The following section uses cryptographic random number generators.
 *  Although std::random_device was designed in the C++ standard library
 *  for this reason, if hardware sources are insufficient, it allows
 *  a possibly deterministic fallback, which is true for compilers
 *  such a MinGW. For these reasons, native APIs and /dev/urandom
 *  are used for truly random bytes.
 */

#include <pycpp/os.h>
#include <pycpp/random.h>
#if defined(OS_WINDOWS)
#   include <windows.h>
#   include <wincrypt.h>
#   include <stdexcept>
#else
#   include <fstream>
#endif

#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

#if defined(OS_WINDOWS)         // WINDOWS


 /**
  * Wrapper that acquires the context for the crypt library. If the
  * keyset has not been set, it also initializes that.
  */
bool acquire_context(HCRYPTPROV *ctx)
{
    if (!CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, 0)) {
        return CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, CRYPT_NEWKEYSET);
    }
    return true;
}


size_t sysrandom(void* dst, size_t dstlen)
{
    HCRYPTPROV ctx;
    if (!acquire_context(&ctx)) {
        throw std::runtime_error("Unable to initialize Win32 crypt library.");
    }

    BYTE* buffer = reinterpret_cast<BYTE*>(dst);
    if(!CryptGenRandom(ctx, dstlen, buffer)) {
        throw std::runtime_error("Unable to generate random bytes.");
    }

    if (!CryptReleaseContext(ctx, 0)) {
        throw std::runtime_error("Unable to release Win32 crypt library.");
    }

    return dstlen;
}

#else                           // POSIX


/**
 *  Generate cryptograhically random bytes on POSIX-systems. Open and
 *  read from `/dev/urandom`, **not** `/dev/random`.
 *
 *  [reference] http://www.2uo.de/myths-about-urandom/
 */
size_t sysrandom(void* dst, size_t dstlen)
{
    char* buffer = reinterpret_cast<char*>(dst);
    std::ifstream stream("/dev/urandom", std::ios_base::binary | std::ios_base::in);
    stream.read(buffer, dstlen);

    return dstlen;
}

#endif


std::string sysrandom(size_t length)
{
    char* dst = new char[length];
    sysrandom(dst, length);
    std::string output(dst, length);
    delete[] dst;
    return output;
}

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
