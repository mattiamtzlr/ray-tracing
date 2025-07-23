#include "color.h"

void write_color(FILE *out, color_t color) {
    double r = color.x;
    double g = color.y;
    double b = color.z;

    int r_byte = (int) 255.999 * r;
    int g_byte = (int) 255.999 * g;
    int b_byte = (int) 255.999 * b;

    fprintf(out, "%d %d %d\n", r_byte, g_byte, b_byte);
}
