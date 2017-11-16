//  :copyright: (c) 2011 V8 Project Authors
//  :copyright: (c) 2013 Andreas Samoljuk
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  The optimized routines are from Andrea Samoljuk's `fpconv` library,
 *  which is available here:
 *      `https://github.com/night-shift/fpconv`
 */

#include <pycpp/lexical/format.h>
#include <pycpp/lexical/ftoa.h>
#include <pycpp/lexical/table.h>
#include <cassert>
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

// CONSTANTS
// ---------

static constexpr size_t BUFFER_SIZE = 60;
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

// GRISU

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
    union {
        double dbl;
        uint64_t i;
    } dbl_bits = { d };

    return dbl_bits.i;
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
    uint64_t* divp;
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
    uint64_t* unit = TENS + 18;

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

    dest[idx++] = 'e';

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


// RADIX


static void ftoa_naive(double d, char* first, char*& last, uint8_t base)
{
    assert(radix >= 2 && radix <= 36);

    // check for special cases
    int length = filter_special(d, first);
    if(length) {
        last = first + length;
        return;
    }

    // assert no special cases remain
    assert(std::isfinite(d));
    assert(d != 0.0);

    /**
     *  Store the first digit and up to `BUFFER_SIZE - 14` digits
     *  that occur from left-to-right in the decimal representation.
     *  For example, for the number 123.45, store the first digit `1`
     *  and `2345` as the remaining values. Then, decide on-the-fly
     *  if we need scientific or regular formatting.
     *
     *    BUFFER_SIZE
     *  - 1      # first nummber
     *  - 1      # period
     *  - 1      # +/- sign
     *  - 2      # e and +/- sign
     *  - 9      # max exp is 308, in base2 is 9
     *  = 14 characters of formatting required
     */
    static constexpr size_t max_nonfraction_length = 14;
    static constexpr size_t max_fractionn_length = BUFFER_SIZE - max_nonfraction_length;

    /**
     *  Temporary buffer for the result. We start with the decimal point in the
     *  middle and write to the left for the integer part and to the right for the
     *  fractional part. 1024 characters for the exponent and 52 for the mantissa
     *  either way, with additional space for sign, decimal point and string
     *  termination should be sufficient.
     */
    static constexpr size_t buffer_size = 2200;
    char buffer[buffer_size];
    size_t integer_cursor = buffer_size / 2;
    size_t fraction_cursor = integer_cursor;

    // Split the value into an integer part and a fractional part.
    double integer = std::floor(value);
    double fraction = value - integer;

    // We only compute fractional digits up to the input double's precision.
//    double delta = 0.5 * (Double(value).NextDouble() - value);
//  delta = std::max(Double(0.0).NextDouble(), delta);
}

// TODO: use this for the Radix C-string
//char* DoubleToRadixCString(double value, int radix) {
//


//  DCHECK_GT(delta, 0.0);
//  if (fraction > delta) {
//    // Insert decimal point.
//    buffer[fraction_cursor++] = '.';
//    do {
//      // Shift up by one digit.
//      fraction *= radix;
//      delta *= radix;
//      // Write digit.
//      int digit = static_cast<int>(fraction);
//      buffer[fraction_cursor++] = BASEN[digit];
//      // Calculate remainder.
//      fraction -= digit;
//      // Round to even.
//      if (fraction > 0.5 || (fraction == 0.5 && (digit & 1))) {
//        if (fraction + delta > 1) {
//          // We need to back trace already written digits in case of carry-over.
//          while (true) {
//            fraction_cursor--;
//            if (fraction_cursor == kBufferSize / 2) {
//              CHECK_EQ('.', buffer[fraction_cursor]);
//              // Carry over to the integer part.
//              integer += 1;
//              break;
//            }
//            char c = buffer[fraction_cursor];
//            // Reconstruct digit.
//            int digit = c > '9' ? (c - 'a' + 10) : (c - '0');
//            if (digit + 1 < radix) {
//              buffer[fraction_cursor++] = BASEN[digit + 1];
//              break;
//            }
//          }
//          break;
//        }
//      }
//    } while (fraction > delta);
//  }
//
//  // Compute integer digits. Fill unrepresented digits with zero.
//  while (Double(integer / radix).Exponent() > 0) {
//    integer /= radix;
//    buffer[--integer_cursor] = '0';
//  }
//  do {
//    double remainder = Modulo(integer, radix);
//    buffer[--integer_cursor] = BASEN[static_cast<int>(remainder)];
//    integer = (integer - remainder) / radix;
//  } while (integer > 0);
//
//  // Add sign and terminate string.
//  if (negative) buffer[--integer_cursor] = '-';
//  buffer[fraction_cursor++] = '\0';
//  DCHECK_LT(fraction_cursor, kBufferSize);
//  DCHECK_LE(0, integer_cursor);
//  // Allocate new string as return value.
//  char* result = NewArray<char>(fraction_cursor - integer_cursor);
//  memcpy(result, buffer + integer_cursor, fraction_cursor - integer_cursor);
//  return result;
//}


// BASEN


static void ftoa_base10(double value, char* first, char*& last)
{
    int len = fpconv_dtoa(value, first);
    last = first + len;
}


static void ftoa_basen(double value, char* first, char*& last, uint8_t base)
{
    // TODO: may change signatures
    // Workflow
    // 1.   Check for special values (-inf, inf, NaN)
    // 2.   Write the mantissa without the 1 or decimal.
    // 3.   Wait... Do I write the mantissa, since it's base2, not 10....
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


void f64toa(double value, char* first, char*& last, uint8_t base)
{
    ftoa_(value, first, last, base);
}


PYCPP_END_NAMESPACE
