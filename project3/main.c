#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "v3math.h"
#include "ppm.h"

typedef struct{
    float color[3];
    int position[3];
    int radius;
} sphere;

typedef struct{
    float color[3];
    int position[3];
    int normal[3];
} plane;

typedef struct{
    int flag; //if flag = 0 then it is a sphere, flag = 1 then it is a plane.
    sphere *Sphere;
    plane *Plane;
} object;

typedef struct{
    // camera width and height
    double width;
    double height;
    object *objects;
    int size;
} Scene;

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
    object *Objects = malloc(sizeof(Objects) * 128);
    double width = -1, height = -1;
    while(ch != EOF){
        str = malloc(sizeof(char) * 20);
        fh = extractData(fh, &ch, str);
        if(strcmp(str, "sphere") == 0){
            Objects[size].flag = 0;
            Objects[size].Sphere = malloc(sizeof(sphere));
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
                            Objects[size].Sphere->color[count] = atof(str);
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
                            Objects[size].Sphere->position[count] = atof(str);
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
                    Objects[size].Sphere->radius = atoi(str);
                }
                
            }
            size++;
        }
        if(strcmp(str, "plane") == 0){
            Objects[size].flag = 1;
            Objects[size].Plane = malloc(sizeof(plane));
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
                            Objects[size].Plane->color[count] = atof(str);
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
                            Objects[size].Plane->position[count] = atof(str);
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
                            Objects[size].Plane->normal[count] = atof(str);
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
                        printf("%s\n", buffer);
                        width = atof(buffer);
                    }
                } else if (strcmp(item, "height") == 0) {
                    char buffer[20];
                    fgetc(fh); // skip space
                    extractData(fh, &ch, buffer);
                    if (strlen(buffer) > 0) {
                        printf("%s\n", buffer);
                        height = atof(buffer);
                    }
                }
            }
        }
    }
    printf("\nSphere - Radius: %d, Color: [%f, %f, %f], Position: [%d, %d, %d]", Objects[0].Sphere->radius, Objects[0].Sphere->color[0], Objects[0].Sphere->color[1], Objects[0].Sphere->color[2], Objects[0].Sphere->position[0], Objects[0].Sphere->position[1], Objects[0].Sphere->position[2]);
    printf("\nPlane - Normal: [%d, %d, %d], Color: [%f, %f, %f], Position: [%d, %d, %d]", Objects[1].Plane->normal[0], Objects[1].Plane->normal[1], Objects[1].Plane->normal[2], Objects[1].Plane->color[0], Objects[1].Plane->color[1], Objects[1].Plane->color[2], Objects[1].Plane->position[0], Objects[1].Plane->position[1], Objects[1].Plane->position[2]);
    printf("\nCamera width: %f, height: %f\n", width, height);
    return (Scene){
        .width = width,
        .height = height,
        .objects = Objects,
        .size = size + 1
    };
}

int *createPixArray(object *Objects){
    // convert objects and put them in int array.
}

void writePixArray(int *pixArray){
    // Write the pix array data to P3 file.
}

int *raycastSphere(sphere *sphere){
    //Cast a ray to a sphere.
}

int *raycastPlane(plane *plane){
    //Cast a ray to a plane.
}

typedef struct {
    object *hitObject;
} RayResult;

void render(Scene scene, double imageWidth, double imageHeight) {
    double pixelWidth = scene.width / imageWidth;
    double pixelHeight = scene.height / imageHeight;

    // specified by the instruction document
    float center[3] = {0.0, 0.0, 0.0};

    for (int i = 0; i < imageHeight; i++) { // for each row
        double py = center[1] - scene.height / 2 + pixelHeight * (i + 0.5); // y coord of row
        for (int j = 0; j < imageWidth; j++) { // for each column
            double px = center[0] - scene.width / 2 + pixelWidth * (j + 0.5); // x coord of column
            double pz = center[2];

            float unitRay[3]; // unit ray vector
            float p[3] = (float[3]) {px, py, pz};
            v3_normalize(unitRay, p);
            // float *x = shoot(ur); // return position of first hit
            // image[i][j] = shade(x); // pixel colored by object hit
        }
    }

    // todo put in shoot function
    for (int i = 0; i < scene.size; i++){
        object obj = scene.objects[i];

        // call appropriate function to cast ray.
        if (obj.flag == 0){
            raycastSphere(obj.Sphere);
        } else if (obj.flag == 1){
            raycastPlane(obj.Plane);
        }
    }
}

int *drawRay(int *pixArray, int *startPoint, int *endPoint){
    // Draw a ray from given point to end point.
}

int main(int argc, char *argv[]){
    Scene scene = readFile(argv[1]);
    render(scene, 100, 100);
    return 0;
}
