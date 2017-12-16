//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <benchmark/benchmark.h>
#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/atoi.h>
#include <pycpp/lexical/ftoa.h>
#include <pycpp/lexical/itoa.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/vector.h>

PYCPP_USING_NAMESPACE

// BENCHMARKS
// ----------

vector<std::string> FLOAT_STRINGS = {
    "0",
    "1",
    "1.2",
    "1.23",
    "1.234",
    "1.2345",
    "1.23456",
    "1.234567",
    "1.2345678",
    "1.23456789",
    "1.234567890",
    "1.2345678901",
    "1.23456789012",
    "1.234567890123",
    "1.2345678901234",
    "1.23456789012345",
    "1.234567890123456",
    "1.2345678901234567",
    "1.23456789012345678",
    "1.234567890123456789",
};

vector<std::string> INTEGER_STRINGS = {
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


vector<uint64_t> INTEGERS = {
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


vector<double> FLOATS = {
    0.0,
    1.0,
    1.2,
    1.23,
    1.234,
    1.2345,
    1.23456,
    1.234567,
    1.2345678,
    1.23456789,
    1.234567890,
    1.2345678901,
    1.23456789012,
    1.234567890123,
    1.2345678901234,
    1.23456789012345,
    1.234567890123456,
    1.2345678901234567,
    1.23456789012345678,
    1.234567890123456789,
};


static void std_strtoll(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: INTEGER_STRINGS) {
            benchmark::DoNotOptimize(std::strtoll(s.data(), nullptr, 10));
        }
    }
}


static void atoi64(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: INTEGER_STRINGS) {
            benchmark::DoNotOptimize(atoi64(s, 10));
        }
    }
}


static void std_to_string(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: INTEGERS) {
            benchmark::DoNotOptimize(std::to_string(n));
        }
    }
}


static void i64toa(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: INTEGERS) {
            benchmark::DoNotOptimize(u64toa(n, 10));
        }
    }
}


static void i64toa_base2(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: INTEGERS) {
            benchmark::DoNotOptimize(u64toa(n, 2));
        }
    }
}


static void i64toa_base16(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto n: INTEGERS) {
            benchmark::DoNotOptimize(u64toa(n, 16));
        }
    }
}


static void std_strtod(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: FLOAT_STRINGS) {
            benchmark::DoNotOptimize(std::strtod(s.data(), nullptr));
        }
    }
}


static void atof64(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto s: FLOAT_STRINGS) {
            benchmark::DoNotOptimize(atof64(s, 10));
        }
    }
}

// REGISTER
// --------

BENCHMARK(std_strtoll);
BENCHMARK(atoi64);
BENCHMARK(std_to_string);
BENCHMARK(i64toa);
BENCHMARK(i64toa_base2);
BENCHMARK(i64toa_base16);
BENCHMARK(std_strtod);
BENCHMARK(atof64);
// TODO: std_to_string_double
// TODO: ftoa
BENCHMARK_MAIN();