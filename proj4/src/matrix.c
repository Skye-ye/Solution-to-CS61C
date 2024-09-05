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
      set(result, i, j, rand_double(low, high));
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
  for (int i = 0; i < size; i++) {
    mat->data[i] = val;
  }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
  // Task 1.5 TODO
  int size = mat->rows * mat->cols;
  for (int i = 0; i < size; i++) {
    result->data[i] = mat->data[i] < 0 ? -mat->data[i] : mat->data[i];
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
  for (int i = 0; i < size; i++) {
    result->data[i] = -mat->data[i];
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
  // Task 1.5 TODO
  int size = mat1->rows * mat1->cols;
  for (int i = 0; i < size; i++) {
    result->data[i] = mat1->data[i] + mat2->data[i];
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
  // Task 1.5 TODO
  int size = mat1->rows * mat1->cols;
  for (int i = 0; i < size; i++) {
    result->data[i] = mat1->data[i] - mat2->data[i];
  }
  return 0;
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
  double *data1 = mat1->data;
  double *data2 = mat2->data;
  double *data3 = result->data;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double sum = 0;
      for (int k = 0; k < times; k++) {
        sum += data1[i * times + k] * data2[k * cols + j];
      }
      data3[i * cols + j] = sum;
    }
  }
  return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise
 * multiplication. You may assume `mat` is a square matrix and `pow` is a
 * non-negative integer. Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
  // Task 1.6 TODO
  int width = mat->rows;

  if (pow == 0) {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < width; j++) {
        if (i == j) {
          set(result, i, j, 1.0);
        } else {
          set(result, i, j, 0.0);
        }
      }
    }
    return 0;
  }

  matrix *temp = NULL;
  if (allocate_matrix(&temp, width, width) != 0) {
    return -2;
  }

  for (int i = 0; i < width * width; i++) {
    result->data[i] = mat->data[i];
  }

  for (int p = 1; p < pow; p++) {
    mul_matrix(temp, result, mat);
    double *temp_data = result->data;
    result->data = temp->data;
    temp->data = temp_data;
  }

  deallocate_matrix(temp);
  return 0;
}
