#include "vec3.h"

#include <math.h>
#include <stdio.h>

vec3_t vec3_neg(const vec3_t v) {
    return (vec3_t){-v.x, -v.y, -v.z};
}


vec3_t vec3_add(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
}

vec3_t vec3_sub(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };
}

vec3_t vec3_mul(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z
    };
}


vec3_t vec3_add_s(const vec3_t v, double d) {
    return (vec3_t){
        v.x + d,
        v.y + d,
        v.z + d
    };
}

vec3_t vec3_sub_s(const vec3_t v, double d) {
    return (vec3_t){
        v.x - d,
        v.y - d,
        v.z - d
    };
}

vec3_t vec3_mul_s(const vec3_t v, double d) {
    return (vec3_t){
        v.x * d,
        v.y * d,
        v.z * d
    };
}

vec3_t vec3_div_s(const vec3_t v, double d) {
    return vec3_mul_s(v, 1 / d);
}


double vec3_len(const vec3_t v) {
    return sqrt(vec3_len_sqr(v));
}

double vec3_len_sqr(const vec3_t v) {
    return vec3_dot(v, v);
}

double vec3_dot(const vec3_t v1, const vec3_t v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

vec3_t vec3_cross(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x),
    };
}

vec3_t vec3_unit(const vec3_t v) {
    return vec3_div_s(v, vec3_len(v));
}


void vec3_print(const vec3_t v) {
    printf("[%lf %lf %lf]\n", v.x, v.y, v.z);
}
