#pragma once
#include "vec3.h"

/* ray type with origin and direction vector, representing a line in space */
typedef struct {
    point3_t *origin;
    vec3_t *direction;
} ray_t;

/* returns the point along the ray at distance t */
point3_t *ray_at(point3_t *res, ray_t *ray, double t);
