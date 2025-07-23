#include "vec3.h"

#include <math.h>
#include <stdio.h>

vec3_t *vec3_neg(vec3_t *res, const vec3_t *v) {
    if (res && v) {
        vec3_t ret = {-(v->x), -(v->y), -(v->z)};
        *res = ret;
    }

    return res;
}


vec3_t *vec3_add(vec3_t *res, const vec3_t *v1, const vec3_t *v2) {
    if (res && v1 && v2) {
        vec3_t ret = {
            v1->x + v2->x,
            v1->y + v2->y,
            v1->z + v2->z,
        };
        *res = ret;
    }
    return res;
}

vec3_t *vec3_sub(vec3_t *res, const vec3_t *v1, const vec3_t *v2) {
    if (res && v1 && v2) {
        vec3_t ret = {
            v1->x - v2->x,
            v1->y - v2->y,
            v1->z - v2->z,
        };
        *res = ret;
    }
    return res;
}

vec3_t *vec3_mul(vec3_t *res, const vec3_t *v1, const vec3_t *v2) {
    if (res && v1 && v2) {
        vec3_t ret = {
            v1->x * v2->x,
            v1->y * v2->y,
            v1->z * v2->z,
        };
        *res = ret;
    }
    return res;
}


vec3_t *vec3_add_s(vec3_t *res, const vec3_t *v, double d) {
    if (res && v) {
        vec3_t ret = {
            v->x + d,
            v->y + d,
            v->z + d,
        };
        *res = ret;
    }

    return res;
}

vec3_t *vec3_sub_s(vec3_t *res, const vec3_t *v, double d) {
    if (res && v) {
        vec3_t ret = {
            v->x - d,
            v->y - d,
            v->z - d,
        };
        *res = ret;
    }

    return res;
}

vec3_t *vec3_mul_s(vec3_t *res, const vec3_t *v, double d) {
    if (res && v) {
        vec3_t ret = {
            v->x * d,
            v->y * d,
            v->z * d,
        };
        *res = ret;
    }

    return res;
}

vec3_t *vec3_div_s(vec3_t *res, const vec3_t *v, double d) {
    return vec3_mul_s(res, v, 1 / d);
}


double vec3_len(const vec3_t *v) {
    return sqrt(vec3_len_sqr(v));
}

double vec3_len_sqr(const vec3_t *v) {
    double ret = 0.0;
    if (v)
        ret = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);

    return ret;
}


double dot(const vec3_t *v1, const vec3_t *v2) {
    double ret = 0.0;
    if (v1 && v2)
        ret = (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);

    return ret;
}

vec3_t *cross(vec3_t *res, const vec3_t *v1, const vec3_t *v2) {
    if (res && v1 && v2) {
        vec3_t ret = {
            (v1->y * v2->z) - (v1->z * v2->y),
            (v1->z * v2->x) - (v1->x * v2->z),
            (v1->x * v2->y) - (v1->y * v2->x),
        };
    }

    return res;
}

vec3_t *vec3_unit(vec3_t *res, const vec3_t *v) {
    return vec3_div_s(res, v, vec3_len(v));
}


void vec3_print(const vec3_t *v) {
    if (v)
        printf("[%lf %lf %lf]\n", v->x, v->y, v->z);
    else
        printf("<NULL>\n");
}
