#pragma once

#include "vec3.h"
#include <stdio.h>

/* color type, alias of vector */
typedef vec3_t color_t;

/* writes `color` in 'r g b\n' representation to `out` */
void write_color(FILE *out, color_t color);
