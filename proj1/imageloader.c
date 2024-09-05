/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include "imageloader.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: %s could not open\n", filename);
    return NULL;
  }

  char format[20];
  uint32_t width, height, maxColor;
  Image *image = malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory for image\n");
    fclose(fp);
    return NULL;
  }

  if (fscanf(fp, "%s", format) != 1 ||
      fscanf(fp, "%d %d", &width, &height) != 2 ||
      fscanf(fp, "%d", &maxColor) != 1) {
    fprintf(stderr, "Error: Invalid file format\n");
    free(image);
    fclose(fp);
    return NULL;
  }

  image->rows = height;
  image->cols = width;
  image->image = (Color **)malloc(sizeof(Color *) * height);
  if (image->image == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory for image rows\n");
    free(image);
    fclose(fp);
    return NULL;
  }

  for (uint32_t i = 0; i < height; i++) {
    image->image[i] = (Color *)malloc(sizeof(Color) * width);
    if (image->image[i] == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for image row %d\n", i);
      for (uint32_t j = 0; j < i; j++) {
        free(image->image[j]);
      }
      free(image->image);
      free(image);
      fclose(fp);
      return NULL;
    }
  }

  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      if (fscanf(fp, "%hhu %hhu %hhu", &(image->image[i][j].R),
                 &(image->image[i][j].G), &(image->image[i][j].B)) != 3) {
        fprintf(stderr, "Error: Invalid color data at pixel (%d, %d)\n", i, j);
        freeImage(image);
        fclose(fp);
        return NULL;
      }
    }
  }

  fclose(fp);
  return image;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the
// image's data.
void writeData(Image *image) {
  if (image == NULL) return;

  char *format = "P3";
  uint32_t width = image->cols;
  uint32_t height = image->rows;
  uint32_t maxColor = 255;

  printf("%s\n", format);
  printf("%d %d\n", width, height);
  printf("%d\n", maxColor);

  for (uint32_t i = 0; i < height; i++) {
    for (uint32_t j = 0; j < width; j++) {
      printf("%3hhu %3hhu %3hhu", image->image[i][j].R, image->image[i][j].G,
             image->image[i][j].B);
      if (j != width - 1) printf("   ");
    }
    printf("\n");
  }
}

// Frees an image
void freeImage(Image *image) {
  if (image == NULL) return;
  if (image->image != NULL) {
    for (uint32_t i = 0; i < image->rows; i++) {
      if (image->image[i] != NULL) {
        free(image->image[i]);
      }
    }
    free(image->image);
  }
  free(image);
}