//  :copyright: (c) 2011 V8 Project Authors
//  :copyright: (c) 2013 Andreas Samoljuk
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  The optimized routines are from Andrea Samoljuk's `fpconv` library,
 *  which is available here:
 *      `https://github.com/night-shift/fpconv`
 */

#include <pycpp/lexical/itoa.h>
#include <pycpp/lexical/float.h>
#include <pycpp/lexical/format.h>
#include <pycpp/lexical/ftoa.h>
#include <pycpp/lexical/table.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/string_view.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define npowers     87
#define steppowers  8
#define firstpower -348 /* 10 ^ -348 */
#define expmax     -32
#define expmin     -60
#define fracmask  0x000FFFFFFFFFFFFFU
#define expmask   0x7FF0000000000000U
#define hiddenbit 0x0010000000000000U
#define signmask  0x8000000000000000U
#define expbias   (1023 + 52)
#define absv(n) ((n) < 0 ? -(n) : (n))
#define minv(a, b) ((a) < (b) ? (a) : (b))

// OBJECTS
// -------

struct fp_t
{
    uint64_t frac;
    int exp;
};

union dbits_t
{
    double d;
    uint64_t i;
};

// CONSTANTS
// ---------

// IEEE754 CONSTANTS
static constexpr uint64_t EXPONENT_MASK = 0x7FF0000000000000ULL;
static constexpr uint64_t HIDDEN_BIT = 0x0010000000000000ULL;
static constexpr uint64_t SIGN_MASK = 0x800000000000000ULL;
static constexpr uint64_t SIGNIFICAND_MASK = 0x000FFFFFFFFFFFFFULL;
static constexpr uint64_t U64_INFINITY = 0x7FF0000000000000ULL;
static constexpr int PHYSICAL_SIGNIFICAND_SIZE = 52;
static constexpr int SIGNIFICAND_SIZE = 53;
static constexpr int EXPONENT_BIAS = 0x3FF + PHYSICAL_SIGNIFICAND_SIZE;
static constexpr int DENORMAL_EXPONENT = -EXPONENT_BIAS + 1;

// BUFFER PARAMETERS
static constexpr size_t BUFFER_SIZE = MAX_FLOAT_SIZE;

// LOOKUPS
static constexpr uint64_t TENS[] = {
    10000000000000000000U, 1000000000000000000U, 100000000000000000U,
    10000000000000000U, 1000000000000000U, 100000000000000U,
    10000000000000U, 1000000000000U, 100000000000U,
    10000000000U, 1000000000U, 100000000U,
    10000000U, 1000000U, 100000U,
    10000U, 1000U, 100U,
    10U, 1U
};

static constexpr fp_t POWERS_TEN[] = {
    { 18054884314459144840U, -1220 }, { 13451937075301367670U, -1193 },
    { 10022474136428063862U, -1166 }, { 14934650266808366570U, -1140 },
    { 11127181549972568877U, -1113 }, { 16580792590934885855U, -1087 },
    { 12353653155963782858U, -1060 }, { 18408377700990114895U, -1034 },
    { 13715310171984221708U, -1007 }, { 10218702384817765436U, -980 },
    { 15227053142812498563U, -954 },  { 11345038669416679861U, -927 },
    { 16905424996341287883U, -901 },  { 12595523146049147757U, -874 },
    { 9384396036005875287U,  -847 },  { 13983839803942852151U, -821 },
    { 10418772551374772303U, -794 },  { 15525180923007089351U, -768 },
    { 11567161174868858868U, -741 },  { 17236413322193710309U, -715 },
    { 12842128665889583758U, -688 },  { 9568131466127621947U,  -661 },
    { 14257626930069360058U, -635 },  { 10622759856335341974U, -608 },
    { 15829145694278690180U, -582 },  { 11793632577567316726U, -555 },
    { 17573882009934360870U, -529 },  { 13093562431584567480U, -502 },
    { 9755464219737475723U,  -475 },  { 14536774485912137811U, -449 },
    { 10830740992659433045U, -422 },  { 16139061738043178685U, -396 },
    { 12024538023802026127U, -369 },  { 17917957937422433684U, -343 },
    { 13349918974505688015U, -316 },  { 9946464728195732843U,  -289 },
    { 14821387422376473014U, -263 },  { 11042794154864902060U, -236 },
    { 16455045573212060422U, -210 },  { 12259964326927110867U, -183 },
    { 18268770466636286478U, -157 },  { 13611294676837538539U, -130 },
    { 10141204801825835212U, -103 },  { 15111572745182864684U, -77 },
    { 11258999068426240000U, -50 },   { 16777216000000000000U, -24 },
    { 12500000000000000000U,   3 },   { 9313225746154785156U,   30 },
    { 13877787807814456755U,  56 },   { 10339757656912845936U,  83 },
    { 15407439555097886824U, 109 },   { 11479437019748901445U, 136 },
    { 17105694144590052135U, 162 },   { 12744735289059618216U, 189 },
    { 9495567745759798747U,  216 },   { 14149498560666738074U, 242 },
    { 10542197943230523224U, 269 },   { 15709099088952724970U, 295 },
    { 11704190886730495818U, 322 },   { 17440603504673385349U, 348 },
    { 12994262207056124023U, 375 },   { 9681479787123295682U,  402 },
    { 14426529090290212157U, 428 },   { 10748601772107342003U, 455 },
    { 16016664761464807395U, 481 },   { 11933345169920330789U, 508 },
    { 17782069995880619868U, 534 },   { 13248674568444952270U, 561 },
    { 9871031767461413346U,  588 },   { 14708983551653345445U, 614 },
    { 10959046745042015199U, 641 },   { 16330252207878254650U, 667 },
    { 12166986024289022870U, 694 },   { 18130221999122236476U, 720 },
    { 13508068024458167312U, 747 },   { 10064294952495520794U, 774 },
    { 14996968138956309548U, 800 },   { 11173611982879273257U, 827 },
    { 16649979327439178909U, 853 },   { 12405201291620119593U, 880 },
    { 9242595204427927429U,  907 },   { 13772540099066387757U, 933 },
    { 10261342003245940623U, 960 },   { 15290591125556738113U, 986 },
    { 11392378155556871081U, 1013 },  { 16975966327722178521U, 1039 },
    { 12648080533535911531U, 1066 }
};

// HELPERS
// -------

// EXPONENT

char e_notation_char(uint8_t base)
{
    // After radix of base15 and higher, 'E' and 'e' are
    // part of the controlled vocabulary.
    // We use a non-standard extension of '^' to signify
    // the exponent in base15 and above.
    return base >= 15 ? '^' : 'e';
}

// FPCONV GRISU

static fp_t find_cachedpow10(int exp, int* k)
{
    const double one_log_ten = 0.30102999566398114;

    int approx = -(exp + npowers) * one_log_ten;
    int idx = (approx - firstpower) / steppowers;

    while(true) {
        int current = exp + POWERS_TEN[idx].exp + 64;

        if(current < expmin) {
            idx++;
            continue;
        }

        if(current > expmax) {
            idx--;
            continue;
        }

        *k = (firstpower + idx * steppowers);

        return POWERS_TEN[idx];
    }
}


static inline uint64_t get_dbits(double d)
{
    dbits_t dbl_bits = { d };
    return dbl_bits.i;
}


static inline double get_u64bits(uint64_t u64)
{
    dbits_t dbl_bits;
    dbl_bits.i = u64;
    return dbl_bits.d;
}


static fp_t build_fp(double d)
{
    uint64_t bits = get_dbits(d);

    fp_t fp;
    fp.frac = bits & fracmask;
    fp.exp = (bits & expmask) >> 52;

    if (fp.exp) {
        fp.frac += hiddenbit;
        fp.exp -= expbias;

    } else {
        fp.exp = -expbias + 1;
    }

    return fp;
}

static void normalize(fp_t* fp)
{
    while ((fp->frac & hiddenbit) == 0) {
        fp->frac <<= 1;
        fp->exp--;
    }

    int shift = 64 - 52 - 1;
    fp->frac <<= shift;
    fp->exp -= shift;
}


static void get_normalized_boundaries(fp_t* fp, fp_t* lower, fp_t* upper)
{
    upper->frac = (fp->frac << 1) + 1;
    upper->exp  = fp->exp - 1;

    while ((upper->frac & (hiddenbit << 1)) == 0) {
        upper->frac <<= 1;
        upper->exp--;
    }

    int u_shift = 64 - 52 - 2;

    upper->frac <<= u_shift;
    upper->exp = upper->exp - u_shift;


    int l_shift = fp->frac == hiddenbit ? 2 : 1;

    lower->frac = (fp->frac << l_shift) - 1;
    lower->exp = fp->exp - l_shift;


    lower->frac <<= lower->exp - upper->exp;
    lower->exp = upper->exp;
}


static fp_t multiply(fp_t* a, fp_t* b)
{
    const uint64_t lomask = 0x00000000FFFFFFFF;

    uint64_t ah_bl = (a->frac >> 32)    * (b->frac & lomask);
    uint64_t al_bh = (a->frac & lomask) * (b->frac >> 32);
    uint64_t al_bl = (a->frac & lomask) * (b->frac & lomask);
    uint64_t ah_bh = (a->frac >> 32)    * (b->frac >> 32);

    uint64_t tmp = (ah_bl & lomask) + (al_bh & lomask) + (al_bl >> 32);
    /* round up */
    tmp += 1U << 31;

    fp_t fp = {
        ah_bh + (ah_bl >> 32) + (al_bh >> 32) + (tmp >> 32),
        a->exp + b->exp + 64
    };

    return fp;
}


static void round_digit(char* digits, int ndigits, uint64_t delta, uint64_t rem, uint64_t kappa, uint64_t frac)
{
    while (rem < frac && delta - rem >= kappa &&
           (rem + kappa < frac || frac - rem > rem + kappa - frac)) {

        digits[ndigits - 1]--;
        rem += kappa;
    }
}


static int generate_digits(fp_t* fp, fp_t* upper, fp_t* lower, char* digits, int* K)
{
    uint64_t wfrac = upper->frac - fp->frac;
    uint64_t delta = upper->frac - lower->frac;

    fp_t one;
    one.frac = 1ULL << -upper->exp;
    one.exp  = upper->exp;

    uint64_t part1 = upper->frac >> -one.exp;
    uint64_t part2 = upper->frac & (one.frac - 1);

    int idx = 0, kappa = 10;
    const uint64_t* divp;
    /* 1000000000 */
    for(divp = TENS + 10; kappa > 0; divp++) {

        uint64_t div = *divp;
        unsigned digit = part1 / div;

        if (digit || idx) {
            digits[idx++] = digit + '0';
        }

        part1 -= digit * div;
        kappa--;

        uint64_t tmp = (part1 <<-one.exp) + part2;
        if (tmp <= delta) {
            *K += kappa;
            round_digit(digits, idx, delta, tmp, div << -one.exp, wfrac);

            return idx;
        }
    }

    /* 10 */
    const uint64_t* unit = TENS + 18;

    while(true) {
        part2 *= 10;
        delta *= 10;
        kappa--;

        unsigned digit = part2 >> -one.exp;
        if (digit || idx) {
            digits[idx++] = digit + '0';
        }

        part2 &= one.frac - 1;
        if (part2 < delta) {
            *K += kappa;
            round_digit(digits, idx, delta, part2, one.frac, wfrac * *unit);

            return idx;
        }

        unit--;
    }
}

static int grisu2(double d, char* digits, int* K)
{
    fp_t w = build_fp(d);

    fp_t lower, upper;
    get_normalized_boundaries(&w, &lower, &upper);

    normalize(&w);

    int k;
    fp_t cp = find_cachedpow10(upper.exp, &k);

    w     = multiply(&w,     &cp);
    upper = multiply(&upper, &cp);
    lower = multiply(&lower, &cp);

    lower.frac++;
    upper.frac--;

    *K = -k;

    return generate_digits(&w, &upper, &lower, digits, K);
}


static int emit_digits(char* digits, int ndigits, char* dest, int K)
{
    int exp = absv(K + ndigits - 1);

    /* write plain integer */
    if(K >= 0 && (exp < (ndigits + 7))) {
        memcpy(dest, digits, ndigits);
        memset(dest + ndigits, '0', K);

        return ndigits + K;
    }

    /* write decimal w/o scientific notation */
    if(K < 0 && (K > -7 || exp < 4)) {
        int offset = ndigits - absv(K);
        /* fp < 1.0 -> write leading zero */
        if(offset <= 0) {
            offset = -offset;
            dest[0] = '0';
            dest[1] = '.';
            memset(dest + 2, '0', offset);
            memcpy(dest + offset + 2, digits, ndigits);

            return ndigits + 2 + offset;

        /* fp > 1.0 */
        } else {
            memcpy(dest, digits, offset);
            dest[offset] = '.';
            memcpy(dest + offset + 1, digits + offset, ndigits - offset);

            return ndigits + 1;
        }
    }

    /* write decimal w/ scientific notation */
    ndigits = minv(ndigits, 18);

    int idx = 0;
    dest[idx++] = digits[0];

    if(ndigits > 1) {
        dest[idx++] = '.';
        memcpy(dest + idx, digits + 1, ndigits - 1);
        idx += ndigits - 1;
    }

    dest[idx++] = e_notation_char(10);

    char sign = K + ndigits - 1 < 0 ? '-' : '+';
    dest[idx++] = sign;

    int cent = 0;

    if(exp > 99) {
        cent = exp / 100;
        dest[idx++] = cent + '0';
        exp -= cent * 100;
    }
    if(exp > 9) {
        int dec = exp / 10;
        dest[idx++] = dec + '0';
        exp -= dec * 10;

    } else if(cent) {
        dest[idx++] = '0';
    }

    dest[idx++] = exp % 10 + '0';

    return idx;
}

static int filter_special(double fp, char* dest)
{
    if(fp == 0.0) {
        dest[0] = '0';
        return 1;
    }

    uint64_t bits = get_dbits(fp);

    bool nan = (bits & expmask) == expmask;

    if(!nan) {
        return 0;
    }

    if(bits & fracmask) {
        memcpy(dest, NAN_STRING.data(), NAN_STRING.size());
        return static_cast<int>(NAN_STRING.size());

    } else {
        memcpy(dest, INFINITY_STRING.data(), INFINITY_STRING.size());
        return static_cast<int>(INFINITY_STRING.size());
    }
}


int fpconv_dtoa(double d, char* dest)
{
    char digits[18];
    int str_len = 0;
    int spec = filter_special(d, dest + str_len);
    if(spec) {
        return str_len + spec;
    }

    int K = 0;
    int ndigits = grisu2(d, digits, &K);
    str_len += emit_digits(digits, ndigits, dest + str_len, K);

    return str_len;
}


// V8 RADIX


/**
 *  \brief Returns true if the double is a denormal.
 */
bool v8_is_denormal(double d)
{
    uint64_t d64 = get_dbits(d);
    return (d64 & EXPONENT_MASK) == 0;
}

/**
 *  We consider denormals not to be special.
 *  Hence only Infinity and NaN are special.
 */
bool v8_is_special(double d)
{
    uint64_t d64 = get_dbits(d);
    return (d64 & EXPONENT_MASK) == EXPONENT_MASK;
}


bool v8_is_nan(double d)
{
    uint64_t d64 = get_dbits(d);
    return ((d64 & EXPONENT_MASK) == EXPONENT_MASK) && ((d64 & SIGNIFICAND_MASK) != 0);
}


bool v8_is_infinite(double d)
{
    uint64_t d64 = get_dbits(d);
    return ((d64 & EXPONENT_MASK) == EXPONENT_MASK) && ((d64 & SIGNIFICAND_MASK) == 0);
}


static int v8_sign(double d)
{
    uint64_t d64 = get_dbits(d);
    return (d64 & SIGN_MASK) == 0 ? 1 : -1;
}


int v8_exponent(double d)
{
    if (v8_is_denormal(d)) {
        return DENORMAL_EXPONENT;
    }

    uint64_t d64 = get_dbits(d);
    int biased_e = static_cast<int>((d64 & EXPONENT_MASK) >> PHYSICAL_SIGNIFICAND_SIZE);
    return biased_e - EXPONENT_BIAS;
}


static uint64_t v8_significand(double d)
{
    uint64_t d64 = get_dbits(d);
    uint64_t s = d64 & SIGNIFICAND_MASK;
    if (!v8_is_denormal(d)) {
      return s + HIDDEN_BIT;
    } else {
      return s;
    }
}


/**
 *  \brief Returns the next greater double. Returns +infinity on input +infinity.
 */
static double v8_next_double(double d)
{
    uint64_t d64 = get_dbits(d);
    if (d64 == U64_INFINITY) {
        return get_u64bits(U64_INFINITY);
    }
    if (v8_sign(d) < 0 && v8_significand(d) == 0) {
      // -0.0
      return 0.0;
    }
    if (v8_sign(d) < 0) {
        return get_u64bits(d64 - 1);
    } else {
        return get_u64bits(d64 + 1);
    }
}


static double v8_modulo(double x, double y)
{
#if defined(OS_WINDOWS)
    // Workaround MS fmod bugs. ECMA-262 says:
    // dividend is finite and divisor is an infinity => result equals dividend
    // dividend is a zero and divisor is nonzero finite => result equals dividend
    if (!(std::isfinite(x) && (!std::isfinite(y) && !std::isnan(y))) &&
        !(x == 0 && (y != 0 && std::isfinite(y)))) {
        x = fmod(x, y);
    }
    return x;
#elif defined(OS_AIX)
    // AIX raises an underflow exception for (Number.MIN_VALUE % Number.MAX_VALUE)
    feclearexcept(FE_ALL_EXCEPT);
    double result = std::fmod(x, y);
    int exception = fetestexcept(FE_UNDERFLOW);
    return (exception ? x : result);
#else
    return std::fmod(x, y);
#endif
}


static int naive_exponent(double d, uint8_t base)
{
    // std::floor returns the minimal value, which is our
    // desired exponent
    // log(1.1e-5) -> -4.95 -> -5
    // log(1.1e5) -> -5.04 -> 5
    return static_cast<int>(std::floor(std::log(d) / std::log(base)));
}


static void ftoa_naive(double d, char* first, char*& last, uint8_t base)
{
    assert(base >= 2 && base <= 36);

    // check for special cases
    int length = filter_special(d, first);
    if (length) {
        last = first + length;
        return;
    }

    // assert no special cases remain
    assert(std::isfinite(d));
    assert(d != 0.0);

    /**
     *  Store the first digit and up to `BUFFER_SIZE - 15` digits
     *  that occur from left-to-right in the decimal representation.
     *  For example, for the number 123.45, store the first digit `1`
     *  and `2345` as the remaining values. Then, decide on-the-fly
     *  if we need scientific or regular formatting.
     *
     *    BUFFER_SIZE
     *  - 1      # first digit
     *  - 1      # period
     *  - 1      # +/- sign
     *  - 2      # e and +/- sign
     *  - 9      # max exp is 308, in base2 is 9
     *  - 1      # null terminator
     *  = 15 characters of formatting required
     */
    static constexpr size_t max_nondigit_length = 15;
    static constexpr size_t max_digit_length = BUFFER_SIZE - max_nondigit_length;

    /**
     *  Temporary buffer for the result. We start with the decimal point in the
     *  middle and write to the left for the integer part and to the right for the
     *  fractional part. 1024 characters for the exponent and 52 for the mantissa
     *  either way, with additional space for sign, decimal point and string
     *  termination should be sufficient.
     */
    static constexpr size_t buffer_size = 2200;
    char buffer[buffer_size];
    size_t initial_position = buffer_size / 2;
    size_t integer_cursor = initial_position;
    size_t fraction_cursor = initial_position;

    // Split the value into an integer part and a fractional part.
    double integer = std::floor(d);
    double fraction = d - integer;

    // We only compute fractional digits up to the input double's precision.
    double delta = 0.5 * (v8_next_double(d) - d);
    delta = std::max(v8_next_double(0.0), delta);
    assert(delta > 0.0);

    if (fraction > delta) {
        do {
            // Shift up by one digit.
            fraction *= base;
            delta *= base;
            // Write digit.
            int digit = static_cast<int>(fraction);
            buffer[fraction_cursor++] = BASEN[digit];
            // Calculate remainder.
            fraction -= digit;
            // Round to even.
            if (fraction > 0.5 || (fraction == 0.5 && (digit & 1))) {
                if (fraction + delta > 1) {
                    // We need to back trace already written digits in case of carry-over.
                    while (true) {
                        fraction_cursor--;
                        if (fraction_cursor == initial_position-1) {
                            // Carry over to the integer part.
                            integer += 1;
                            break;
                        }
                        char c = buffer[fraction_cursor];
                        // Reconstruct digit.
                        int digit;
                        if (c <= '9') {
                            digit = c - '0';
                        } else if (c >= 'A' && c <= 'Z') {
                            digit = c - 'A' + 10;
                        } else {
                            assert(c >= 'a' && c <= 'z');
                            digit = c - 'a' + 10;
                        }
                        if (digit + 1 < base) {
                            buffer[fraction_cursor++] = BASEN[digit + 1];
                            break;
                        }
                    }
                    break;
                }
            }
        } while (fraction > delta);
    }

    // Compute integer digits. Fill unrepresented digits with zero.
    while (v8_exponent(integer / base) > 0) {
        integer /= base;
        buffer[--integer_cursor] = '0';
    }

    do {
        double remainder = v8_modulo(integer, base);
        buffer[--integer_cursor] = BASEN[static_cast<int>(remainder)];
        integer = (integer - remainder) / base;
    } while (integer > 0);

    if (d <= 1e-5 || d >= 1e9) {
        // write scientific notation with negative exponent
        int exponent = naive_exponent(d, base);

        // Non-exponent portion.
        // 1.   Get as many digits as possible, up to `max_digit_length+1`
        //      (since we are ignoring the digit for the first digit),
        //      or the number of written digits
        size_t buf_start, buf_end;
        if (d <= 1e-5) {
            buf_start = initial_position - exponent - 1;
            buf_end = std::min(fraction_cursor, buf_start + max_digit_length + 1);
        } else {
            buf_start = integer_cursor;
            buf_end = std::min(fraction_cursor, buf_start + max_digit_length + 1);
        }
        string_view buf_view(buffer + buf_start, buf_end - buf_start);

        // 2.   Remove any trailing 0s in the selected range.
        for (auto it = buf_view.rbegin(); *it == '0'; ++it, --buf_end)
            ;
        buf_view = string_view(buf_view.data(), buf_end - buf_start);

        // 3.   Write the fraction component
        *first++ = buf_view[0];
        *first++ = '.';
        memcpy(first, buf_view.data()+1, buf_view.size() - 1);
        first += buf_view.size() - 1;

        // write the exponent component
        *first++ = e_notation_char(base);
        i32toa(exponent, first, last, base);

    } else {
        last = first;
        // get component lengths
        size_t integer_length = initial_position - integer_cursor;
        size_t fraction_length = std::min(fraction_cursor - initial_position, max_digit_length - integer_length);

        // write integer component123450000000
        memcpy(first, buffer + integer_cursor, integer_length);
        last = first + integer_length;

        // write fraction component
        if (fraction_length > 0) {
            *last++ = '.';
            memcpy(last, buffer + initial_position, fraction_length);
            last += fraction_length;
        }
    }
}


// BASEN


static void ftoa_base10(double value, char* first, char*& last)
{
    int len = fpconv_dtoa(value, first);
    last = first + len;
}


static void ftoa_basen(double value, char* first, char*& last, uint8_t base)
{
    ftoa_naive(value, first, last, base);
}


static void ftoa_impl(double value, char* first, char*& last, uint8_t base)
{
    // disable this check in release builds, since it's a logic error
    assert(last - first >= BUFFER_SIZE && "Need a larger buffer.");

    // handle negative numbers
    char* tmp = last - 1;
    if (value < 0) {
        first[0]= '-';
        value = -value;
        first++;
    }

    // use optimized functions if possible
    switch (base) {
        case 10:        ftoa_base10(value, first, tmp);         break;
        default:        ftoa_basen(value, first, tmp, base);    break;
    }

    // add a trailing null character
    *tmp = '\0';
    last = tmp;
}


void ftoa_(double value, char* first, char*& last, uint8_t base)
{
    // sanity checks
    assert(first <= last);

    // check to use a temporary buffer
    size_t distance = std::distance(first, last);
    if (distance == 0) {
        // cannot write null terminator
        return;
    } if (distance < BUFFER_SIZE) {
        // use a temporary buffer and write number to buffer
        char buffer[BUFFER_SIZE];
        char* first_ = buffer;
        char* last_ = first_ + BUFFER_SIZE;
        ftoa_impl(value, first_, last_, base);

        // copy as many bytes as possible except the trailing null byte
        // then, write null-byte so the string is always terminated
        size_t length = std::min<size_t>(std::distance(first_, last_), distance) - 1;
        memcpy(first, first_, length);
        last = first + length;
        *last = '\0';
    } else {
        // current buffer has sufficient capacity, use it
        ftoa_impl(value, first, last, base);
    }
}

// FUNCTIONS
// ---------


void f32toa(float value, char* first, char*& last, uint8_t base)
{
    ftoa_(value, first, last, base);
}


std::string f32toa(float value, uint8_t base)
{
    char buffer[BUFFER_SIZE];
    char* last = buffer + BUFFER_SIZE;
    f32toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void f64toa(double value, char* first, char*& last, uint8_t base)
{
    ftoa_(value, first, last, base);
}


std::string f64toa(double value, uint8_t base)
{
    char buffer[BUFFER_SIZE];
    char* last = buffer + BUFFER_SIZE;
    f64toa(value, buffer, last, base);
    return std::string(buffer, last);
}


PYCPP_END_NAMESPACE
