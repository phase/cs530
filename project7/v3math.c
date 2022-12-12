/**
 * CS 530 - Project 5
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#include "v3math.h"
#include <math.h>
#include <stdio.h>

void v3_from_points(float *dst, float *a, float *b) { // Function to find vector from 2 points.
    dst[0] = b[0] - a[0];
    dst[1] = b[1] - a[1];
    dst[2] = b[2] - a[2];
}

void v3_add(float *dst, float *a, float *b) { // Function to add two vectors.
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}

void v3_subtract(float *dst, float *a, float *b) { // Function to subtract two vectors.
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

float v3_dot_product(float *a, float *b) { // Function to find dot product of two vectors.
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void v3_cross_product(float *dst, float *a, float *b) { // Function to find cross product of two vectors.
    dst[0] = a[1] * b[2] - a[2] * b[1];
    dst[1] = a[2] * b[0] - a[0] * b[2];
    dst[2] = a[0] * b[1] - a[1] * b[0];
}

void v3_scale(float *dst, float s) { // Function to resize vectors.
    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;
}

float v3_angle(float *a, float *b) { // Function to find angle between two vectors.
    float PI = 3.14;
    float a_normal[3], b_normal[3];
    v3_normalize(a_normal, a);
    v3_normalize(b_normal, b);
    return acos(v3_dot_product(a_normal, b_normal))*180/PI;
}

float v3_angle_quick(float *a, float *b) { // Function to fin the cosine Θ of two vectors.
    float a_normal[3], b_normal[3];
    v3_normalize(a_normal, a);
    v3_normalize(b_normal, b);
    return v3_dot_product(a_normal, b_normal);
}

void v3_reflect(float *dst, float *v, float *n) { // Function to find the reflection of a vector.
    float dp_value = v3_dot_product(n, v);
    dp_value *=2;
    v3_scale(n, dp_value);
    v3_subtract(dst, v, n);
}

float v3_length(float *a) { // Function to find the length of a vector.
    return sqrtf(powf(a[0], 2) + powf(a[1], 2) + powf(a[2], 2));
}

void v3_normalize(float *dst, float *a) { // Function to normalize a vector. 
    float len = v3_length(a);
    dst[0] = a[0]/len;
    dst[1] = a[1]/len;
    dst[2] = a[2]/len;
}

bool almostEqual(double a, double b, float tolerance) { // Function to check if two float values are equal given some toloerence.
    return fabs(a - b) < tolerance;
}

bool v3_equals(float *a, float *b, float tolerance) { // Function to check if two arrays of size 3, are equal given some tolorence.
    return almostEqual(a[0], b[0], tolerance) &&
           almostEqual(a[1], b[1], tolerance) &&
           almostEqual(a[2], b[2], tolerance);
}

bool v4_equals(float *a, float *b, float tolerance) { // Function to check if two arrays of size 4, are equal given some tolorence.
    return almostEqual(a[0], b[0], tolerance) &&
           almostEqual(a[1], b[1], tolerance) &&
           almostEqual(a[2], b[2], tolerance) &&
           almostEqual(a[3], b[3], tolerance);
}

void v4_matrix_multiply(float* dst, float* matrix, float* a) { // Function to multiply a 4x4 Matrix with an array of size 4.
    for(int i = 0; i < 4; i++){
        int sum = 0;
        for(int j = 0; j < 4; j++){
            sum += a[j] * matrix[j + (i*4)];
        }
        dst[i] = sum;
    }
}
