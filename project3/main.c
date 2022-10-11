#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "v3math.h"

typedef struct{
    sphere *Sphere;
    plane *Plane;
} object;

typedef struct{
    char * type;
    float color[3];
    int position[3];
    int radius;
} sphere;

typedef struct{
    char * type;
    float color[3];
    int position[3];
    int normal[3];
} plane;

object *readFile(char *filename){
    // read File
}

int *createPixArray(object *Objects){
    // convert objects and put them in int array.
}

void writePixArray(int *pixArray){
    // Write the pix array data to P3 file.
}

int *raycast(object *Objects){
    // call appropriate function to cast ray.
}

int *raycastSphere(int *pixArray, sphere *Sphere){
    //Cast a ray to a sphere.
}

int *raycastPlane(int *pixArray, plane *Plane){
    //Cast a ray to a plane.
}

int *drawRay(int *pixArray, int *startPoint, int *endPoint){
    // Draw a ray from given point to end point.
}

int main(int argc, char *argv[]){
    return 0;
}