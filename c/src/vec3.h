#pragma once

/* vec3 type */
typedef struct {
    double x, y, z;
} vec3_t;


/* point3 type, alias of vector */
typedef vec3_t point3_t;

vec3_t vec3_neg(const vec3_t v);

vec3_t vec3_add(const vec3_t v1, const vec3_t v2);
vec3_t vec3_sub(const vec3_t v1, const vec3_t v2);
vec3_t vec3_mul(const vec3_t v1, const vec3_t v2);

vec3_t vec3_add_s(const vec3_t v, double d);
vec3_t vec3_sub_s(const vec3_t v, double d);
vec3_t vec3_mul_s(const vec3_t v, double d);
vec3_t vec3_div_s(const vec3_t v, double d);

double vec3_len(const vec3_t v);
double vec3_len_sqr(const vec3_t v);

double vec3_dot(const vec3_t v1, const vec3_t v2);
vec3_t vec3_cross(const vec3_t v1, const vec3_t v2);
vec3_t vec3_unit(const vec3_t v);

void vec3_print(const vec3_t v);
