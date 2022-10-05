/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#include "v4math.h"
#include <math.h>

void v4_from_points(float *dst, float *a, float *b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
}

void v4_add(float *dst, float *a, float *b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3] + b[3];
}

void v4_subtract(float *dst, float *a, float *b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
}

float v4_dot_product(float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void v4_cross_product(float *dst, float *a, float *b) {
    // TODO
}

void v4_scale(float *dst, float s) {
    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;
    dst[3] = dst[3] * s;
}

float v4_angle(float *a, float *b) {
    float PI = 3.14;
    float *a_normal, *b_normal;
    v4_normalize(a_normal, a);
    v4_normalize(b_normal, b);
    return acos(v4_dot_product(a_normal, b_normal))*180/PI;
} // angle between a and b

float v4_angle_quick(float *a, float *b) {
    float *a_normal, *b_normal;
    v4_normalize(a_normal, a);
    v4_normalize(b_normal, b);
    return v4_dot_product(a_normal, b_normal);
} // angle between a and b; no cos-1

void v4_reflect(float *dst, float *v, float *n) {
    // TODO
}

float v4_length(float *a) {
    return sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2) + pow(a[3], 2));
}

void v4_normalize(float *dst, float *a) {
    float len = v4_length(a);
    dst[0] = a[0]/len;
    dst[1] = a[1]/len;
    dst[2] = a[2]/len;
    dst[3] = a[3]/len;
}

bool almostEqual(double a, double b, float tolerance) {
    return fabs(a - b) < tolerance;
}

bool v4_equals(float *a, float *b, float tolerance) {
    return almostEqual(a[0], b[0], tolerance) &&
           almostEqual(a[1], b[1], tolerance) &&
           almostEqual(a[2], b[2], tolerance) &&
           almostEqual(a[3], b[3], tolerance);
}

void v4_matrix_multiply(float* dst, float* matrix, float* a) {
    // TODO
}
