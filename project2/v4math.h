/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#ifndef CS530_V4MATH_H
#define CS530_V4MATH_H
#include <stdbool.h>

void v4_from_points(float *dst, float *a, float *b);
void v4_add(float *dst, float *a, float *b);
void v4_subtract(float *dst, float *a, float *b);
float v4_dot_product(float *a, float *b);
void v4_cross_product(float *dst, float *a, float *b);
void v4_scale(float *dst, float s);
float v4_angle(float *a, float *b); // angle between a and b
float v4_angle_quick(float *a, float *b); // angle between a and b; no cos-1
void v4_reflect(float *dst, float *v, float *n);
float v4_length(float *a);
void v4_normalize(float *dst, float *a);
bool v4_equals(float *a, float *b, float tolerance);
void v4_matrix_multiply(float* dst, float* matrix, float* a);

#endif