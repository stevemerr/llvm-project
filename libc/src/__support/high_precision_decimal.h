//===-- High Precision Decimal ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See httpss//llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// -----------------------------------------------------------------------------
//                               **** WARNING ****
// This file is shared with libc++. You should also be careful when adding
// dependencies to this file, since it needs to build for all libc++ targets.
// -----------------------------------------------------------------------------

#ifndef LLVM_LIBC_SRC___SUPPORT_HIGH_PRECISION_DECIMAL_H
#define LLVM_LIBC_SRC___SUPPORT_HIGH_PRECISION_DECIMAL_H

#include "hdr/stdint_proxy.h"
#include "src/__support/CPP/limits.h"
#include "src/__support/ctype_utils.h"
#include "src/__support/macros/config.h"
#include "src/__support/str_to_integer.h"

namespace LIBC_NAMESPACE_DECL {
namespace internal {

struct LShiftTableEntry {
  uint32_t new_digits;
  char const *power_of_five;
};

// -----------------------------------------------------------------------------
//                               **** WARNING ****
// This interface is shared with libc++, if you change this interface you need
// to update it in both libc and libc++.
// -----------------------------------------------------------------------------
// This is used in both this file and in the main str_to_float.h.
// TODO: Figure out where to put this.
enum class RoundDirection { Up, Down, Nearest };

// This is based on the HPD data structure described as part of the Simple
// Decimal Conversion algorithm by Nigel Tao, described at this link:
// https://nigeltao.github.io/blog/2020/parse-number-f64-simple.html
class HighPrecisionDecimal {

  // This precomputed table speeds up left shifts by having the number of new
  // digits that will be added by multiplying 5^i by 2^i. If the number is less
  // than 5^i then it will add one fewer digit. There are only 60 entries since
  // that's the max shift amount.
  // This table was generated by the script at
  // libc/utils/mathtools/GenerateHPDConstants.py
  static constexpr LShiftTableEntry LEFT_SHIFT_DIGIT_TABLE[] = {
      {0, ""},
      {1, "5"},
      {1, "25"},
      {1, "125"},
      {2, "625"},
      {2, "3125"},
      {2, "15625"},
      {3, "78125"},
      {3, "390625"},
      {3, "1953125"},
      {4, "9765625"},
      {4, "48828125"},
      {4, "244140625"},
      {4, "1220703125"},
      {5, "6103515625"},
      {5, "30517578125"},
      {5, "152587890625"},
      {6, "762939453125"},
      {6, "3814697265625"},
      {6, "19073486328125"},
      {7, "95367431640625"},
      {7, "476837158203125"},
      {7, "2384185791015625"},
      {7, "11920928955078125"},
      {8, "59604644775390625"},
      {8, "298023223876953125"},
      {8, "1490116119384765625"},
      {9, "7450580596923828125"},
      {9, "37252902984619140625"},
      {9, "186264514923095703125"},
      {10, "931322574615478515625"},
      {10, "4656612873077392578125"},
      {10, "23283064365386962890625"},
      {10, "116415321826934814453125"},
      {11, "582076609134674072265625"},
      {11, "2910383045673370361328125"},
      {11, "14551915228366851806640625"},
      {12, "72759576141834259033203125"},
      {12, "363797880709171295166015625"},
      {12, "1818989403545856475830078125"},
      {13, "9094947017729282379150390625"},
      {13, "45474735088646411895751953125"},
      {13, "227373675443232059478759765625"},
      {13, "1136868377216160297393798828125"},
      {14, "5684341886080801486968994140625"},
      {14, "28421709430404007434844970703125"},
      {14, "142108547152020037174224853515625"},
      {15, "710542735760100185871124267578125"},
      {15, "3552713678800500929355621337890625"},
      {15, "17763568394002504646778106689453125"},
      {16, "88817841970012523233890533447265625"},
      {16, "444089209850062616169452667236328125"},
      {16, "2220446049250313080847263336181640625"},
      {16, "11102230246251565404236316680908203125"},
      {17, "55511151231257827021181583404541015625"},
      {17, "277555756156289135105907917022705078125"},
      {17, "1387778780781445675529539585113525390625"},
      {18, "6938893903907228377647697925567626953125"},
      {18, "34694469519536141888238489627838134765625"},
      {18, "173472347597680709441192448139190673828125"},
      {19, "867361737988403547205962240695953369140625"},
  };

  // The maximum amount we can shift is the number of bits used in the
  // accumulator, minus the number of bits needed to represent the base (in this
  // case 4).
  static constexpr uint32_t MAX_SHIFT_AMOUNT = sizeof(uint64_t) - 4;

  // 800 is an arbitrary number of digits, but should be
  // large enough for any practical number.
  static constexpr uint32_t MAX_NUM_DIGITS = 800;

  uint32_t num_digits = 0;
  int32_t decimal_point = 0;
  bool truncated = false;
  uint8_t digits[MAX_NUM_DIGITS];

private:
  LIBC_INLINE bool should_round_up(int32_t round_to_digit,
                                   RoundDirection round) {
    if (round_to_digit < 0 ||
        static_cast<uint32_t>(round_to_digit) >= this->num_digits) {
      return false;
    }

    // The above condition handles all cases where all of the trailing digits
    // are zero. In that case, if the rounding mode is up, then this number
    // should be rounded up. Similarly, if the rounding mode is down, then it
    // should always round down.
    if (round == RoundDirection::Up) {
      return true;
    } else if (round == RoundDirection::Down) {
      return false;
    }
    // Else round to nearest.

    // If we're right in the middle and there are no extra digits
    if (this->digits[round_to_digit] == 5 &&
        static_cast<uint32_t>(round_to_digit + 1) == this->num_digits) {

      // Round up if we've truncated (since that means the result is slightly
      // higher than what's represented.)
      if (this->truncated) {
        return true;
      }

      // If this exactly halfway, round to even.
      if (round_to_digit == 0)
        // When the input is ".5".
        return false;
      return this->digits[round_to_digit - 1] % 2 != 0;
    }
    // If there are digits after round_to_digit, they must be non-zero since we
    // trim trailing zeroes after all operations that change digits.
    return this->digits[round_to_digit] >= 5;
  }

  // Takes an amount to left shift and returns the number of new digits needed
  // to store the result based on LEFT_SHIFT_DIGIT_TABLE.
  LIBC_INLINE uint32_t get_num_new_digits(uint32_t lshift_amount) {
    const char *power_of_five =
        LEFT_SHIFT_DIGIT_TABLE[lshift_amount].power_of_five;
    uint32_t new_digits = LEFT_SHIFT_DIGIT_TABLE[lshift_amount].new_digits;
    uint32_t digit_index = 0;
    while (power_of_five[digit_index] != 0) {
      if (digit_index >= this->num_digits) {
        return new_digits - 1;
      }
      if (this->digits[digit_index] !=
          internal::b36_char_to_int(power_of_five[digit_index])) {
        return new_digits -
               ((this->digits[digit_index] <
                 internal::b36_char_to_int(power_of_five[digit_index]))
                    ? 1
                    : 0);
      }
      ++digit_index;
    }
    return new_digits;
  }

  // Trim all trailing 0s
  LIBC_INLINE void trim_trailing_zeroes() {
    while (this->num_digits > 0 && this->digits[this->num_digits - 1] == 0) {
      --this->num_digits;
    }
    if (this->num_digits == 0) {
      this->decimal_point = 0;
    }
  }

  // Perform a digitwise binary non-rounding right shift on this value by
  // shift_amount. The shift_amount can't be more than MAX_SHIFT_AMOUNT to
  // prevent overflow.
  LIBC_INLINE void right_shift(uint32_t shift_amount) {
    uint32_t read_index = 0;
    uint32_t write_index = 0;

    uint64_t accumulator = 0;

    const uint64_t shift_mask = (uint64_t(1) << shift_amount) - 1;

    // Warm Up phase: we don't have enough digits to start writing, so just
    // read them into the accumulator.
    while (accumulator >> shift_amount == 0) {
      uint64_t read_digit = 0;
      // If there are still digits to read, read the next one, else the digit is
      // assumed to be 0.
      if (read_index < this->num_digits) {
        read_digit = this->digits[read_index];
      }
      accumulator = accumulator * 10 + read_digit;
      ++read_index;
    }

    // Shift the decimal point by the number of digits it took to fill the
    // accumulator.
    this->decimal_point -= read_index - 1;

    // Middle phase: we have enough digits to write, as well as more digits to
    // read. Keep reading until we run out of digits.
    while (read_index < this->num_digits) {
      uint64_t read_digit = this->digits[read_index];
      uint64_t write_digit = accumulator >> shift_amount;
      accumulator &= shift_mask;
      this->digits[write_index] = static_cast<uint8_t>(write_digit);
      accumulator = accumulator * 10 + read_digit;
      ++read_index;
      ++write_index;
    }

    // Cool Down phase: All of the readable digits have been read, so just write
    // the remainder, while treating any more digits as 0.
    while (accumulator > 0) {
      uint64_t write_digit = accumulator >> shift_amount;
      accumulator &= shift_mask;
      if (write_index < MAX_NUM_DIGITS) {
        this->digits[write_index] = static_cast<uint8_t>(write_digit);
        ++write_index;
      } else if (write_digit > 0) {
        this->truncated = true;
      }
      accumulator = accumulator * 10;
    }
    this->num_digits = write_index;
    this->trim_trailing_zeroes();
  }

  // Perform a digitwise binary non-rounding left shift on this value by
  // shift_amount. The shift_amount can't be more than MAX_SHIFT_AMOUNT to
  // prevent overflow.
  LIBC_INLINE void left_shift(uint32_t shift_amount) {
    uint32_t new_digits = this->get_num_new_digits(shift_amount);

    int32_t read_index = static_cast<int32_t>(this->num_digits - 1);
    uint32_t write_index = this->num_digits + new_digits;

    uint64_t accumulator = 0;

    // No Warm Up phase. Since we're putting digits in at the top and taking
    // digits from the bottom we don't have to wait for the accumulator to fill.

    // Middle phase: while we have more digits to read, keep reading as well as
    // writing.
    while (read_index >= 0) {
      accumulator += static_cast<uint64_t>(this->digits[read_index])
                     << shift_amount;
      uint64_t next_accumulator = accumulator / 10;
      uint64_t write_digit = accumulator - (10 * next_accumulator);
      --write_index;
      if (write_index < MAX_NUM_DIGITS) {
        this->digits[write_index] = static_cast<uint8_t>(write_digit);
      } else if (write_digit != 0) {
        this->truncated = true;
      }
      accumulator = next_accumulator;
      --read_index;
    }

    // Cool Down phase: there are no more digits to read, so just write the
    // remaining digits in the accumulator.
    while (accumulator > 0) {
      uint64_t next_accumulator = accumulator / 10;
      uint64_t write_digit = accumulator - (10 * next_accumulator);
      --write_index;
      if (write_index < MAX_NUM_DIGITS) {
        this->digits[write_index] = static_cast<uint8_t>(write_digit);
      } else if (write_digit != 0) {
        this->truncated = true;
      }
      accumulator = next_accumulator;
    }

    this->num_digits += new_digits;
    if (this->num_digits > MAX_NUM_DIGITS) {
      this->num_digits = MAX_NUM_DIGITS;
    }
    this->decimal_point += new_digits;
    this->trim_trailing_zeroes();
  }

public:
  // num_string is assumed to be a string of numeric characters. It doesn't
  // handle leading spaces.
  LIBC_INLINE
  HighPrecisionDecimal(
      const char *__restrict num_string,
      const size_t num_len = cpp::numeric_limits<size_t>::max()) {
    bool saw_dot = false;
    size_t num_cur = 0;
    // This counts the digits in the number, even if there isn't space to store
    // them all.
    uint32_t total_digits = 0;
    while (num_cur < num_len &&
           (isdigit(num_string[num_cur]) || num_string[num_cur] == '.')) {
      if (num_string[num_cur] == '.') {
        if (saw_dot) {
          break;
        }
        this->decimal_point = static_cast<int32_t>(total_digits);
        saw_dot = true;
      } else {
        if (num_string[num_cur] == '0' && this->num_digits == 0) {
          --this->decimal_point;
          ++num_cur;
          continue;
        }
        ++total_digits;
        if (this->num_digits < MAX_NUM_DIGITS) {
          this->digits[this->num_digits] = static_cast<uint8_t>(
              internal::b36_char_to_int(num_string[num_cur]));
          ++this->num_digits;
        } else if (num_string[num_cur] != '0') {
          this->truncated = true;
        }
      }
      ++num_cur;
    }

    if (!saw_dot)
      this->decimal_point = static_cast<int32_t>(total_digits);

    if (num_cur < num_len &&
        (num_string[num_cur] == 'e' || num_string[num_cur] == 'E')) {
      ++num_cur;
      if (isdigit(num_string[num_cur]) || num_string[num_cur] == '+' ||
          num_string[num_cur] == '-') {
        auto result =
            strtointeger<int32_t>(num_string + num_cur, 10, num_len - num_cur);
        if (result.has_error()) {
          // TODO: handle error
        }
        int32_t add_to_exponent = result.value;

        // Here we do this operation as int64 to avoid overflow.
        int64_t temp_exponent = static_cast<int64_t>(this->decimal_point) +
                                static_cast<int64_t>(add_to_exponent);

        // Theoretically these numbers should be MAX_BIASED_EXPONENT for long
        // double, but that should be ~16,000 which is much less than 1 << 30.
        if (temp_exponent > (1 << 30)) {
          temp_exponent = (1 << 30);
        } else if (temp_exponent < -(1 << 30)) {
          temp_exponent = -(1 << 30);
        }
        this->decimal_point = static_cast<int32_t>(temp_exponent);
      }
    }

    this->trim_trailing_zeroes();
  }

  // Binary shift left (shift_amount > 0) or right (shift_amount < 0)
  LIBC_INLINE void shift(int shift_amount) {
    if (shift_amount == 0) {
      return;
    }
    // Left
    else if (shift_amount > 0) {
      while (static_cast<uint32_t>(shift_amount) > MAX_SHIFT_AMOUNT) {
        this->left_shift(MAX_SHIFT_AMOUNT);
        shift_amount -= MAX_SHIFT_AMOUNT;
      }
      this->left_shift(static_cast<uint32_t>(shift_amount));
    }
    // Right
    else {
      while (static_cast<uint32_t>(shift_amount) < -MAX_SHIFT_AMOUNT) {
        this->right_shift(MAX_SHIFT_AMOUNT);
        shift_amount += MAX_SHIFT_AMOUNT;
      }
      this->right_shift(static_cast<uint32_t>(-shift_amount));
    }
  }

  // Round the number represented to the closest value of unsigned int type T.
  // This is done ignoring overflow.
  template <class T>
  LIBC_INLINE T
  round_to_integer_type(RoundDirection round = RoundDirection::Nearest) {
    T result = 0;
    uint32_t cur_digit = 0;

    while (static_cast<int32_t>(cur_digit) < this->decimal_point &&
           cur_digit < this->num_digits) {
      result = result * 10 + (this->digits[cur_digit]);
      ++cur_digit;
    }

    // If there are implicit 0s at the end of the number, include those.
    while (static_cast<int32_t>(cur_digit) < this->decimal_point) {
      result *= 10;
      ++cur_digit;
    }
    return result +
           static_cast<T>(this->should_round_up(this->decimal_point, round));
  }

  // Extra functions for testing.

  LIBC_INLINE uint8_t *get_digits() { return this->digits; }
  LIBC_INLINE uint32_t get_num_digits() { return this->num_digits; }
  LIBC_INLINE int32_t get_decimal_point() { return this->decimal_point; }
  LIBC_INLINE void set_truncated(bool trunc) { this->truncated = trunc; }
};

} // namespace internal
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_HIGH_PRECISION_DECIMAL_H
