#include "ray.h"

#include <unistd.h>

point3_t *ray_at(point3_t *res, ray_t *ray, double t) {
    if (res && ray) {
        point3_t ret = {0};
        vec3_mul_s(&ret, ray->direction, t);
        vec3_add(&ret, ray->origin, &ret);

        *res = ret;
    }

    return res;
}
