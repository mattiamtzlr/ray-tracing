#include "ray.h"

#include <unistd.h>

point3_t ray_at(ray_t ray, double t) {
    return vec3_add(
        ray.origin,
        vec3_mul_s(ray.direction, t)
    );
}
