/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "imageloader.h"

// Determines what color the cell at the given row/col should be. This should
// not affect Image, and should allocate space for a new Color.
Color evaluateOnePixel(Image *image, int row, int col) {
  Color new_color;
  if (image == NULL || row < 0 || row >= image->rows || col < 0 ||
      col >= image->cols) {
    new_color.R = new_color.G = new_color.B = 0;
    return new_color;
  }

  if (image->image[row][col].B & 1) {
    new_color.R = new_color.G = new_color.B = 255;
  } else {
    new_color.R = new_color.G = new_color.B = 0;
  }
  return new_color;
}

// Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image) {
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
      new_image->image[i][j] = evaluateOnePixel(image, i, j);
    }
  }

  return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with
printf) a new image, where each pixel is black if the LSB of the B channel is 0,
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not
necessarily with .ppm file extension). If the input is not correct, a malloc
fails, or any other error occurs, you should exit with code -1. Otherwise, you
should return from main with code 0. Make sure to free all memory before
returning!
*/
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Error: Invalid number of arguments\n");
    exit(-1);
  }

  Image *image = readData(argv[1]);
  if (image == NULL) {
    fprintf(stderr, "Error: readData failed\n");
    exit(-1);
  }

  Image *new_image = steganography(image);
  if (new_image == NULL) {
    fprintf(stderr, "Error: steganography failed\n");
    exit(-1);
  }

  writeData(new_image);
  freeImage(image);
  freeImage(new_image);
  return 0;
}
