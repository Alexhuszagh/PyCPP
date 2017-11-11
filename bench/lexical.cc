//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <benchmark/benchmark.h>
#include <pycpp/lexical/atoi.h>
#include <pycpp/lexical/itoa.h>
#include <string>
#include <vector>

PYCPP_USING_NAMESPACE

// BENCHMARKS
// ----------

std::vector<std::string> STRINGS = {
    "0",
    "1",
    "12",
    "123",
    "1234",
    "12345",
    "123456",
    "1234567",
    "12345678",
    "123456789",
    "1234567890",
    "12345678901",
    "123456789012",
    "1234567890123",
    "12345678901234",
    "123456789012345",
    "1234567890123456",
    "12345678901234567",
    "123456789012345678",
    "1234567890123456789",
    "12345678901234567890",
};


std::vector<uint64_t> NUMBERS = {
    0ULL,
    1ULL,
    12ULL,
    123ULL,
    1234ULL,
    12345ULL,
    123456ULL,
    1234567ULL,
    12345678ULL,
    123456789ULL,
    1234567890ULL,
    12345678901ULL,
    123456789012ULL,
    1234567890123ULL,
    12345678901234ULL,
    123456789012345ULL,
    1234567890123456ULL,
    12345678901234567ULL,
    123456789012345678ULL,
    1234567890123456789ULL,
    12345678901234567890ULL,
};


static void std_strtoll(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: STRINGS) {
            benchmark::DoNotOptimize(std::strtoll(s.data(), nullptr, 10));
        }
    }
}


static void atoi64(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: STRINGS) {
            benchmark::DoNotOptimize(atoi64(s, 10));
        }
    }
}


static void std_to_string(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: NUMBERS) {
            benchmark::DoNotOptimize(std::to_string(n));
        }
    }
}


static void i64toa(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: NUMBERS) {
            benchmark::DoNotOptimize(u64toa(n, 10));
        }
    }
}

// TODO: need benchmarks for other integer-to-string versions

// REGISTER
// --------

BENCHMARK(std_strtoll);
BENCHMARK(atoi64);
BENCHMARK(std_to_string);
BENCHMARK(i64toa);
BENCHMARK_MAIN();