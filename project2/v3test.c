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

bool test_v4_binop(char *testName, float *a, float *b, float *expected, void (*func)(float *, float *, float *)) {
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
}

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
}

void test_v4() {
    { // test v4_from_points
        test_v4_binop(
                "v4_from_points 1",
                (float[4]) {1, 2, 3, 5},
                (float[4]) {3, 2, 1, 2},
                (float[4]) {-2, 0, 2, 7},
                v4_from_points);
        test_v4_binop(
                "v4_from_points 2",
                (float[4]) {6, 5, 2, 1},
                (float[4]) {5, 8, 3, 7},
                (float[4]) {1, -3, -1, 9},
                v4_from_points);
        test_v4_binop(
                "v4_from_points 3",
                (float[4]) {6, 4, 7, 5},
                (float[4]) {12, 5, 3, 2},
                (float[4]) {-6, -1, 4, 7},
                v4_from_points);
    }
    { // test v4_add
        test_v4_binop(
                "v4_add 1",
                (float[4]) {1, 2, 3, 1},
                (float[4]) {3, 2, 1, 6},
                (float[4]) {4, 4, 4, 7},
                v4_add);
        test_v4_binop(
                "v4_add 2",
                (float[4]) {6, 5, 2, 1},
                (float[4]) {5, 8, 3, 2},
                (float[4]) {11, 13, 5, 3},
                v4_add);
        test_v4_binop(
                "v4_add 3",
                (float[4]) {6, 4, 7, 6},
                (float[4]) {12, 5, 3, 4},
                (float[4]) {18, 9, 10, 10},
                v4_add);
    }
    { // test v4_subtract
        test_v4_binop(
                "v4_subtract 1",
                (float[4]) {1, 2, 3, 6},
                (float[4]) {3, 2, 1, 4},
                (float[4]) {-2, 0, 2, 2},
                v4_subtract);
        test_v4_binop(
                "v4_subtract 2",
                (float[4]) {6, 5, 2, 8},
                (float[4]) {5, 8, 3, 4},
                (float[4]) {1, -3, -1, 4},
                v4_subtract);
        test_v4_binop(
                "v4_subtract 3",
                (float[4]) {6, 4, 7, 4},
                (float[4]) {12, 5, 3, 1},
                (float[4]) {-6, -1, 4, 3},
                v4_subtract);
    }
    { // test v4_cross_product
        test_v4_binop(
                "v4_cross_product 1",
                (float[4]) {1, 2, 3, 8},
                (float[4]) {3, 2, 1, 2},
                (float[4]) {-4, 8, -4, 0},
                v4_cross_product);
        test_v4_binop(
                "v4_cross_product 2",
                (float[4]) {6, 5, 2, 7},
                (float[4]) {5, 8, 3, 4},
                (float[4]) {-1, -8, 23, 2},
                v4_cross_product);
        test_v4_binop(
                "v4_cross_product 3",
                (float[4]) {6, 4, 7, 6},
                (float[4]) {12, 5, 3, 3},
                (float[4]) {-23, 66, -18, 2},
                v4_cross_product);
    }
}

int main(int argc, char *argv[]) {
    test_v3();
    test_v4();
    return 0;
}
