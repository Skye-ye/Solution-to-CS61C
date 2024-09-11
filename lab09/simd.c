#include "simd.h"

#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128) sum += vals[i];
      if (vals[i + 1] >= 128) sum += vals[i + 1];
      if (vals[i + 2] >= 128) sum += vals[i + 2];
      if (vals[i + 3] >= 128) sum += vals[i + 3];
    }

    // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
    // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
    // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(
      127);  // This is a vector with 127s in it... Why might you need this?
  long long int result = 0;  // This is where you should put your final result!
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    __m128i sum_vec = _mm_setzero_si128();
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      __m128i vals_vec = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i cmp = _mm_cmpgt_epi32(vals_vec, _127);
      __m128i result_vec = _mm_and_si128(vals_vec, cmp);
      sum_vec = _mm_add_epi32(sum_vec, result_vec);
    }

    int tmp_Arr[4];
    _mm_storeu_si128((__m128i *)tmp_Arr, sum_vec);
    result += tmp_Arr[0] + tmp_Arr[1] + tmp_Arr[2] + tmp_Arr[3];

    /* Hint: you'll need a tail case. */
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        result += vals[i];
      }
    }
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    __m128i sum_vec0 = _mm_setzero_si128();
    __m128i sum_vec1 = _mm_setzero_si128();
    __m128i sum_vec2 = _mm_setzero_si128();
    __m128i sum_vec3 = _mm_setzero_si128();

    for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
      __m128i vals_vec0 = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i vals_vec1 = _mm_loadu_si128((__m128i *)(vals + i + 4));
      __m128i vals_vec2 = _mm_loadu_si128((__m128i *)(vals + i + 8));
      __m128i vals_vec3 = _mm_loadu_si128((__m128i *)(vals + i + 12));

      __m128i cmp0 = _mm_cmpgt_epi32(vals_vec0, _127);
      __m128i cmp1 = _mm_cmpgt_epi32(vals_vec1, _127);
      __m128i cmp2 = _mm_cmpgt_epi32(vals_vec2, _127);
      __m128i cmp3 = _mm_cmpgt_epi32(vals_vec3, _127);

      __m128i result_vec0 = _mm_and_si128(vals_vec0, cmp0);
      __m128i result_vec1 = _mm_and_si128(vals_vec1, cmp1);
      __m128i result_vec2 = _mm_and_si128(vals_vec2, cmp2);
      __m128i result_vec3 = _mm_and_si128(vals_vec3, cmp3);

      sum_vec0 = _mm_add_epi32(sum_vec0, result_vec0);
      sum_vec1 = _mm_add_epi32(sum_vec1, result_vec1);
      sum_vec2 = _mm_add_epi32(sum_vec2, result_vec2);
      sum_vec3 = _mm_add_epi32(sum_vec3, result_vec3);
    }

    int tmp_Arr[4];
    _mm_storeu_si128((__m128i *)tmp_Arr, sum_vec0);
    result += tmp_Arr[0] + tmp_Arr[1] + tmp_Arr[2] + tmp_Arr[3];
    _mm_storeu_si128((__m128i *)tmp_Arr, sum_vec1);
    result += tmp_Arr[0] + tmp_Arr[1] + tmp_Arr[2] + tmp_Arr[3];
    _mm_storeu_si128((__m128i *)tmp_Arr, sum_vec2);
    result += tmp_Arr[0] + tmp_Arr[1] + tmp_Arr[2] + tmp_Arr[3];
    _mm_storeu_si128((__m128i *)tmp_Arr, sum_vec3);
    result += tmp_Arr[0] + tmp_Arr[1] + tmp_Arr[2] + tmp_Arr[3];

    for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        result += vals[i];
      }
    }
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}