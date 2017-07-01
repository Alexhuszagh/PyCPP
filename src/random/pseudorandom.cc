//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <random.h>
#include <chrono>
#include <cstring>
#include <ctime>
#include <random>

// ALIAS
// -----

typedef std::chrono::high_resolution_clock highres_clock_t;

// FUNCTIONS
// ---------


size_t pseudorandom(void* dst, size_t dstlen, bool deterministic)
{
    std::mt19937 generator;
    if (deterministic) {
        generator.seed(time(nullptr));
    } else {
        auto now = highres_clock_t::now();
        auto seed = now.time_since_epoch().count();
        generator.seed(seed);
    }

    char* buffer = reinterpret_cast<char*>(dst);
    size_t counter = dstlen / sizeof(uint32_t);
    while (counter--) {
        uint32_t value = generator();
        memmove(buffer, &value, sizeof(value));
        buffer += sizeof(value);
    }

    return dstlen;
}


std::string pseudorandom(size_t length, bool deterministic)
{
    char* dst = new char[length];
    pseudorandom(dst, length, deterministic);
    std::string output(dst, length);
    delete[] dst;
    return output;
}
