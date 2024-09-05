#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      dst[y + x * n] = src[x + y * n];
    }
  }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
  for (int i = 0; i < n; i += blocksize) {
    for (int j = 0; j < n; j += blocksize) {
      for (int bi = i; bi < i + blocksize && bi < n; bi++) {
        for (int bj = j; bj < j + blocksize && bj < n; bj++) {
          dst[bj * n + bi] = src[bi * n + bj];
        }
      }
    }
  }
}
