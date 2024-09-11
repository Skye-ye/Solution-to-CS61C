#include "matrix.h"

#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

#define SMALL_THRESHOLD 5000
#define BLOCK_SIZE 64

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
 */

/* Generates a random double between low and high */
double rand_double(double low, double high) {
  double range = (high - low);
  double div = RAND_MAX / range;
  return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
  srand(seed);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->cols; j++) {
      result->data[i * result->cols + j] = rand_double(low, high);
    }
  }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in
 * row-major order.
 */
double get(matrix *mat, int row, int col) {
  return mat->data[row * mat->cols + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
  mat->data[row * mat->cols + col] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data
 * array and initialize all entries to be zeros. `parent` should be set to NULL
 * to indicate that this matrix is not a slice. You should also set `ref_cnt`
 * to 1. You should return -1 if either `rows` or `cols` or both have invalid
 * values. Return -2 if any call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
  // Task 1.2 TODO
  // HINTS: Follow these steps.
  // 1. Check if the dimensions are valid. Return -1 if either dimension is not
  // positive.
  // 2. Allocate space for the new matrix struct. Return -2 if allocating memory
  // failed.
  // 3. Allocate space for the matrix data, initializing all entries to be 0.
  // Return -2 if allocating memory failed.
  // 4. Set the number of rows and columns in the matrix struct according to the
  // arguments provided.
  // 5. Set the `parent` field to NULL, since this matrix was not created from a
  // slice.
  // 6. Set the `ref_cnt` field to 1.
  // 7. Store the address of the allocated matrix struct at the location `mat`
  // is pointing at.
  // 8. Return 0 upon success.
  if (rows <= 0 || cols <= 0) {
    return -1;
  }
  *mat = (matrix *)malloc(sizeof(matrix));
  if (*mat == NULL) {
    return -2;
  }
  (*mat)->data = (double *)calloc(rows * cols, sizeof(double));
  if ((*mat)->data == NULL) {
    free(*mat);
    *mat = NULL;
    return -2;
  }
  (*mat)->rows = rows;
  (*mat)->cols = cols;
  (*mat)->parent = NULL;
  (*mat)->ref_cnt = 1;
  return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice
 * and has no existing slices, or that you free `mat->parent->data` if `mat` is
 * the last existing slice of its parent matrix and its parent matrix has no
 * other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
  // Task 1.3 TODO
  // HINTS: Follow these steps.
  // 1. If the matrix pointer `mat` is NULL, return.
  // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the
  // `ref_cnt` field becomes 0, then free `mat` and its `data` field.
  // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then
  // free `mat`.
  if (mat == NULL) {
    return;
  }
  if (mat->parent == NULL) {
    mat->ref_cnt--;
    if (mat->ref_cnt == 0) {
      free(mat->data);
      free(mat);
    }
  } else {
    deallocate_matrix(mat->parent);
    free(mat);
  }
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and
 * `cols` columns. Its data should point to the `offset`th entry of `from`'s
 * data (you do not need to allocate memory) for the data field. `parent` should
 * be set to `from` to indicate this matrix is a slice of `from` and the
 * reference counter for `from` should be incremented. Lastly, do not forget to
 * set the matrix's row and column values as well. You should return -1 if
 * either `rows` or `cols` or both have invalid values. Return -2 if any call to
 * allocate memory in this function fails. Return 0 upon success. NOTE: Here
 * we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows,
                        int cols) {
  // Task 1.4 TODO
  // HINTS: Follow these steps.
  // 1. Check if the dimensions are valid. Return -1 if either dimension is not
  // positive.
  // 2. Allocate space for the new matrix struct. Return -2 if allocating memory
  // failed.
  // 3. Set the `data` field of the new struct to be the `data` field of the
  // `from` struct plus `offset`.
  // 4. Set the number of rows and columns in the new struct according to the
  // arguments provided.
  // 5. Set the `parent` field of the new struct to the `from` struct pointer.
  // 6. Increment the `ref_cnt` field of the `from` struct by 1.
  // 7. Store the address of the allocated matrix struct at the location `mat`
  // is pointing at.
  // 8. Return 0 upon success.
  if (rows <= 0 || cols <= 0) {
    return -1;
  }
  *mat = (matrix *)malloc(sizeof(matrix));
  if (*mat == NULL) {
    return -2;
  }
  (*mat)->data = from->data + offset;
  (*mat)->rows = rows;
  (*mat)->cols = cols;
  (*mat)->parent = from;
  from->ref_cnt++;
  return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
  // Task 1.5 TODO
  int size = mat->rows * mat->cols;
  __m256d val_vec = _mm256_set1_pd(val);

#pragma omp parallel
  {
#pragma omp for nowait
    for (int i = 0; i < size / 16 * 16; i += 16) {
      _mm256_storeu_pd(mat->data + i, val_vec);
      _mm256_storeu_pd(mat->data + i + 4, val_vec);
      _mm256_storeu_pd(mat->data + i + 8, val_vec);
      _mm256_storeu_pd(mat->data + i + 12, val_vec);
    }

    for (int i = size / 16 * 16; i < size; i++) {
      mat->data[i] = val;
    }
  }
}

// Fast absolute value
static inline double fast_abs(double x) {
  union double_bits u = {x};
  u.i &= 0x7FFFFFFFFFFFFFFFULL;
  return u.d;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
  // Task 1.5 TODO
  int size = mat->rows * mat->cols;
  __m256d sign_mask = _mm256_set1_pd(-0.0);
  if (size <= SMALL_THRESHOLD) {
    for (int i = 0; i < size; i++) {
      result->data[i] = fast_abs(mat->data[i]);
    }
  } else {
    // Check if data is aligned to 32-byte boundary
    int aligned = (((uintptr_t)mat->data | (uintptr_t)result->data) & 31) == 0;

#pragma omp parallel
    {
#pragma omp for nowait
      for (int i = 0; i < size / 16 * 16; i += 16) {
        // Prefetch next chunk of data
        _mm_prefetch((char *)&mat->data[i + 64], _MM_HINT_T0);

        __m256d data_vec1, data_vec2, data_vec3, data_vec4;
        __m256d abs_vec1, abs_vec2, abs_vec3, abs_vec4;

        if (aligned) {
          data_vec1 = _mm256_load_pd(mat->data + i);
          data_vec2 = _mm256_load_pd(mat->data + i + 4);
          data_vec3 = _mm256_load_pd(mat->data + i + 8);
          data_vec4 = _mm256_load_pd(mat->data + i + 12);
        } else {
          data_vec1 = _mm256_loadu_pd(mat->data + i);
          data_vec2 = _mm256_loadu_pd(mat->data + i + 4);
          data_vec3 = _mm256_loadu_pd(mat->data + i + 8);
          data_vec4 = _mm256_loadu_pd(mat->data + i + 12);
        }

        abs_vec1 = _mm256_andnot_pd(sign_mask, data_vec1);
        abs_vec2 = _mm256_andnot_pd(sign_mask, data_vec2);
        abs_vec3 = _mm256_andnot_pd(sign_mask, data_vec3);
        abs_vec4 = _mm256_andnot_pd(sign_mask, data_vec4);

        if (aligned) {
          _mm256_store_pd(result->data + i, abs_vec1);
          _mm256_store_pd(result->data + i + 4, abs_vec2);
          _mm256_store_pd(result->data + i + 8, abs_vec3);
          _mm256_store_pd(result->data + i + 12, abs_vec4);
        } else {
          _mm256_storeu_pd(result->data + i, abs_vec1);
          _mm256_storeu_pd(result->data + i + 4, abs_vec2);
          _mm256_storeu_pd(result->data + i + 8, abs_vec3);
          _mm256_storeu_pd(result->data + i + 12, abs_vec4);
        }
      }

#pragma omp single
      {
        for (int i = size / 16 * 16; i < size; i++) {
          result->data[i] = fast_abs(mat->data[i]);
        }
      }
    }
  }

  return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
  // Task 1.5 TODO
  int size = mat->rows * mat->cols;
  __m256d sign_mask = _mm256_set1_pd(-0.0);

  if (size <= SMALL_THRESHOLD) {
    // Small matrices: Simple loop without SIMD or OpenMP
    for (int i = 0; i < size; i++) {
      result->data[i] = -mat->data[i];
    }
  } else {
    int aligned = (((uintptr_t)mat->data | (uintptr_t)result->data) & 31) == 0;

#pragma omp parallel
    {
#pragma omp for nowait
      for (int i = 0; i < size / 16 * 16; i += 16) {
        // Prefetch next chunk of data
        _mm_prefetch((char *)&mat->data[i + 64], _MM_HINT_T0);

        __m256d data_vec1, data_vec2, data_vec3, data_vec4;
        __m256d neg_vec1, neg_vec2, neg_vec3, neg_vec4;

        if (aligned) {
          data_vec1 = _mm256_load_pd(mat->data + i);
          data_vec2 = _mm256_load_pd(mat->data + i + 4);
          data_vec3 = _mm256_load_pd(mat->data + i + 8);
          data_vec4 = _mm256_load_pd(mat->data + i + 12);
        } else {
          data_vec1 = _mm256_loadu_pd(mat->data + i);
          data_vec2 = _mm256_loadu_pd(mat->data + i + 4);
          data_vec3 = _mm256_loadu_pd(mat->data + i + 8);
          data_vec4 = _mm256_loadu_pd(mat->data + i + 12);
        }

        neg_vec1 = _mm256_xor_pd(sign_mask, data_vec1);
        neg_vec2 = _mm256_xor_pd(sign_mask, data_vec2);
        neg_vec3 = _mm256_xor_pd(sign_mask, data_vec3);
        neg_vec4 = _mm256_xor_pd(sign_mask, data_vec4);

        if (aligned) {
          _mm256_store_pd(result->data + i, neg_vec1);
          _mm256_store_pd(result->data + i + 4, neg_vec2);
          _mm256_store_pd(result->data + i + 8, neg_vec3);
          _mm256_store_pd(result->data + i + 12, neg_vec4);
        } else {
          _mm256_storeu_pd(result->data + i, neg_vec1);
          _mm256_storeu_pd(result->data + i + 4, neg_vec2);
          _mm256_storeu_pd(result->data + i + 8, neg_vec3);
          _mm256_storeu_pd(result->data + i + 12, neg_vec4);
        }
      }

#pragma omp single
      {
        for (int i = size / 16 * 16; i < size; i++) {
          result->data[i] = -mat->data[i];
        }
      }
    }
  }

  return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  int size = mat1->rows * mat1->cols;

  if (size <= SMALL_THRESHOLD) {
    // Small matrices: Simple loop without SIMD or OpenMP
    for (int i = 0; i < size; i++) {
      result->data[i] = mat1->data[i] + mat2->data[i];
    }
  } else {
    int aligned = (((uintptr_t)mat1->data | (uintptr_t)mat2->data |
                    (uintptr_t)result->data) &
                   31) == 0;

#pragma omp parallel
    {
#pragma omp for nowait
      for (int i = 0; i < size / 16 * 16; i += 16) {
        // Prefetch next chunk of data
        _mm_prefetch((char *)&mat1->data[i + 64], _MM_HINT_T0);
        _mm_prefetch((char *)&mat2->data[i + 64], _MM_HINT_T0);

        __m256d data1_vec1, data1_vec2, data1_vec3, data1_vec4;
        __m256d data2_vec1, data2_vec2, data2_vec3, data2_vec4;
        __m256d sum_vec1, sum_vec2, sum_vec3, sum_vec4;

        if (aligned) {
          data1_vec1 = _mm256_load_pd(mat1->data + i);
          data1_vec2 = _mm256_load_pd(mat1->data + i + 4);
          data1_vec3 = _mm256_load_pd(mat1->data + i + 8);
          data1_vec4 = _mm256_load_pd(mat1->data + i + 12);

          data2_vec1 = _mm256_load_pd(mat2->data + i);
          data2_vec2 = _mm256_load_pd(mat2->data + i + 4);
          data2_vec3 = _mm256_load_pd(mat2->data + i + 8);
          data2_vec4 = _mm256_load_pd(mat2->data + i + 12);
        } else {
          data1_vec1 = _mm256_loadu_pd(mat1->data + i);
          data1_vec2 = _mm256_loadu_pd(mat1->data + i + 4);
          data1_vec3 = _mm256_loadu_pd(mat1->data + i + 8);
          data1_vec4 = _mm256_loadu_pd(mat1->data + i + 12);

          data2_vec1 = _mm256_loadu_pd(mat2->data + i);
          data2_vec2 = _mm256_loadu_pd(mat2->data + i + 4);
          data2_vec3 = _mm256_loadu_pd(mat2->data + i + 8);
          data2_vec4 = _mm256_loadu_pd(mat2->data + i + 12);
        }

        sum_vec1 = _mm256_add_pd(data1_vec1, data2_vec1);
        sum_vec2 = _mm256_add_pd(data1_vec2, data2_vec2);
        sum_vec3 = _mm256_add_pd(data1_vec3, data2_vec3);
        sum_vec4 = _mm256_add_pd(data1_vec4, data2_vec4);

        if (aligned) {
          _mm256_store_pd(result->data + i, sum_vec1);
          _mm256_store_pd(result->data + i + 4, sum_vec2);
          _mm256_store_pd(result->data + i + 8, sum_vec3);
          _mm256_store_pd(result->data + i + 12, sum_vec4);
        } else {
          _mm256_storeu_pd(result->data + i, sum_vec1);
          _mm256_storeu_pd(result->data + i + 4, sum_vec2);
          _mm256_storeu_pd(result->data + i + 8, sum_vec3);
          _mm256_storeu_pd(result->data + i + 12, sum_vec4);
        }
      }

#pragma omp single
      {
        // Handle remaining elements
        for (int i = size / 16 * 16; i < size; i++) {
          result->data[i] = mat1->data[i] + mat2->data[i];
        }
      }
    }
  }

  return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  if (result->rows != mat1->rows || result->cols != mat1->cols ||
      result->rows != mat2->rows || result->cols != mat2->cols) {
    return -1;
  }

  int size = mat1->rows * mat1->cols;

  if (size <= SMALL_THRESHOLD) {
    // Small matrices: Simple loop without SIMD or OpenMP
    for (int i = 0; i < size; i++) {
      result->data[i] = mat1->data[i] - mat2->data[i];
    }
  } else {
    int aligned = (((uintptr_t)mat1->data | (uintptr_t)mat2->data |
                    (uintptr_t)result->data) &
                   31) == 0;

#pragma omp parallel
    {
#pragma omp for nowait
      for (int i = 0; i < size / 16 * 16; i += 16) {
        // Prefetch next chunk of data
        _mm_prefetch((char *)&mat1->data[i + 64], _MM_HINT_T0);
        _mm_prefetch((char *)&mat2->data[i + 64], _MM_HINT_T0);

        __m256d data1_vec1, data1_vec2, data1_vec3, data1_vec4;
        __m256d data2_vec1, data2_vec2, data2_vec3, data2_vec4;
        __m256d diff_vec1, diff_vec2, diff_vec3, diff_vec4;

        if (aligned) {
          data1_vec1 = _mm256_load_pd(mat1->data + i);
          data1_vec2 = _mm256_load_pd(mat1->data + i + 4);
          data1_vec3 = _mm256_load_pd(mat1->data + i + 8);
          data1_vec4 = _mm256_load_pd(mat1->data + i + 12);

          data2_vec1 = _mm256_load_pd(mat2->data + i);
          data2_vec2 = _mm256_load_pd(mat2->data + i + 4);
          data2_vec3 = _mm256_load_pd(mat2->data + i + 8);
          data2_vec4 = _mm256_load_pd(mat2->data + i + 12);
        } else {
          data1_vec1 = _mm256_loadu_pd(mat1->data + i);
          data1_vec2 = _mm256_loadu_pd(mat1->data + i + 4);
          data1_vec3 = _mm256_loadu_pd(mat1->data + i + 8);
          data1_vec4 = _mm256_loadu_pd(mat1->data + i + 12);

          data2_vec1 = _mm256_loadu_pd(mat2->data + i);
          data2_vec2 = _mm256_loadu_pd(mat2->data + i + 4);
          data2_vec3 = _mm256_loadu_pd(mat2->data + i + 8);
          data2_vec4 = _mm256_loadu_pd(mat2->data + i + 12);
        }

        diff_vec1 = _mm256_sub_pd(data1_vec1, data2_vec1);
        diff_vec2 = _mm256_sub_pd(data1_vec2, data2_vec2);
        diff_vec3 = _mm256_sub_pd(data1_vec3, data2_vec3);
        diff_vec4 = _mm256_sub_pd(data1_vec4, data2_vec4);

        if (aligned) {
          _mm256_store_pd(result->data + i, diff_vec1);
          _mm256_store_pd(result->data + i + 4, diff_vec2);
          _mm256_store_pd(result->data + i + 8, diff_vec3);
          _mm256_store_pd(result->data + i + 12, diff_vec4);
        } else {
          _mm256_storeu_pd(result->data + i, diff_vec1);
          _mm256_storeu_pd(result->data + i + 4, diff_vec2);
          _mm256_storeu_pd(result->data + i + 8, diff_vec3);
          _mm256_storeu_pd(result->data + i + 12, diff_vec4);
        }
      }

#pragma omp single
      {
        // Handle remaining elements
        for (int i = size / 16 * 16; i < size; i++) {
          result->data[i] = mat1->data[i] - mat2->data[i];
        }
      }
    }
  }

  return 0;
}

static inline double _mm256_reduce_add_pd(__m256d v) {
  __m128d vlow = _mm256_castpd256_pd128(v);
  __m128d vhigh = _mm256_extractf128_pd(v, 1);
  vlow = _mm_add_pd(vlow, vhigh);
  __m128d high64 = _mm_unpackhi_pd(vlow, vlow);
  return _mm_cvtsd_f64(_mm_add_sd(vlow, high64));
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual
 * elements. You may assume `mat1`'s number of columns is equal to `mat2`'s
 * number of rows. Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  // Task 1.6 TODO
  int rows = mat1->rows;
  int cols = mat2->cols;
  int times = mat1->cols;

  int size = rows * cols * times;

  if (size <= SMALL_THRESHOLD) {
// Small matrices: Simple loop without SIMD or OpenMP
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        double sum = 0.0;
        for (int k = 0; k < times; k++) {
          sum += mat1->data[i * times + k] * mat2->data[k * cols + j];
        }
        result->data[i * cols + j] = sum;
      }
    }
  } else {
    // Transpose mat2
    matrix *mat2_transpose = transpose(mat2);
    fill_matrix(result, 0.0);

    // Main case
#pragma omp parallel
    {
      __m256d sum_vec1, sum_vec2, sum_vec3, sum_vec4;
      __m256d data1_vec1, data1_vec2, data1_vec3, data1_vec4;
      __m256d data2_vec1, data2_vec2, data2_vec3, data2_vec4;

#pragma omp for collapse(3) schedule(guided) nowait
      for (int i = 0; i < rows; i += BLOCK_SIZE)
        for (int j = 0; j < cols; j += BLOCK_SIZE)
          for (int k = 0; k < times; k += BLOCK_SIZE) {
            int max_i = (i + BLOCK_SIZE > rows) ? rows : i + BLOCK_SIZE;
            int max_j = (j + BLOCK_SIZE > cols) ? cols : j + BLOCK_SIZE;
            int max_k = (k + BLOCK_SIZE > times) ? times : k + BLOCK_SIZE;

            for (int ii = i; ii < max_i; ii++) {
              for (int jj = j; jj < max_j; jj++) {
                sum_vec1 = _mm256_setzero_pd();
                sum_vec2 = _mm256_setzero_pd();
                sum_vec3 = _mm256_setzero_pd();
                sum_vec4 = _mm256_setzero_pd();

                for (int kk = k; kk < max_k / 16 * 16; kk += 16) {
                  int mat1_offset = ii * times + kk;
                  int mat2_offset = jj * times + kk;
                  // Prefetch next chunks
                  _mm_prefetch((char *)mat1->data + mat1_offset + 64,
                               _MM_HINT_T0);
                  _mm_prefetch((char *)mat2_transpose->data + mat2_offset + 64,
                               _MM_HINT_T0);

                  data1_vec1 = _mm256_loadu_pd(mat1->data + mat1_offset);
                  data1_vec2 = _mm256_loadu_pd(mat1->data + mat1_offset + 4);
                  data1_vec3 = _mm256_loadu_pd(mat1->data + mat1_offset + 8);
                  data1_vec4 = _mm256_loadu_pd(mat1->data + mat1_offset + 12);

                  data2_vec1 =
                      _mm256_loadu_pd(mat2_transpose->data + mat2_offset);
                  data2_vec2 =
                      _mm256_loadu_pd(mat2_transpose->data + mat2_offset + 4);
                  data2_vec3 =
                      _mm256_loadu_pd(mat2_transpose->data + mat2_offset + 8);
                  data2_vec4 =
                      _mm256_loadu_pd(mat2_transpose->data + mat2_offset + 12);

                  sum_vec1 = _mm256_fmadd_pd(data1_vec1, data2_vec1, sum_vec1);
                  sum_vec2 = _mm256_fmadd_pd(data1_vec2, data2_vec2, sum_vec2);
                  sum_vec3 = _mm256_fmadd_pd(data1_vec3, data2_vec3, sum_vec3);
                  sum_vec4 = _mm256_fmadd_pd(data1_vec4, data2_vec4, sum_vec4);
                }

                sum_vec1 = _mm256_add_pd(sum_vec1, sum_vec2);
                sum_vec3 = _mm256_add_pd(sum_vec3, sum_vec4);
                sum_vec1 = _mm256_add_pd(sum_vec1, sum_vec3);

                double sum = _mm256_reduce_add_pd(sum_vec1);

                // Handle tail
                for (int kk = max_k / 16 * 16; kk < max_k; kk++) {
                  sum += mat1->data[ii * times + kk] *
                         mat2_transpose->data[jj * times + kk];
                }

                result->data[ii * cols + jj] += sum;
              }
            }
          }
    }

    deallocate_matrix(mat2_transpose);
  }

  return 0;
}

matrix *transpose(matrix *mat) {
  matrix *result = NULL;
  if (allocate_matrix(&result, mat->cols, mat->rows) != 0) {
    return NULL;
  }

  int width = mat->cols;
  int height = mat->rows;

  // For small matrices, use the naive method
  if (width * height <= 1024) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        result->data[j * height + i] = mat->data[i * width + j];
      }
    }
  } else {
// For larger matrices, use blocking and parallel processing
#pragma omp parallel for collapse(2)
    for (int i = 0; i < height; i += BLOCK_SIZE) {
      for (int j = 0; j < width; j += BLOCK_SIZE) {
        int block_height =
            (i + BLOCK_SIZE > height) ? (height - i) : BLOCK_SIZE;
        int block_width = (j + BLOCK_SIZE > width) ? (width - j) : BLOCK_SIZE;

        // Transpose the block
        for (int bi = 0; bi < block_height; bi += 4) {
          for (int bj = 0; bj < block_width; bj += 4) {
            if (bi + 4 <= block_height && bj + 4 <= block_width) {
              int ibi = i + bi;
              int jbj = j + bj;
              // Load 4x4 block
              __m256d row0 = _mm256_loadu_pd(mat->data + ibi * width + jbj);
              __m256d row1 =
                  _mm256_loadu_pd(mat->data + (ibi + 1) * width + jbj);
              __m256d row2 =
                  _mm256_loadu_pd(mat->data + (ibi + 2) * width + jbj);
              __m256d row3 =
                  _mm256_loadu_pd(mat->data + (ibi + 3) * width + jbj);

              // Transpose 4x4 block
              __m256d tmp0 = _mm256_unpacklo_pd(row0, row1);
              __m256d tmp1 = _mm256_unpackhi_pd(row0, row1);
              __m256d tmp2 = _mm256_unpacklo_pd(row2, row3);
              __m256d tmp3 = _mm256_unpackhi_pd(row2, row3);

              row0 = _mm256_permute2f128_pd(tmp0, tmp2, 0x20);
              row1 = _mm256_permute2f128_pd(tmp1, tmp3, 0x20);
              row2 = _mm256_permute2f128_pd(tmp0, tmp2, 0x31);
              row3 = _mm256_permute2f128_pd(tmp1, tmp3, 0x31);

              // Store transposed 4x4 block
              _mm256_storeu_pd(result->data + jbj * height + ibi, row0);
              _mm256_storeu_pd(result->data + (jbj + 1) * height + ibi, row1);
              _mm256_storeu_pd(result->data + (jbj + 2) * height + ibi, row2);
              _mm256_storeu_pd(result->data + (jbj + 3) * height + ibi, row3);
            } else {
              // Tail case
              for (int ii = 0; ii < 4 && bi + ii < block_height; ii++) {
                for (int jj = 0; jj < 4 && bj + jj < block_width; jj++) {
                  result->data[(j + bj + jj) * height + (i + bi + ii)] =
                      mat->data[(i + bi + ii) * width + (j + bj + jj)];
                }
              }
            }
          }
        }
      }
    }
  }

  return result;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise
 * multiplication. You may assume `mat` is a square matrix and `pow` is a
 * non-negative integer. Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
  int width = mat->rows;

  // Handle base cases
  if (pow == 0 || pow == 1) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < width; j++) {
        result->data[i * width + j] = (pow == 0 && i == j) ? 1.0
                                      : (pow == 1) ? mat->data[i * width + j]
                                                   : 0.0;
      }
    }
    return 0;
  }

  // Allocate temporary matrix
  matrix *temp = NULL;
  if (allocate_matrix(&temp, width, width) != 0) {
    return -2;
  }

// Initialize result to mat
#pragma omp parallel for
  for (int i = 0; i < width * width; i++) {
    result->data[i] = mat->data[i];
  }

  // Repeated squaring algorithm
  for (int p = pow - 1; p > 0; p >>= 1) {
    if (p & 1) {
      mul_matrix(temp, result, mat);
      // Swap result and temp
      double *temp_data = result->data;
      result->data = temp->data;
      temp->data = temp_data;
    }
    if (p > 1) {
      mul_matrix(temp, mat, mat);
      // Swap mat and temp
      double *temp_data = mat->data;
      mat->data = temp->data;
      temp->data = temp_data;
    }
  }

  deallocate_matrix(temp);
  return 0;
}