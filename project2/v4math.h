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
void v4_scale(float *dst, float s);
bool v4_equals(float *a, float *b, float tolerance);

#endif