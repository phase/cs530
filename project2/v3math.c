/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#include "v3math.h"
#include <math.h>

void v3_from_points(float *dst, float *a, float *b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

void v3_add(float *dst, float *a, float *b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}

void v3_subtract(float *dst, float *a, float *b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

float v3_dot_product(float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void v3_cross_product(float *dst, float *a, float *b) {
    dst[0] = a[1] * b[2] - a[2] * b[1];
    dst[1] = a[2] * b[0] - a[0] * b[2];
    dst[2] = a[0] * b[1] - a[1] * b[0];
}

void v3_scale(float *dst, float s) {
    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;
}

float v3_angle(float *a, float *b) {
    return 0;
}

float v3_angle_quick(float *a, float *b) {
    return 0;
}

void v3_reflect(float *dst, float *v, float *n) {

}

float v3_length(float *a) {
    return 0;
}

void v3_normalize(float *dst, float *a) {

}

bool almostEqual(double a, double b, float tolerance) {
    return fabs(a - b) < tolerance;
}

bool v3_equals(float *a, float *b, float tolerance) {
    return almostEqual(a[0], b[0], tolerance) &&
           almostEqual(a[1], b[1], tolerance) &&
           almostEqual(a[2], b[2], tolerance);
}
