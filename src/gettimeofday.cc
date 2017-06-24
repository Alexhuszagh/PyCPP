//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "gettimeofday.h"

#ifdef _MSC_VER

// CONSTANTS
// ---------

#define EPOCH_TIME 116444736000000000ull
#define HECTONANOSEC_PER_SEC 10000000ull

// FUNCTIONS
// ---------


int gettimeofday(struct timeval* tp, void* tz)
{
    int res = 0;
    union {
        uint64_t ns100;
        FILETIME file_time;
    } now;
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
