/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#include <stdio.h>
#include <stdbool.h>
#include "v3math.h"

// From the instruction doc:
// What the actual output looks like is up to you but it should clearly identify the test, pass or fail,
// and on failure, it should identify the expected and actual values. I expect 3-5 tests per function.
// Please consider testing equivalence classes in your tests. You may use Matlab (or similar) to
// help you develop your tests.
// Your tests should demonstrate scale, rotation, and translation using the matrix.

/**
 * Tests a binary operator for length 3 vectors
 * @param testName Name of the test
 * @param a first argument
 * @param b second argument
 * @param expected expected output value
 * @param func binary operator function to use
 * @return true if the test passed
 */
bool test_v3_binop(char *testName, float *a, float *b, float *expected, void (*func)(float *, float *, float *)) {
    float actual[] = {0, 0, 0};
    func(actual, a, b);
    bool result = v3_equals(expected, actual, 0.05f);
    if (result) {
        printf("[\u2713] Test '%s' passed: got %f %f %f\n",
               testName,
               actual[0], actual[1], actual[2]);
    } else {
        printf("[ ] Test '%s' failed: expected= %f %f %f, actual= %f %f %f\n",
               testName,
               expected[0], expected[1], expected[2],
               actual[0], actual[1], actual[2]);
    }
    return result;
}

/*bool test_v4_binop(char *testName, float *a, float *b, float *expected, void (*func)(float *, float *, float *)) {
    float actual[] = {0, 0, 0, 0};
    func(actual, a, b);
    bool result = v4_equals(expected, actual, 0.05f);
    if (result) {
        printf("[\u2713] Test '%s' passed: got %f %f %f %f\n",
               testName,
               actual[0], actual[1], actual[2], actual[3]);
    } else {
        printf("[ ] Test '%s' failed: expected= %f %f %f %f, actual= %f %f %f %f\n",
               testName,
               expected[0], expected[1], expected[2], expected[3],
               actual[0], actual[1], actual[2], actual[3]);
    }
    return result;
}*/

void test_v3() {
    { // test v3_from_points
        test_v3_binop(
                "v3_from_points 1",
                (float[3]) {1, 2, 3},
                (float[3]) {3, 2, 1},
                (float[3]) {-2, 0, 2},
                v3_from_points);
        test_v3_binop(
                "v3_from_points 2",
                (float[3]) {6, 5, 2},
                (float[3]) {5, 8, 3},
                (float[3]) {1, -3, -1},
                v3_from_points);
        test_v3_binop(
                "v3_from_points 3",
                (float[3]) {6, 4, 7},
                (float[3]) {12, 5, 3},
                (float[3]) {-6, -1, 4},
                v3_from_points);
    }
    { // test v3_add
        test_v3_binop(
                "v3_add 1",
                (float[3]) {1, 2, 3},
                (float[3]) {3, 2, 1},
                (float[3]) {4, 4, 4},
                v3_add);
        test_v3_binop(
                "v3_add 2",
                (float[3]) {6, 5, 2},
                (float[3]) {5, 8, 3},
                (float[3]) {11, 13, 5},
                v3_add);
        test_v3_binop(
                "v3_add 3",
                (float[3]) {6, 4, 7},
                (float[3]) {12, 5, 3},
                (float[3]) {18, 9, 10},
                v3_add);
    }
    { // test v3_subtract
        test_v3_binop(
                "v3_subtract 1",
                (float[3]) {1, 2, 3},
                (float[3]) {3, 2, 1},
                (float[3]) {-2, 0, 2},
                v3_subtract);
        test_v3_binop(
                "v3_subtract 2",
                (float[3]) {6, 5, 2},
                (float[3]) {5, 8, 3},
                (float[3]) {1, -3, -1},
                v3_subtract);
        test_v3_binop(
                "v3_subtract 3",
                (float[3]) {6, 4, 7},
                (float[3]) {12, 5, 3},
                (float[3]) {-6, -1, 4},
                v3_subtract);
    }
    { // test v3_cross_product
        test_v3_binop(
                "v3_cross_product 1",
                (float[3]) {1, 2, 3},
                (float[3]) {3, 2, 1},
                (float[3]) {-4, 8, -4},
                v3_cross_product);
        test_v3_binop(
                "v3_cross_product 2",
                (float[3]) {6, 5, 2},
                (float[3]) {5, 8, 3},
                (float[3]) {-1, -8, 23},
                v3_cross_product);
        test_v3_binop(
                "v3_cross_product 3",
                (float[3]) {6, 4, 7},
                (float[3]) {12, 5, 3},
                (float[3]) {-23, 66, -18},
                v3_cross_product);
    }
    { // test v3_dot_product
        if(almostEqual(v3_dot_product((float[3]){6, 7, 8}, (float[3]){11, 16, 21}), 346.00, 1)){
                printf("[\u2713] Test v3_dot_product 1 passed: got %f\n", v3_dot_product((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));
        }
        else{
        printf("[ ] Test v3_dot_product 1 failed: expected= %f, actual= %f\n", 346.00, v3_dot_product((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));   
        }
        if(almostEqual(v3_dot_product((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}), -436.00, 1)){
                printf("[\u2713] Test v3_dot_product 2 passed: got %f\n", v3_dot_product((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));
        }
        else{
        printf("[ ] Test v3_dot_product 2 failed: expected= %f, actual= %f\n", -436.00, v3_dot_product((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));   
        }
        if(almostEqual(v3_dot_product((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}), 1313.00, 1)){
                printf("[\u2713] Test v3_dot_product 3 passed: got %f\n", v3_dot_product((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));
        }
        else{
        printf("[ ] Test v3_dot_product 3 failed: expected= %f, actual= %f\n", 1313.00, v3_dot_product((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));   
        }     
    }
    { // test v3_scale
        float actual[3];
        float expected[3];
        actual[0] = 13;
        actual[1] = 18;
        actual[2] = 9;
        expected[0] = 26;
        expected[1] = 36;
        expected[2] = 18;
        v3_scale(actual, 2);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_scale 1 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_scale 1 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }
        actual[0] = -20;
        actual[1] = 5;
        actual[2] = 14;
        expected[0] = -2000;
        expected[1] = 500;
        expected[2] = 1400;
        v3_scale(actual, 100);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_scale 2 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_scale 2 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }
        actual[0] = -31;
        actual[1] = 43;
        actual[2] = 77;
        expected[0] = 2573;
        expected[1] = -3569;
        expected[2] = -6391;
        v3_scale(actual, -83);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_scale 3 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_scale 3 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }      
    }
    {// test v3_angle
        if(almostEqual(v3_angle((float[3]){6, 7, 8}, (float[3]){11, 16, 21}), 7.66084, 1)){
                printf("[\u2713] Test v3_angle 1 passed: got %f\n", v3_angle((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));
        }
        else{
        printf("[ ] Test v3_angle 1 failed: expected= %f, actual= %f\n", 7.66084, v3_angle((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));   
        }
        if(almostEqual(v3_angle((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}), 138.02164, 1)){
                printf("[\u2713] Test v3_angle 2 passed: got %f\n", v3_angle((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));
        }
        else{
        printf("[ ] Test v3_angle 2 failed: expected= %f, actual= %f\n", 138.02164, v3_angle((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));   
        }
        if(almostEqual(v3_angle((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}), 55.45081, 1)){
                printf("[\u2713] Test v3_angle 3 passed: got %f\n", v3_angle((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));
        }
        else{
        printf("[ ] Test v3_angle 3 failed: expected= %f, actual= %f\n", 55.45081, v3_angle((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));   
        }  
    }
    {// test v3_angle_quick
        if(almostEqual(v3_angle_quick((float[3]){6, 7, 8}, (float[3]){11, 16, 21}), 0.99107, 1)){
                printf("[\u2713] Test v3_angle_quick 1 passed: got %f\n", v3_angle_quick((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));
        }
        else{
        printf("[ ] Test v3_angle_quick 1 failed: expected= %f, actual= %f\n", 0.99107, v3_angle_quick((float[3]){6, 7, 8}, (float[3]){11, 16, 21}));   
        }
        if(almostEqual(v3_angle_quick((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}), -0.74339, 1)){
                printf("[\u2713] Test v3_angle_quick 2 passed: got %f\n", v3_angle_quick((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));
        }
        else{
        printf("[ ] Test v3_angle_quick 2 failed: expected= %f, actual= %f\n", -0.74339, v3_angle_quick((float[3]){-33, 1, 7}, (float[3]){11, -10, -9}));   
        }
        if(almostEqual(v3_angle_quick((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}), 0.56711, 1)){
                printf("[\u2713] Test v3_angle_quick 3 passed: got %f\n", v3_angle_quick((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));
        }
        else{
        printf("[ ] Test v3_angle_quick 3 failed: expected= %f, actual= %f\n", 0.56711, v3_angle_quick((float[3]){-36, 19, -87}, (float[3]){-20, -10, -9}));   
        }  
    }
    {// test v3_length
        if(almostEqual(v3_length((float[3]){6, 7, 8}), 12.20655, 1)){
                printf("[\u2713] Test v3_length 1 passed: got %f\n", v3_length((float[3]){6, 7, 8}));
        }
        else{
        printf("[ ] Test v3_length 1 failed: expected= %f, actual= %f\n", 12.20655, v3_length((float[3]){6, 7, 8}));   
        }
        if(almostEqual(v3_length((float[3]){-33, 1, 7}), 33.74907, 1)){
                printf("[\u2713] Test v3_length 2 passed: got %f\n", v3_length((float[3]){-33, 1, 7}));
        }
        else{
        printf("[ ] Test v3_length 2 failed: expected= %f, actual= %f\n", 33.74907, v3_length((float[3]){-33, 1, 7}));   
        }
        if(almostEqual(v3_length((float[3]){-20, -10, -9}), 24.10394, 1)){
                printf("[\u2713] Test v3_length 3 passed: got %f\n", v3_length((float[3]){-20, -10, -9}));
        }
        else{
        printf("[ ] Test v3_length 3 failed: expected= %f, actual= %f\n", 24.10394, v3_length((float[3]){-20, -10, -9}));   
        } 
    }
    { // test v3_normalize
        float actual[3];
        float expected[3];
        actual[0] = 13;
        actual[1] = 18;
        actual[2] = 9;
        expected[0] = 0.54261;
        expected[1] = 0.75130;
        expected[2] = 0.37565;
        v3_normalize(actual, actual);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_normalize 1 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_normalized 1 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }
        actual[0] = -33;
        actual[1] = 1;
        actual[2] = 7;
        expected[0] = -0.97780;
        expected[1] = 0.02963;
        expected[2] = 0.20741;
        v3_normalize(actual, actual);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_normalize 2 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_normalized 2 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }
        actual[0] = -20;
        actual[1] = -10;
        actual[2] = -9;
        expected[0] = -0.82974;
        expected[1] = -0.41487;
        expected[2] = -0.37338;
        v3_normalize(actual, actual);
        if(v3_equals(actual, expected, 1)){
                printf("[\u2713] Test v3_normalize 3 passed: got %f %f %f\n", actual[0], actual[1], actual[2]);
        }
        else{
        printf("[ ] Test v3_normalized 3 failed: expected= %f %f %f, actual= %f %f %f\n", expected[0], expected[1], expected[2], actual[0], actual[1], actual[2]);   
        }
    }
    { // test v3_reflect
        test_v3_binop(
                "v3_reflect 1",
                (float[3]) {6, 7, 8},
                (float[3]) {11, 16, 21},
                (float[3]) {-7606, -11065, -14524},
                v3_reflect);
        test_v3_binop(
                "v3_reflect 2",
                (float[3]) {-33, 1, 7},
                (float[3]) {11, -10, -9},
                (float[3]) {9559, -8719, -7841},
                v3_reflect);
        test_v3_binop(
                "v3_reflect 3",
                (float[3]) {-36, 19, -87},
                (float[3]) {-20, -10, -9},
                (float[3]) {52484, 26279, 23547},
                v3_reflect);
    }
}

void test_v4() {
    {// test v4_matrix_multiply
        float matrix1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        float a1[4] = {1, 2, 3, 4};
        float actual[4];
        float expected1[4] = {30, 70, 110, 150};
        v4_matrix_multiply(actual, matrix1, a1);
        if(v4_equals(actual, expected1, 1)){
                printf("[\u2713] Test v4_matrix_multiply 1 passed: got %f %f %f %f\n", actual[0], actual[1], actual[2], actual[3]);
        }
        else{
        printf("[ ] Test v4_matrix_multiply 1 failed: expected= %f %f %f %f, actual= %f %f %f %f\n", expected1[0], expected1[1], expected1[2], expected1[3], actual[0], actual[1], actual[2], actual[3]);   
        }
        float matrix2[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
        float a2[4] = {2, 3, 4, 5};
        float expected2[4] = {54, 110, 166, 222};
        v4_matrix_multiply(actual, matrix2, a2);
        if(v4_equals(actual, expected2, 1)){
                printf("[\u2713] Test v4_matrix_multiply 2 passed: got %f %f %f %f\n", actual[0], actual[1], actual[2], actual[3]);
        }
        else{
        printf("[ ] Test v4_matrix_multiply 2 failed: expected= %f %f %f %f, actual= %f %f %f %f\n", expected2[0], expected2[1], expected2[2], expected2[3], actual[0], actual[1], actual[2], actual[3]);   
        }
        float matrix3[16] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
        float a3[4] = {3, 4, 5, 6};
        float expected3[4] = {86, 158, 230, 302};
        v4_matrix_multiply(actual, matrix3, a3);
        if(v4_equals(actual, expected3, 1)){
                printf("[\u2713] Test v4_matrix_multiply 2 passed: got %f %f %f %f\n", actual[0], actual[1], actual[2], actual[3]);
        }
        else{
        printf("[ ] Test v4_matrix_multiply 2 failed: expected= %f %f %f %f, actual= %f %f %f %f\n", expected3[0], expected3[1], expected3[2], expected3[3], actual[0], actual[1], actual[2], actual[3]);   
        }
        
    }
}

int main(int argc, char *argv[]) {
    test_v3(); // Runnings tests for v3 suite.
    test_v4(); // Running tests for v4 suite
    return 0;
}
