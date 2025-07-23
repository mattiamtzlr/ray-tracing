#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "lodepng.h"
#include "ray.h"
#include "vec3.h"

#define OUTPUT "output.png"


/* writes the color for the given ray or NULL into `res` */
color_t *ray_color(color_t *res, const ray_t *ray);


int main(void) {
    /* image */
    double aspect_ratio = 16.0 / 9.0;
    const unsigned int image_width = 480; /* NOTE: IMG WIDTH */
    const unsigned int image_height = (int)(image_width / aspect_ratio);


    /* camera */
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width =
        viewport_height * (double)image_width / image_height;
    point3_t camera_center = {0};

    /* vectors accross horizontal and down vertical viewport edges */
    vec3_t viewport_u = {viewport_width, 0, 0};
    vec3_t viewport_v = {0, -viewport_height, 0};

    /* horizontal and vertical delta vectors from pixel to pixel */
    vec3_t pixel_delta_u = {0};
    vec3_div_s(&pixel_delta_u, &viewport_u, image_width);
    vec3_t pixel_delta_v = {0};
    vec3_div_s(&pixel_delta_v, &viewport_v, image_height);

    /* location of upper left pixel:
     * viewport_upper_left = camera_center - vec3(0, 0, focal_length)
     *                                     - viewport_u/2 - viewport_v/2 */
    point3_t viewport_upper_left = {0};
    vec3_t tmp = {0, 0, focal_length};
    vec3_sub(&viewport_upper_left, &camera_center, &tmp);
    vec3_div_s(&tmp, &viewport_u, 2);
    vec3_sub(&viewport_upper_left, &viewport_upper_left, &tmp);
    vec3_div_s(&tmp, &viewport_v, 2);
    vec3_sub(&viewport_upper_left, &viewport_upper_left, &tmp);

    /* upper_left_pixel
     *     = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v) */
    point3_t upper_left_pixel = {0};
    vec3_add(&tmp, &pixel_delta_u, &pixel_delta_v);
    vec3_mul_s(&tmp, &tmp, 0.5);
    vec3_add(&upper_left_pixel, &viewport_upper_left, &tmp);



    /* allocate image, use three color channels */
    const size_t image_size = image_width * image_height * 3;

    unsigned char *image = malloc(image_size);
    if (image == NULL) {
        fputs("Memory allocation of image failed", stderr);
        return 1;
    }


    /* render left-to-right, top-to-bottom */
    for (unsigned int row = 0; row < image_height; row++) {
        /* progress indicator */
        printf("\rScanlines remaining: %u", image_height - row);
        fflush(stdout);

        for (unsigned int col = 0; col < image_width; col++) {
            vec3_t tmp = {0};

            /* pixel_center = upper_left_pixel
             *                  + (col * pixel_delta_u) + (j * pixel_delta_v) */
            point3_t pixel_center = {0};
            vec3_mul_s(&tmp, &pixel_delta_u, col);
            vec3_add(&pixel_center, &upper_left_pixel, &tmp);
            vec3_mul_s(&tmp, &pixel_delta_v, row);
            vec3_add(&pixel_center, &pixel_center, &tmp);

            vec3_t ray_direction = {0};
            vec3_sub(&ray_direction, &pixel_center, &camera_center);
            ray_t ray = {&camera_center, &ray_direction};

            color_t pixel_color = {0};
            ray_color(&pixel_color, &ray);

            /* scaled values, ranging from 0 to 255, for writing */
            unsigned char ir = (unsigned char)(255.999 * pixel_color.x);
            unsigned char ig = (unsigned char)(255.999 * pixel_color.y);
            unsigned char ib = (unsigned char)(255.999 * pixel_color.z);

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
        fprintf(stderr, "\rPNG encoding error %u: %s\n", error,
                lodepng_error_text(error));
    else
        printf("\rSuccessfully wrote to %s\n", OUTPUT);

    free(image);
    return error != 0;
}


color_t *ray_color(color_t *res, const ray_t *ray) {
    if (res != NULL) {
        /* background: simple gradient */
        vec3_t unit_dir = {0};
        vec3_unit(&unit_dir, ray->direction);

        double a = 0.5 * (unit_dir.y + 1.0);
        color_t color1 = {1.0, 1.0, 1.0};
        color_t color2 = {0.8, 0.4, 1.0};

        color_t tmp = {0};
        vec3_mul_s(res, &color1, 1.0 - a);
        vec3_mul_s(&tmp, &color2, a);
        vec3_add(res, res, &tmp);
    }
    return res;
}

