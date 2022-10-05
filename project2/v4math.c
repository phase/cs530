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

void v4_scale(float *dst, float s) {
    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;
    dst[3] = dst[3] * s;
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
