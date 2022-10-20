#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "v3math.h"
#include "ppm.h"

typedef struct{
    int radius;
} sphere;

typedef struct{
    float normal[3];
} plane;

typedef struct {
    float a, b, c, d, e, f, g, h, i, j;
} Quadric;

typedef struct{
    // flag == 0 ==> sphere
    // flag == 1 ==> plane
    // flag == 2 ==> quadric
    int flag;
    float color[3];
    float position[3];
    // todo: turn this into a union
    sphere *Sphere;
    plane *Plane;
    Quadric *quadric;
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
        if (strcmp(str, "quadric") == 0) {
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[size] = obj;

            obj->flag = 2;
            obj->quadric = malloc(sizeof(Quadric));

            while (ch != '\n' && ch != EOF) {
                char item[20];
                extractData(fh, &ch, item);
                if (strcmp(item, "color") == 0) {
                    fgetc(fh); // skip space
                    fscanf(fh, "[%f, %f, %f], ", &obj->color[0], &obj->color[1], &obj->color[2]);
                } else if (strcmp(item, "constants") == 0) {
                    char buffer[20];
                    fscanf(fh, " [%f, %f, %f, %f, %f, %f, %f, %f, %f, %f]",
                           &obj->quadric->a,
                           &obj->quadric->b,
                           &obj->quadric->c,
                           &obj->quadric->d,
                           &obj->quadric->e,
                           &obj->quadric->f,
                           &obj->quadric->g,
                           &obj->quadric->h,
                           &obj->quadric->i,
                           &obj->quadric->j
                   );
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

    return (Scene){
        .width = width,
        .height = height,
        .objects = objects,
        .size = size + 1
    };
}

/**
 * This is using the method described at
 * http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
 */
float findQuadricIntersection(Ray ray, Object *obj, float *hitDest) {
    return -1.0f; // TODO
}

/**
 * This is using the method described at
 * https://education.siggraph.org/static/HyperGraph/raytrace/rtinter1.htm
 */
float findSphereIntersection(Ray ray, Object *obj, float *hitDest) {
    //A = Xd^2 + Yd^2 + Zd^2
    //B = 2 * ((Xd * (X0 - Xc)) + (Yd * (Y0 - Yc)) + (Zd * (Z0 - Zc)))
    //C = (X0 - Xc)^2 + (Y0 - Yc)^2 + (Z0 - Zc)^2 - Sr^2
    float r = (float) obj->Sphere->radius;
    float a = powf(ray.unitRay[0], 2) + powf(ray.unitRay[1], 2) + powf(ray.unitRay[2], 2);
    float b = 2 * ((ray.unitRay[0] * (ray.position[0] - obj->position[0])) +
            (ray.unitRay[1] * (ray.position[1] - obj->position[1])) +
            (ray.unitRay[2] * (ray.position[2] - obj->position[2])));
    float c = powf(ray.position[0] - obj->position[0], 2) +
            powf(ray.position[1] - obj->position[1], 2) +
            powf(ray.position[2] - obj->position[2], 2) -
            powf(r, 2);

    // t0, t1 = (- B + (B^2 - 4*C)^1/2) / 2 where t0 is for (-) and t1 is for (+)
    float d = powf(b, 2) - 4*c;
    if (d < 0.0f) {
        // there is no solution
        return -1.0f;
    }

    /*
     t0, t1 = (- B + (B^2 - 4*C)^1/2) / 2 where t0 is for (-) and t1 is for (+)
     If the discriminant is < 0.0 then there is no real root and no intersection.
     If there is a real root (Disc. > = 0.0) then the smaller positive root is the
     closest intersection point. So we can just compute t0 and if it is positive,
     then we are done, else compute t1.
     */

    float t0 = (-b - sqrtf(d)) / 2;
    float t = t0;
    if (t < 0.0f) {
        float t1 = (-b + sqrtf(d)) / 2;
        t = t1;
    }

    // Ri = [xi, yi, zi] = [x0 + xd * ti ,  y0 + yd * ti,  z0 + zd * ti]
    float rx = ray.position[0] + ray.unitRay[0] * t;
    float ry = ray.position[1] + ray.unitRay[1] * t;
    float rz = ray.position[2] + ray.unitRay[2] * t;
    hitDest[0] = rx;
    hitDest[1] = ry;
    hitDest[2] = rz;
    return fabsf(t);
}

/**
 * This is using the method described in the book
 */
float findSphereIntersectionOld(Ray ray, Object *obj, float *hitDest){
    float r = (float) obj->Sphere->radius;
    float pDiff[3];
    v3_subtract(pDiff, obj->position, ray.position);
    // distance from the start of the ray to the point inside the sphere
    // that is still on the ray and closest to the center of the sphere
    float tClose = v3_dot_product(ray.unitRay, pDiff);

    // use that distance to find the actual point inside the sphere
    float scaledUnitRay[3] = {ray.unitRay[0], ray.unitRay[1], ray.unitRay[2]};
    v3_scale(scaledUnitRay, tClose);
    float xClose[3];
    v3_add(xClose, ray.position, scaledUnitRay);

    float dVec[3];
    v3_subtract(dVec, xClose, obj->position);
    float d = v3_length(dVec);

    if (d > r) {
        return -1.0f;
    }

    float a = sqrtf(r * r - d * d);
    float t = tClose - a;

    if (t > 0.0f) {
        float xHit[3] = {ray.unitRay[0], ray.unitRay[1], ray.unitRay[2]};
        v3_scale(xHit, tClose - a);
        v3_add(xHit, ray.position, xHit);
        hitDest[0] = xHit[0];
        hitDest[1] = xHit[1];
        hitDest[2] = xHit[2];
    }

    return t;
}

float findPlaneIntersection(Ray ray, Object *obj, float *hitDest){
    float pDiff[3];
    v3_subtract(pDiff, ray.position, obj->position);
    float top = v3_dot_product(obj->Plane->normal, pDiff);
    float bottom = v3_dot_product(obj->Plane->normal, ray.unitRay);
    float t = top / bottom;

    if (t > 0.0f) {
        float xHit[3] = {ray.unitRay[0], ray.unitRay[1], ray.unitRay[2]};
        v3_scale(xHit, t);
        v3_add(xHit, ray.position, xHit);
        hitDest[0] = xHit[0];
        hitDest[1] = xHit[1];
        hitDest[2] = xHit[2];
    }
    return t;
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
        float hit[3];
        if (obj->flag == 0) {
            distance = findSphereIntersection(ray, obj, hit);
        } else if (obj->flag == 1) {
            distance = findPlaneIntersection(ray, obj, hit);
        } else if (obj->flag == 2) {
            distance = findQuadricIntersection(ray, obj, hit);
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
    float center[3] = {0.0f, 0.0f, -1.0f};

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
                // printf("found obj = %f %d\n", result.distance, result.hitObject->flag);
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

    // TODO remove debug
    for (int i = 0; i < scene.size; i++) {
        Object* obj = scene.objects[i];
        if (obj == NULL) { continue; }
        printf(" found object %d\n", obj->flag);
        if (obj->flag == 2) {
            // this is a quadric
            printf("  found quadric: [%f, %f, %f, %f, %f, %f, %f, %f, %f, %f]\n",
                   obj->quadric->a,
                   obj->quadric->b,
                   obj->quadric->c,
                   obj->quadric->d,
                   obj->quadric->e,
                   obj->quadric->f,
                   obj->quadric->g,
                   obj->quadric->h,
                   obj->quadric->i,
                   obj->quadric->j
            );
        }
    }

    Image *image = render(scene, 256, 256);
    writeP3(image, argv[2]);
    return 0;
}
