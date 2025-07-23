#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

#define OUTPUT "output.png"

int main(void) {
    const unsigned int image_width = 256;
    const unsigned int image_height = 256;
    /* three color channels */
    const size_t image_size = image_width * image_height * 3;

    unsigned char *image = malloc(image_size);
    if (image == NULL) {
        fputs("Memory allocation of image failed", stderr);
        return 1;
    }

    /* write left-to-right, top-to-bottom */
    for (unsigned int row = 0; row < image_height; row++) {
        /* progress indicator */
        printf("\rScanlines remaining: %u", image_height - row);
        fflush(stdout);

        for (unsigned int col = 0; col < image_width; col++) {
            /* real values, ranging from 0.0 to 1.0 */
            double r = (double)col / (image_width - 1);
            double g = (double)row / (image_height - 1);
            double b = 0 * (double)(image_width - col) / (image_width - 1);

            /* scaled values, ranging from 0 to 255, for writing */
            unsigned char ir = (unsigned char)(255.999 * r);
            unsigned char ig = (unsigned char)(255.999 * g);
            unsigned char ib = (unsigned char)(255.999 * b);

            /* current index in image */
            size_t img_index = 3 * (row * image_width + col);
            image[img_index + 0] = ir;
            image[img_index + 1] = ig;
            image[img_index + 2] = ib;
        }
    }

    /* some error handling */
    unsigned int error =
        lodepng_encode24_file(OUTPUT, image, image_width, image_height);

    if (error)
        fprintf(stderr, "\rPNG encoding error %u: %s\n", error, lodepng_error_text(error));
    else
        printf("\rSuccessfully wrote to %s\n", OUTPUT);

    free(image);
    return error != 0;
}
