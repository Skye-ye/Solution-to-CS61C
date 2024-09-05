/* Include the system headers we need */
#include <stdio.h>
#include <stdlib.h>

/* Include our header */
#include "vector.h"

/* Define what our struct is */
struct vector_t {
  size_t size;
  int *data;
};

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
  fprintf(stderr, "Out of memory.\n");
  exit(1);
}

/* Bad example of how to create a new vector */

/* The reason why this function is bad: This function returns a local variable
 * v. This is a serious error because local variables are deallocated when the
 * function returns, leading to undefined behavior when the caller tries to use
 * the returned pointer. The function allocates memory for data using malloc(),
 * but this memory will be lost because the vector_t structure itself is not
 * dynamically allocated. The function initializes the vector with a size of 1
 * and a single element set to 0. This might not be the desired initial state
 * for all use cases.*/
vector_t *bad_vector_new() {
  /* Create the vector and a pointer to it */
  vector_t *retval, v;
  retval = &v;

  /* Initialize attributes */
  retval->size = 1;
  retval->data = malloc(sizeof(int));
  if (retval->data == NULL) {
    allocation_failed();
  }

  retval->data[0] = 0;
  return retval;
}

/* Another suboptimal way of creating a vector */

/* The reason why this function is bad: This function return a vector_t by value
 * not by pointer, which can by inefficient.*/
vector_t also_bad_vector_new() {
  /* Create the vector */
  vector_t v;

  /* Initialize attributes */
  v.size = 1;
  v.data = malloc(sizeof(int));
  if (v.data == NULL) {
    allocation_failed();
  }
  v.data[0] = 0;
  return v;
}

/* Create a new vector with a size (length) of 1
   and set its single component to zero... the
   RIGHT WAY */
vector_t *vector_new() {
  /* Declare what this function will return */
  vector_t *retval;

  /* First, we need to allocate memory on the heap for the struct */
  retval = (vector_t *)malloc(sizeof(vector_t));

  /* Check our return value to make sure we got memory */
  if (retval == NULL) {
    allocation_failed();
  }

  /* Now we need to initialize our data.
     Since retval->data should be able to dynamically grow,
     what do you need to do? */
  retval->size = 1;
  retval->data = malloc(sizeof(int));

  /* Check the data attribute of our vector to make sure we got memory */
  if (retval->data == NULL) {
    free(retval);  // Why is this line necessary?
    allocation_failed();
  }

  /* Complete the initialization by setting the single component to zero */
  retval->data[0] = 0;

  /* and return... */
  return retval;
}

/* Return the value at the specified location/component "loc" of the vector */
int vector_get(vector_t *v, size_t loc) {
  /* If we are passed a NULL pointer for our vector, complain about it and exit.
   */
  if (v == NULL) {
    fprintf(stderr, "vector_get: passed a NULL vector.\n");
    abort();
  }

  /* If the requested location is higher than we have allocated, return 0.
   * Otherwise, return what is in the passed location.
   */
  if (loc < v->size) {
    return v->data[loc];
  } else {
    return 0;
  }
}

/* Free up the memory allocated for the passed vector.
   Remember, you need to free up ALL the memory that was allocated. */
void vector_delete(vector_t *v) {
  if (v == NULL) {
    fprintf(stderr, "vector_delete: passed a NULL vector.\n");
    abort();
  }
  free(v->data);
  free(v);
}

/* Set a value in the vector. If the extra memory allocation fails, call
   allocation_failed(). */
void vector_set(vector_t *v, size_t loc, int value) {
  /* What do you need to do if the location is greater than the size we have
   * allocated?  Remember that unset locations should contain a value of 0.
   */

  if (loc >= v->size) {
    size_t new_size = v->size;
    while (loc >= new_size) {
      new_size *= 2;
    }

    int *new_data = realloc(v->data, new_size * sizeof(int));
    if (new_data == NULL) {
      allocation_failed();
    }

    for (size_t i = v->size; i < new_size; i++) {
      new_data[i] = 0;
    }

    v->size = new_size;
    v->data = new_data;
  }

  v->data[loc] = value;
}