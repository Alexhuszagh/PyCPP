//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/gettimeofday.h>

PYCPP_BEGIN_NAMESPACE

#ifdef _MSC_VER
#   include <cstdint>
#   include <windows.h>

// CONSTANTS
// ---------

#define EPOCH_TIME 116444736000000000ull
#define HECTONANOSEC_PER_SEC 10000000ull

// UNIONS
// ------

union TimeUnion
{
    uint64_t ns100;
    FILETIME file_time;
};

// FUNCTIONS
// ---------


int gettimeofday(struct timeval* tp, void* tz)
{
    int res = 0;
    TimeUnion now;
    SYSTEMTIME system_time;
    uint64_t value;

    if (tp != NULL) {
        GetSystemTimeAsFileTime(&now.file_time);
        now.ns100 -= EPOCH_TIME;
        tp->tv_sec = now.ns100 / HECTONANOSEC_PER_SEC;
        tp->tv_usec = (long) (now.ns100 % HECTONANOSEC_PER_SEC) / 10;
    }

    return res;
}

#endif

PYCPP_END_NAMESPACE
