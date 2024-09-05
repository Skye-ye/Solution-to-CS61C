#include "bit_ops.h"

#include <stdio.h>

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x, unsigned n) { return (x >> n) & 1; }

// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned* x, unsigned n, unsigned v) {
  *x = (*x & ~(1U << n)) | (v << n);
}

// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned* x, unsigned n) { *x = *x ^ (1U << n); }
