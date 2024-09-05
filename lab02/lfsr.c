#include "lfsr.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lfsr_calculate(uint16_t *reg) {
  *reg = (*reg >> 1) | (*reg ^ *reg >> 2 ^ *reg >> 3 ^ *reg >> 5) << 15;
}
