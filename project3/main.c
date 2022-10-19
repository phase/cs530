#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "v3math.h"
#include "ppm.h"

typedef struct{
    int radius;
} sphere;

typedef struct{
    float normal[3];
} plane;

typedef struct{
    int flag; //if flag = 0 then it is a sphere, flag = 1 then it is a plane.
    float color[3];
    float position[3];
    sphere *Sphere;
    plane *Plane;
} Object;

typedef struct{
    // camera width and height
    float width;
    float height;
    Object **objects;
    int size;
} Scene;

typedef struct {
    float* position;
    float* unitRay;
} Ray;

FILE *extractData(FILE * fh, char *ch, char *str){
    int count = 0;
    *ch = fgetc(fh);
    while (*ch != ',' && *ch != ':' && *ch != ' ' && *ch != EOF && *ch !='[' && *ch != ']' && *ch != '\n'){
        //printf("%c", *ch);
        str[count] = *ch;
        count++;
        *ch = fgetc(fh);
    }
    //printf("%c", *ch);
    if(count > 0)
        str[count] = '\0';
    else
        str[0] = '\0';
    return fh;
}

Scene readFile(char *filename){
    // read the json file
    FILE *fh = fopen(filename, "r");
    char ch = ' ';
    char *str;
    int size = 0;
    int count;
    Object **objects = malloc(sizeof(Object*) * 128);
    float width = -1, height = -1;
    while(ch != EOF){
        str = malloc(sizeof(char) * 20);
        fh = extractData(fh, &ch, str);
        if(strcmp(str, "sphere") == 0){
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[size] = obj;

            obj->flag = 0;
            obj->Sphere = malloc(sizeof(sphere));
            while(ch != '\n' && ch != EOF){
                str = malloc(sizeof(char) * 20);
                fh = extractData(fh, &ch, str);
                if(strcmp(str, "color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            printf("%s\n", str);
                            obj->color[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if (strcmp(str, "position") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            printf("%s\n", str);
                            obj->position[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if (strcmp(str, "radius") == 0){
                    fgetc(fh);
                    fh = extractData(fh, &ch, str);
                    printf("%s\n", str);
                    obj->Sphere->radius = atof(str);
                }

            }
            size++;
        }
        if(strcmp(str, "plane") == 0){
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[size] = obj;

            obj->flag = 1;
            obj->Plane = malloc(sizeof(plane));
            while(ch != '\n' && ch != EOF){
                str = malloc(sizeof(char) * 20);
                fh = extractData(fh, &ch, str);
                if(strcmp(str, "color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            printf("%s\n", str);
                            obj->color[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if (strcmp(str, "position") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            printf("%s\n", str);
                            obj->position[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if (strcmp(str, "normal") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            printf("%s\n", str);
                            obj->Plane->normal[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }

            }
            size++;
        }
        // parse camera properties
        if (strcmp(str, "camera") == 0) {
            while (ch != '\n' && ch != EOF) {
                char item[20];
                extractData(fh, &ch, item);
                if (strcmp(item, "width") == 0) {
                    char buffer[20];
                    fgetc(fh); // skip space
                    extractData(fh, &ch, buffer);
                    if (strlen(buffer) > 0) {
                        width = atof(buffer);
                    }
                } else if (strcmp(item, "height") == 0) {
                    char buffer[20];
                    fgetc(fh); // skip space
                    extractData(fh, &ch, buffer);
                    if (strlen(buffer) > 0) {
                        height = atof(buffer);
                    }
                }
            }
        }
    }
    printf("\nSphere - Radius: %d, Color: [%f, %f, %f], Position: [%f, %f, %f]", objects[0]->Sphere->radius, objects[0]->color[0], objects[0]->color[1], objects[0]->color[2], objects[0]->position[0], objects[0]->position[1], objects[0]->position[2]);
    printf("\nPlane - Normal: [%f, %f, %f], Color: [%f, %f, %f], Position: [%f, %f, %f]", objects[1]->Plane->normal[0], objects[1]->Plane->normal[1], objects[1]->Plane->normal[2], objects[1]->color[0], objects[1]->color[1], objects[1]->color[2], objects[1]->position[0], objects[1]->position[1], objects[1]->position[2]);
    printf("\nCamera width: %f, height: %f\n", width, height);
    return (Scene){
        .width = width,
        .height = height,
        .objects = objects,
        .size = size + 1
    };
}

int *createPixArray(Object *objects){
    // convert objects and put them in int array.
}

void writePixArray(int *pixArray){
    // Write the pix array data to P3 file.
}

int *raycastSphere(sphere *sphere){
    //Cast a ray to a sphere.

}

float findPlaneIntersection(Ray ray, Object *obj){
    float pDiff[3];
    v3_subtract(pDiff, ray.position, obj->position);
    float top = v3_dot_product(obj->Plane->normal, pDiff);
    float bottom = v3_dot_product(obj->Plane->normal, ray.unitRay);
    return top / bottom;
}

typedef struct {
    bool valid;
    Object *hitObject;
    float distance;
} RayResult;

RayResult shoot(Scene scene, Ray ray) {
    RayResult result = {false, NULL, INFINITY};
    for (int m = 0; m < scene.size; m++) {
        Object *obj = scene.objects[m];
        if (obj == NULL) { continue; }

        // call appropriate function to cast ray.
        float distance;
        if (obj->flag == 0) {
            // TODO spheres
            continue;
        } else if (obj->flag == 1) {
            distance = findPlaneIntersection(ray, obj);
        } else {
            continue;
        }
        if (distance > 0 && distance < result.distance) {
            result.valid = true;
            result.hitObject = obj;
            result.distance = distance;
        }
    }
    return result;
}

Image *render(Scene scene, int imageWidth, int imageHeight) {
    printf("rendering scene\n");
    float pixelWidth = scene.width / (float) imageWidth;
    float pixelHeight = scene.height / (float) imageHeight;

    // specified by the instruction document
    float center[3] = {0.0f, 0.0f, 0.0f};

    Image *image = newImage(imageWidth, imageHeight, 255);

    for (int i = 0; i < imageHeight; i++) { // for each row
        float py = center[1] - scene.height / 2.0f + pixelHeight * ((float) i + 0.5f); // y coord of row
        for (int j = 0; j < imageWidth; j++) { // for each column
            float px = center[0] - scene.width / 2.0f + pixelWidth * ((float) j + 0.5f); // x coord of column
            float pz = center[2];

            // build ray to cast
            float unitRay[3]; // unit ray vector
            float p[3] = (float[3]) {px, py, pz};
            v3_normalize(unitRay, p);
            Ray ray = {.position = p, .unitRay = unitRay};

            // cast ray
            RayResult result = shoot(scene, ray);
            // start with black
            float *color = (float[3]) {0.0f, 0.0f, 0.0f};
            // only set the color if we hit a valid object
            if (result.valid) {
                printf("found obj = %f %d\n", result.distance, result.hitObject->flag);
                color = result.hitObject->color;
            }
            // store color in image
            // convert 0.0 - 1.0 color to 0 - 255
            addChannel(image, (int) floorf(color[0] * 255));
            addChannel(image, (int) floorf(color[1] * 255));
            addChannel(image, (int) floorf(color[2] * 255));
            addChannel(image, 255);
        }
    }
    return image;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./raycast <scene> <output_image>\n");
    }
    Scene scene = readFile(argv[1]);
    Image *image = render(scene, 100, 100);
    writeP3(image, argv[2]);
    return 0;
}
