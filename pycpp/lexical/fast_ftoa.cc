//  :copyright: (c) 2010 V8 Project Authors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: BSD-3, see licenses/bsd-3.md for more details.

#include <pycpp/lexical/diyfp.h>
#include <pycpp/lexical/fast_ftoa.h>
#include <cassert>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

/**
 *  The minimal and maximal target exponent define the range of w's binary
 *  exponent, where 'w' is the result of multiplying the input by a cached power
 *  of ten.
 *
 *  A different range might be chosen on a different platform, to optimize digit
 *  generation, but a smaller range requires more powers of ten to be cached.
 */
static const int kMinimalTargetExponent = -60;
static const int kMaximalTargetExponent = -32;

static unsigned int const SMALL_POWERS_OF_TEN[] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

// FUNCTIONS
// ---------

/**
 *  Adjusts the last digit of the generated number, and screens out generated
 *  solutions that may be inaccurate. A solution may be inaccurate if it is
 *  outside the safe interval, or if we cannot prove that it is closer to the
 *  input than a neighboring representation of the same length.
 *
 *  Input: * buffer containing the digits of too_high / 10^kappa
 *         * the buffer's length
 *         * distance_too_high_w == (too_high - w).f() * unit
 *         * unsafe_interval == (too_high - too_low).f() * unit
 *         * rest = (too_high - buffer * 10^kappa).f() * unit
 *         * ten_kappa = 10^kappa * unit
 *         * unit = the common multiplier
 *  Output: returns true if the buffer is guaranteed to contain the closest
 *     representable number to the input.
 *   Modifies the generated digits in the buffer to approach (round towards) w.
 */
static bool round_weed(char* buffer, int length, uint64_t distance_too_high_w,
                       uint64_t unsafe_interval, uint64_t rest,
                       uint64_t ten_kappa, uint64_t unit)
{
    uint64_t small_distance = distance_too_high_w - unit;
    uint64_t big_distance = distance_too_high_w + unit;
    /**
     *  Let w_low  = too_high - big_distance, and
     *     w_high = too_high - small_distance.
     * Note: w_low < w < w_high
     *
     * The real w (* unit) must lie somewhere inside the interval
     * ]w_low; w_high[ (often written as "(w_low; w_high)")
     *
     * Basically the buffer currently contains a number in the unsafe interval
     * ]too_low; too_high[ with too_low < w < too_high
     *
     *  too_high - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     *                     ^v 1 unit            ^      ^                 ^      ^
     *  boundary_high ---------------------     .      .                 .      .
     *                     ^v 1 unit            .      .                 .      .
     *   - - - - - - - - - - - - - - - - - - -  +  - - + - - - - - -     .      .
     *                                          .      .         ^       .      .
     *                                          .  big_distance  .       .      .
     *                                          .      .         .       .    rest
     *                              small_distance     .         .       .      .
     *                                          v      .         .       .      .
     *  w_high - - - - - - - - - - - - - - - - - -     .         .       .      .
     *                     ^v 1 unit                   .         .       .      .
     *  w ----------------------------------------     .         .       .      .
     *                     ^v 1 unit                   v         .       .      .
     *  w_low  - - - - - - - - - - - - - - - - - - - - -         .       .      .
     *                                                           .       .      v
     *  buffer --------------------------------------------------+-------+--------
     *                                                           .       .
     *                                                  safe_interval    .
     *                                                           v       .
     *   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -     .
     *                     ^v 1 unit                                     .
     *  boundary_low -------------------------                     unsafe_interval
     *                     ^v 1 unit                                     v
     *  too_low  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     *
     *
     * Note that the value of buffer could lie anywhere inside the range too_low
     * to too_high.
     *
     * boundary_low, boundary_high and w are approximations of the real boundaries
     * and v (the input number). They are guaranteed to be precise up to one unit.
     * In fact the error is guaranteed to be strictly less than one unit.
     *
     * Anything that lies outside the unsafe interval is guaranteed not to round
     * to v when read again.
     * Anything that lies inside the safe interval is guaranteed to round to v
     * when read again.
     * If the number inside the buffer lies inside the unsafe interval but not
     * inside the safe interval then we simply do not know and bail out (returning
     * false).
     *
     * Similarly we have to take into account the imprecision of 'w' when finding
     * the closest representation of 'w'. If we have two potential
     * representations, and one is closer to both w_low and w_high, then we know
     * it is closer to the actual value v.
     *
     * By generating the digits of too_high we got the largest (closest to
     * too_high) buffer that is still in the unsafe interval. In the case where
     * w_high < buffer < too_high we try to decrement the buffer.
     * This way the buffer approaches (rounds towards) w.
     * There are 3 conditions that stop the decrementation process:
     *   1) the buffer is already below w_high
     *   2) decrementing the buffer would make it leave the unsafe interval
     *   3) decrementing the buffer would yield a number below w_high and farther
     *      away than the current number. In other words:
     *              (buffer{-1} < w_high) && w_high - buffer{-1} > buffer - w_high
     * Instead of using the buffer directly we use its distance to too_high.
     * Conceptually rest ~= too_high - buffer
     * We need to do the following tests in this order to avoid over- and
     * underflows.
     */
    assert(rest <= unsafe_interval);
    while (rest < small_distance &&                 // Negated condition 1
           unsafe_interval - rest >= ten_kappa &&   // Negated condition 2
           (rest + ten_kappa < small_distance ||    // buffer{-1} > w_high
            small_distance - rest >= rest + ten_kappa - small_distance)) {
      buffer[length - 1]--;
      rest += ten_kappa;
    }

    /**
     *  We have approached w+ as much as possible. We now test if approaching w-
     *  would require changing the buffer. If yes, then we have two possible
     *  representations close to w, but we cannot decide which one is closer.
     */
    if (rest < big_distance &&
        unsafe_interval - rest >= ten_kappa &&
        (rest + ten_kappa < big_distance ||
        big_distance - rest > rest + ten_kappa - big_distance)) {
        return false;
    }

    /**
     *  Weeding test.
     *  The safe interval is [too_low + 2 ulp; too_high - 2 ulp]
     *  Since too_low = too_high - unsafe_interval this is equivalent to
     *     [too_high - unsafe_interval + 4 ulp; too_high - 2 ulp]
     *  Conceptually we have: rest ~= too_high - buffer
     */
    return (2 * unit <= rest) && (rest <= unsafe_interval - 4 * unit);
}


/**
 *  Rounds the buffer upwards if the result is closer to v by possibly adding
 *  1 to the buffer. If the precision of the calculation is not sufficient to
 *  round correctly, return false.
 *  The rounding might shift the whole buffer in which case the kappa is
 *  adjusted. For example "99", kappa = 3 might become "10", kappa = 4.
 *
 *  If 2*rest > ten_kappa then the buffer needs to be round up.
 *  rest can have an error of +/- 1 unit. This function accounts for the
 *  imprecision and returns false, if the rounding direction cannot be
 *  unambiguously determined.
 *
 *  Precondition: rest < ten_kappa.
 */
static bool round_weed_counted(char* buffer, int length, uint64_t rest,
                               uint64_t ten_kappa, uint64_t unit, int* kappa)
{
    assert(rest < ten_kappa);
    /**
     *  The following tests are done in a specific order to avoid overflows. They
     *  will work correctly with any uint64 values of rest < ten_kappa and unit.
     *
     *  If the unit is too big, then we don't know which way to round. For example
     *  a unit of 50 means that the real number lies within rest +/- 50. If
     *  10^kappa == 40 then there is no way to tell which way to round.
     */
    if (unit >= ten_kappa) {
        return false;
    }
    /**
     *  Even if unit is just half the size of 10^kappa we are already completely
     *  lost. (And after the previous test we know that the expression will not
     *  over/underflow.)
     */
    if (ten_kappa - unit <= unit) {
        return false;
    }
    // If 2 * (rest + unit) <= 10^kappa we can safely round down.
    if ((ten_kappa - rest > rest) && (ten_kappa - 2 * rest >= 2 * unit)) {
        return true;
    }
    // If 2 * (rest - unit) >= 10^kappa, then we can safely round up.
    if ((rest > unit) && (ten_kappa - (rest - unit) <= (rest - unit))) {
        // Increment the last digit recursively until we find a non '9' digit.
        buffer[length - 1]++;
        for (int i = length - 1; i > 0; --i) {
            if (buffer[i] != '0' + 10) {
                break;
            }
            buffer[i] = '0';
            buffer[i - 1]++;
        }
        /**
         *  If the first digit is now '0'+ 10 we had a buffer with all '9's. With the
         *  exception of the first digit all digits are now '0'. Simply switch the
         *  first digit to '1' and adjust the kappa. Example: "99" becomes "10" and
         *  the power (the kappa) is increased.
         */
        if (buffer[0] == '0' + 10) {
            buffer[0] = '1';
            (*kappa) += 1;
        }
        return true;
    }
    return false;
}


/** Returns the biggest power of ten that is less than or equal to the given
 *  number. We furthermore receive the maximum number of bits 'number' has.
 *
 *  Returns power == 10^(exponent_plus_one-1) such that
 *     power <= number < power * 10.
 *  If number_bits == 0 then 0^(0-1) is returned.
 *  The number of bits must be <= 32.
 *  Precondition: number < (1 << (number_bits + 1)).
 *
 *  Inspired by the method for finding an integer log base 10 from here:
 *  http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
 */
static void biggest_power_of_ten(uint32_t number, int number_bits,
                                 uint32_t* power, int* exponent_plus_one)
{
    assert(number < (1u << (number_bits + 1)));
    // 1233/4096 is approximately 1/lg(10).
    int exponent_plus_one_guess = ((number_bits + 1) * 1233 >> 12);
    // We increment to skip over the first entry in the kPowersOf10 table.
    // Note: kPowersOf10[i] == 10^(i-1).
    exponent_plus_one_guess++;
    // We don't have any guarantees that 2^number_bits <= number.
    if (number < SMALL_POWERS_OF_TEN[exponent_plus_one_guess]) {
        exponent_plus_one_guess--;
    }
    *power = SMALL_POWERS_OF_TEN[exponent_plus_one_guess];
    *exponent_plus_one = exponent_plus_one_guess;
}


/**
 *  Generates the digits of input number w.
 *  w is a floating-point number (DiyFp), consisting of a significand and an
 *  exponent. Its exponent is bounded by kMinimalTargetExponent and
 *  kMaximalTargetExponent.
 *        Hence -60 <= w.e() <= -32.
 *
 *  Returns false if it fails, in which case the generated digits in the buffer
 *  should not be used.
 *  Preconditions:
 *   * low, w and high are correct up to 1 ulp (unit in the last place). That
 *     is, their error must be less than a unit of their last digits.
 *   * low.e() == w.e() == high.e()
 *   * low < w < high, and taking into account their error: low~ <= high~
 *   * kMinimalTargetExponent <= w.e() <= kMaximalTargetExponent
 *  Postconditions: returns false if procedure fails.
 *    otherwise:
 *      * buffer is not null-terminated, but len contains the number of digits.
 *      * buffer contains the shortest possible decimal digit-sequence
 *        such that LOW < buffer * 10^kappa < HIGH, where LOW and HIGH are the
 *        correct values of low and high (without their error).
 *      * if more than one decimal representation gives the minimal number of
 *        decimal digits then the one closest to W (where W is the correct value
 *        of w) is chosen.
 *  Remark: this procedure takes into account the imprecision of its input
 *    numbers. If the precision is not enough to guarantee all the postconditions
 *    then false is returned. This usually happens rarely (~0.5%).
 *
 *  Say, for the sake of example, that
 *    w.e() == -48, and w.f() == 0x1234567890abcdef
 *  w's value can be computed by w.f() * 2^w.e()
 *  We can obtain w's integral digits by simply shifting w.f() by -w.e().
 *   -> w's integral part is 0x1234
 *   w's fractional part is therefore 0x567890abcdef.
 *  Printing w's integral part is easy (simply print 0x1234 in decimal).
 *  In order to print its fraction we repeatedly multiply the fraction by 10 and
 *  get each digit. Example the first digit after the point would be computed by
 *    (0x567890abcdef * 10) >> 48. -> 3
 *  The whole thing becomes slightly more complicated because we want to stop
 *  once we have enough digits. That is, once the digits inside the buffer
 *  represent 'w' we can stop. Everything inside the interval low - high
 *  represents w. However we have to pay attention to low, high and w's
 *  imprecision.
 */
static bool digit_gen(diyfp_t low, diyfp_t w, diyfp_t high, char* buffer,
                      int* length, int* kappa)
{
    assert(low.e() == w.e() && w.e() == high.e());
    assert(low.f() + 1 <= high.f() - 1);
    assert(kMinimalTargetExponent <= w.e() && w.e() <= kMaximalTargetExponent);
    /**
     *  low, w and high are imprecise, but by less than one ulp (unit in the last
     *  place).
     *  If we remove (resp. add) 1 ulp from low (resp. high) we are certain that
     *  the new numbers are outside of the interval we want the final
     *  representation to lie in.
     *  Inversely adding (resp. removing) 1 ulp from low (resp. high) would yield
     *  numbers that are certain to lie in the interval. We will use this fact
     *  later on.
     *  We will now start by generating the digits within the uncertain
     *  interval. Later we will weed out representations that lie outside the safe
     *  interval and thus _might_ lie outside the correct interval.
     */
    uint64_t unit = 1;
    diyfp_t too_low = diyfp_t(low.f() - unit, low.e());
    diyfp_t too_high = diyfp_t(high.f() + unit, high.e());
    // too_low and too_high are guaranteed to lie outside the interval we want the
    // generated number in.
    diyfp_t unsafe_interval = too_high - too_low;
    /**
     *  We now cut the input number into two parts: the integral digits and the
     *  fractionals. We will not write any decimal separator though, but adapt
     *  kappa instead.
     *  Reminder: we are currently computing the digits (stored inside the buffer)
     *  such that:   too_low < buffer * 10^kappa < too_high
     *  We use too_high for the digit_generation and stop as soon as possible.
     *  If we stop early we effectively round down.
     */
    diyfp_t one = diyfp_t(static_cast<uint64_t>(1) << -w.e(), w.e());
    // Division by one is a shift.
    uint32_t integrals = static_cast<uint32_t>(too_high.f() >> -one.e());
    // Modulo by one is an and.
    uint64_t fractionals = too_high.f() & (one.f() - 1);
    uint32_t divisor;
    int divisor_exponent_plus_one;
    biggest_power_of_ten(integrals, diyfp_t::kSignificandSize - (-one.e()), &divisor, &divisor_exponent_plus_one);
    *kappa = divisor_exponent_plus_one;
    *length = 0;
    /**
     *  Loop invariant: buffer = too_high / 10^kappa  (integer division)
     *  The invariant holds for the first iteration: kappa has been initialized
     *  with the divisor exponent + 1. And the divisor is the biggest power of ten
     *  that is smaller than integrals.
     */
    while (*kappa > 0) {
        int digit = integrals / divisor;
        assert(digit <= 9);
        buffer[*length] = static_cast<char>('0' + digit);
        (*length)++;
        integrals %= divisor;
        (*kappa)--;
        // Note that kappa now equals the exponent of the divisor and that the
        // invariant thus holds again.
        uint64_t rest = (static_cast<uint64_t>(integrals) << -one.e()) + fractionals;
        // Invariant: too_high = buffer * 10^kappa + DiyFp(rest, one.e())
        // Reminder: unsafe_interval.e() == one.e()
        if (rest < unsafe_interval.f()) {
            // Rounding down (by not emitting the remaining digits) yields a number
            // that lies within the unsafe interval.
            return round_weed(buffer, *length, (too_high - w).f(),
                              unsafe_interval.f(), rest,
                              static_cast<uint64_t>(divisor) << -one.e(), unit);
        }
        divisor /= 10;
    }

    /**
     *  The integrals have been generated. We are at the point of the decimal
     *  separator. In the following loop we simply multiply the remaining digits by
     *  10 and divide by one. We just need to pay attention to multiply associated
     *  data (like the interval or 'unit'), too.
     *  Note that the multiplication by 10 does not overflow, because w.e >= -60
     *  and thus one.e >= -60.
     */
    assert(one.e() >= -60);
    assert(fractionals < one.f());
    assert(0xFFFFFFFFFFFFFFFFULL / 10 >= one.f());
    for (;;) {
        fractionals *= 10;
        unit *= 10;
        unsafe_interval.f(unsafe_interval.f() * 10);
        // Integer division by one.
        int digit = static_cast<int>(fractionals >> -one.e());
        assert(digit <= 9);
        buffer[*length] = static_cast<char>('0' + digit);
        (*length)++;
        fractionals &= one.f() - 1;  // Modulo by one.
        (*kappa)--;
        if (fractionals < unsafe_interval.f()) {
            return round_weed(buffer, *length, (too_high - w).f() * unit,
                              unsafe_interval.f(), fractionals, one.f(), unit);
        }
    }
}


/**
 *  Generates (at most) requested_digits digits of input number w.
 *  w is a floating-point number (DiyFp), consisting of a significand and an
 *  exponent. Its exponent is bounded by kMinimalTargetExponent and
 *  kMaximalTargetExponent.
 *        Hence -60 <= w.e() <= -32.
 *
 *  Returns false if it fails, in which case the generated digits in the buffer
 *  should not be used.
 *  Preconditions:
 *   * w is correct up to 1 ulp (unit in the last place). That
 *     is, its error must be strictly less than a unit of its last digit.
 *   * kMinimalTargetExponent <= w.e() <= kMaximalTargetExponent
 *
 *  Postconditions: returns false if procedure fails.
 *    otherwise:
 *      * buffer is not null-terminated, but length contains the number of
 *        digits.
 *      * the representation in buffer is the most precise representation of
 *        requested_digits digits.
 *      * buffer contains at most requested_digits digits of w. If there are less
 *        than requested_digits digits then some trailing '0's have been removed.
 *      * kappa is such that
 *             w = buffer * 10^kappa + eps with |eps| < 10^kappa / 2.
 *
 *  Remark: This procedure takes into account the imprecision of its input
 *    numbers. If the precision is not enough to guarantee all the postconditions
 *    then false is returned. This usually happens rarely, but the failure-rate
 *    increases with higher requested_digits.
 */
static bool digit_gen_counted(diyfp_t w, int requested_digits, char* buffer,
                              int* length, int* kappa)
{
    assert(kMinimalTargetExponent <= w.e() && w.e() <= kMaximalTargetExponent);
    assert(kMinimalTargetExponent >= -60);
    assert(kMaximalTargetExponent <= -32);
    // w is assumed to have an error less than 1 unit. Whenever w is scaled we
    // also scale its error.
    uint64_t w_error = 1;
    // We cut the input number into two parts: the integral digits and the
    // fractional digits. We don't emit any decimal separator, but adapt kappa
    // instead. Example: instead of writing "1.2" we put "12" into the buffer and
    // increase kappa by 1.
    diyfp_t one = diyfp_t(static_cast<uint64_t>(1) << -w.e(), w.e());
    // Division by one is a shift.
    uint32_t integrals = static_cast<uint32_t>(w.f() >> -one.e());
    // Modulo by one is an and.
    uint64_t fractionals = w.f() & (one.f() - 1);
    uint32_t divisor;
    int divisor_exponent_plus_one;
    biggest_power_of_ten(integrals, diyfp_t::kSignificandSize - (-one.e()),
                        &divisor, &divisor_exponent_plus_one);
    *kappa = divisor_exponent_plus_one;
    *length = 0;

    /**
     *  Loop invariant: buffer = w / 10^kappa  (integer division)
     *  The invariant holds for the first iteration: kappa has been initialized
     *  with the divisor exponent + 1. And the divisor is the biggest power of ten
     *  that is smaller than 'integrals'.
     */
    while (*kappa > 0) {
        int digit = integrals / divisor;
        assert(digit <= 9);
        buffer[*length] = static_cast<char>('0' + digit);
        (*length)++;
        requested_digits--;
        integrals %= divisor;
        (*kappa)--;
        // Note that kappa now equals the exponent of the divisor and that the
        // invariant thus holds again.
        if (requested_digits == 0) break;
        divisor /= 10;
    }

    if (requested_digits == 0) {
        uint64_t rest = (static_cast<uint64_t>(integrals) << -one.e()) + fractionals;
        return round_weed_counted(buffer, *length, rest,
                                  static_cast<uint64_t>(divisor) << -one.e(),
                                  w_error, kappa);
    }

    /**
     *  The integrals have been generated. We are at the point of the decimal
     *  separator. In the following loop we simply multiply the remaining digits by
     *  10 and divide by one. We just need to pay attention to multiply associated
     *  data (the 'unit'), too.
     *  Note that the multiplication by 10 does not overflow, because w.e >= -60
     *  and thus one.e >= -60.
     */
    assert(one.e() >= -60);
    assert(fractionals < one.f());
    assert(0xFFFFFFFFFFFFFFFFULL / 10 >= one.f());
    while (requested_digits > 0 && fractionals > w_error) {
        fractionals *= 10;
        w_error *= 10;
        // Integer division by one.
        int digit = static_cast<int>(fractionals >> -one.e());
        assert(digit <= 9);
        buffer[*length] = static_cast<char>('0' + digit);
        (*length)++;
        requested_digits--;
        fractionals &= one.f() - 1;  // Modulo by one.
        (*kappa)--;
    }
    if (requested_digits != 0) {
        return false;
    }
    return round_weed_counted(buffer, *length, fractionals, one.f(), w_error, kappa);
}


PYCPP_END_NAMESPACE
