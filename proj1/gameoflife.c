/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "imageloader.h"

// #define BLACK_AND_WITE

// Determines what color the cell at the given row/col should be. This function
// allocates space for a new Color. Note that you will need to read the eight
// neighbors of the cell in question. The grid "wraps", so we treat the top row
// as adjacent to the bottom row and the left column as adjacent to the right
// column.

#ifdef BLACK_AND_WITE
int isAlive(Image *image, int row, int col) {
  if (image == NULL || row < 0 || row >= image->rows || col < 0 ||
      col >= image->cols) {
    return 0;
  }
  return image->image[row][col].R == 255;
}

Color evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
  static int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
  static int dy[] = {1, 0, -1, 1, -1, 1, 0, -1};
  Color new_color;
  if (image == NULL || row < 0 || row >= image->rows || col < 0 ||
      col >= image->cols) {
    new_color.R = new_color.G = new_color.B = 0;
    return new_color;
  }

  int cell_count = 0;
  for (int i = 0; i < 8; i++) {
    int new_row = (row + dx[i] + image->rows) % image->rows;
    int new_col = (col + dy[i] + image->cols) % image->cols;
    if (isAlive(image, new_row, new_col)) {
      cell_count++;
    }
  }

  if (isAlive(image, row, col)) {
    if (rule & (1 << (cell_count + 9))) {
      new_color.R = new_color.G = new_color.B = 255;
    } else {
      new_color.R = new_color.G = new_color.B = 0;
    }
  } else {
    if (rule & (1 << cell_count)) {
      new_color.R = new_color.G = new_color.B = 255;
    } else {
      new_color.R = new_color.G = new_color.B = 0;
    }
  }

  return new_color;
}

#else
#include <stdint.h>

uint8_t evolveChannel(Image *image, int row, int col,
                      uint8_t (*getChannel)(Color *), uint32_t rule) {
  static const int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
  static const int dy[] = {1, 0, -1, 1, -1, 1, 0, -1};
  uint8_t new_state = 0;
  uint8_t current = getChannel(&image->image[row][col]);

  for (int bit = 0; bit < 8; bit++) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
      int new_row = (row + dx[i] + image->rows) % image->rows;
      int new_col = (col + dy[i] + image->cols) % image->cols;
      count += (getChannel(&image->image[new_row][new_col]) >> bit) & 1;
    }

    if (current & (1 << bit)) {
      if (rule & (1 << (count + 9))) {
        new_state |= (1 << bit);
      }
    } else {
      if (rule & (1 << count)) {
        new_state |= (1 << bit);
      }
    }
  }

  return new_state;
}

uint8_t getR(Color *c) { return c->R; }
uint8_t getG(Color *c) { return c->G; }
uint8_t getB(Color *c) { return c->B; }

Color evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
  Color new_color;

  if (image == NULL || row < 0 || row >= image->rows || col < 0 ||
      col >= image->cols) {
    new_color.R = new_color.G = new_color.B = 0;
    return new_color;
  }

  new_color.R = evolveChannel(image, row, col, getR, rule);
  new_color.G = evolveChannel(image, row, col, getG, rule);
  new_color.B = evolveChannel(image, row, col, getB, rule);

  return new_color;
}
#endif

// The main body of Life; given an image and a rule, computes one iteration of
// the Game of Life. You should be able to copy most of this from
// steganography.c
Image *life(Image *image, uint32_t rule) {
  if (image == NULL) {
    return NULL;
  }

  Image *new_image = malloc(sizeof(Image));
  if (new_image == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    return NULL;
  }

  new_image->rows = image->rows;
  new_image->cols = image->cols;

  new_image->image = (Color **)malloc(sizeof(Color *) * image->rows);
  if (new_image->image == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    free(new_image);
    return NULL;
  }

  for (uint32_t i = 0; i < image->rows; i++) {
    new_image->image[i] = (Color *)malloc(sizeof(Color) * image->cols);
    if (new_image->image[i] == NULL) {
      fprintf(stderr, "Error: malloc failed\n");
      for (uint32_t j = 0; j < i; j++) {
        free(new_image->image[j]);
      }
      free(new_image->image);
      free(new_image);
      return NULL;
    }
  }

  for (uint32_t i = 0; i < image->rows; i++) {
    for (uint32_t j = 0; j < image->cols; j++) {
      new_image->image[i][j] = evaluateOneCell(image, i, j, rule);
    }
  }

  return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then
prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this
will be a string. You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you
should exit with code -1. Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv) {
  if (argc != 3) {
    printf(
        "usage :./ gameOfLife filename rule\nfilename is an ASCII PPM "
        "file (type P3) with maximum value 255.\nrule is a hex number "
        "beginning with 0x; Life is 0x1808.\n");
    exit(-1);
  }

  Image *image = readData(argv[1]);
  if (image == NULL) {
    fprintf(stderr, "Error: readData failed\n");
    exit(-1);
  }

  char *endptr;
  uint32_t rule = strtol(argv[2], &endptr, 16);
  if (*endptr != '\0') {
    fprintf(stderr, "Error: Invalid rule\n");
    exit(-1);
  }

  Image *new_image = life(image, rule);
  if (new_image == NULL) {
    fprintf(stderr, "Error: life failed\n");
    exit(-1);
  }

  writeData(new_image);

  freeImage(image);
  freeImage(new_image);

  return 0;
}
