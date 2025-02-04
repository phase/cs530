/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#ifndef CS530_V3MATH_H
#define CS530_V3MATH_H
#include <stdbool.h>

void v3_from_points(float *dst, float *a, float *b); // form v3 from a to b
void v3_add(float *dst, float *a, float *b);
void v3_subtract(float *dst, float *a, float *b);
float v3_dot_product(float *a, float *b);
void v3_cross_product(float *dst, float *a, float *b);
void v3_scale(float *dst, float s);
float v3_angle(float *a, float *b); // angle between a and b
float v3_angle_quick(float *a, float *b); // angle between a and b; no cos-1
void v3_reflect(float *dst, float *v, float *n);
float v3_length(float *a);
void v3_normalize(float *dst, float *a);
bool v3_equals(float *a, float *b, float tolerance);
bool almostEqual(double a, double b, float tolerance);
bool v4_equals(float *a, float *b, float tolerance);
void v4_matrix_multiply(float* dst, float* matrix, float* a);

#endif //CS530_V3MATH_H
