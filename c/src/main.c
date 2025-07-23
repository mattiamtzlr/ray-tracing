#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "lodepng.h"
#include "ray.h"
#include "vec3.h"

#define OUTPUT "output.png"


/* writes the color for the given ray or NULL into `res` */
color_t ray_color(const ray_t ray);


int main(void) {
    /* image */
    double aspect_ratio = 16.0 / 9.0;
    const unsigned int image_width = 480; /* NOTE: IMG WIDTH */
    const unsigned int image_height = (int)(image_width / aspect_ratio);


    /* coordinate system, centered at camera:
     *   x points to the right
     *   y points up
     *   z points forward, i.e. the viewing direction is negative z */


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
    vec3_t pixel_delta_u = vec3_div_s(viewport_u, image_width);
    vec3_t pixel_delta_v = vec3_div_s(viewport_v, image_height);

    /* location of upper left pixel:
     * viewport_upper_left = camera_center - vec3(0, 0, focal_length)
     *                                     - viewport_u/2 - viewport_v/2 */
    point3_t viewport_upper_left = vec3_sub(
        camera_center,
        vec3_add(
            (vec3_t){0, 0, focal_length},
            vec3_add(vec3_div_s(viewport_u, 2), vec3_div_s(viewport_v, 2))
        )
    );

    /* upper_left_pixel
     *     = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v) */
    point3_t upper_left_pixel = vec3_add(
        viewport_upper_left,
        vec3_mul_s(vec3_add(pixel_delta_u, pixel_delta_v), 0.5)
    );


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
        printf("\rScanlines remaining: %4u", image_height - row);
        fflush(stdout);

        for (unsigned int col = 0; col < image_width; col++) {
            /* pixel_center = upper_left_pixel
             *                  + (col * pixel_delta_u) + (row * pixel_delta_v) */
            point3_t pixel_center = vec3_add(
                upper_left_pixel,
                vec3_add(
                    vec3_mul_s(pixel_delta_u, col),
                    vec3_mul_s(pixel_delta_v, row)
                )
            );

            vec3_t ray_direction = vec3_sub(pixel_center, camera_center);
            ray_t ray = {camera_center, ray_direction};

            color_t pixel_color = ray_color(ray);

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


double hit_sphere(const point3_t center, double radius, const ray_t ray) {
    /* ray sphere intersection: for a sphere at center C and radius r, a point
     * P intersects the sphere if
     *     (C - P) * (C - P) = r^2.
     *
     * substituting P for our ray P(t) = Q + td and solving for t, we get
     *     (t^2)*d*d - 2td*(C-Q) + ((C-Q) * (C-Q) - r^2) = 0
     * which can be solved quadratically, where 1 or 2 roots correspond to an
     * intersection. */

    vec3_t center_vec = vec3_sub(center, ray.origin);
    double a = vec3_len_sqr(ray.direction);
    double b = -2.0 * vec3_dot(ray.direction, center_vec);
    double c = vec3_len_sqr(center_vec) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    /* return hit distance */
    if (discriminant < 0)
        return -1;
    return (-b - sqrt(discriminant)) / (2 * a);
}

color_t ray_color(const ray_t ray) {
    /* test sphere at z = -1 */
    point3_t sphere_center = {0, 0, -1};
    double t = hit_sphere(sphere_center, 0.5, ray);
    if (t > 0) {
        vec3_t normal = vec3_unit(vec3_sub(ray_at(ray, t), sphere_center));
        return vec3_mul_s(
            (color_t){normal.x + 1, normal.y + 1, normal.z + 1},
            0.5
        );
    }

    /* background: simple gradient */
    vec3_t unit_dir = vec3_unit(ray.direction);

    double a = 0.5 * (unit_dir.y + 1.0);
    color_t color1 = {1.0, 1.0, 1.0};
    color_t color2 = {0.5, 0.3, 1.0};

    return vec3_add(vec3_mul_s(color1, 1.0 - a), vec3_mul_s(color2, a));
}

