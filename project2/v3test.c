/**
 * CS 530 - Project 2
 * Authors: Jadon Fowler, Harshith Shakelli
 */
#include <stdio.h>
#include <stdbool.h>
#include "v3math.h"
#include "v4math.h"

// From the instruction doc:
// What the actual output looks like is up to you but it should clearly identify the test, pass or fail,
// and on failure, it should identify the expected and actual values. I expect 3-5 tests per function.
// Please consider testing equivalence classes in your tests. You may use Matlab (or similar) to
// help you develop your tests.
// Your tests should demonstrate scale, rotation, and translation using the matrix.

bool test_v3_binop(float *a, float *b, float *expected, void (*func)(float *, float *, float *)) {
    float actual[] = {0, 0, 0};
    func(actual, a, b);
    bool result = v3_equals(expected, actual, 0.05f);
    if (result) {
        printf("[\u2713] Test passed\n");
    } else {
        printf("[ ] Test failed\n");
    }
    return result;
}

int main(int argc, char *argv[]) {
    { // test v3_add
        test_v3_binop(
                (float[3]) {1, 2, 3},
                (float[3]) {3, 2, 1},
                (float[3]) {4, 4, 4},
                v3_add);
        test_v3_binop(
                (float[3]) {6, 5, 2},
                (float[3]) {5, 8, 3},
                (float[3]) {11, 13, 5},
                v3_add);
        test_v3_binop(
                (float[3]) {6, 4, 7},
                (float[3]) {12, 5, 3},
                (float[3]) {18, 9, 10},
                v3_add);
    }
    return 0;
}
