/**
 * CS 530 - Project 5
 * Authors: Jadon Fowler, Harshith Shakelli
 */
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
    float position[3]; // The location of the light
    float color[3]; // The color of the light (vector)
    float radialA0; // The lowest order term in the radial attenuation function (lights only)
    float radialA1; // The middle order term in the radial attenuation function (lights only)
    float radialA2; // The highest order term in the radial attenuation function (lights only)
    float theta; // The angle of the spotlight cone (spot lights only) in degrees;
    /* If theta = 0 or is not present, the light is a point light;
    Note that the C trig functions assume radians so you may need to do
    a conversion. */
    float angularA0; // The exponent in the angular attenuation function (spot lights only)
    float direction[3]; // The direction vector of the spot light (spot lights only)
} Light;

typedef struct {
    float a, b, c, d, e, f, g, h, i, j;
} Quadric;

typedef struct{
    // flag == 0 ==> sphere
    // flag == 1 ==> plane
    // flag == 2 ==> quadric
    int flag;
    float diffuse_color[3];
    float specular_color[3];
    float position[3];
    float reflectivity;
    float refractivity; // only on spheres?
    float ior; // The index of refraction of the volume. only on spheres?
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
    int objectCount;
    Light **lights;
    int lightCount;
} Scene;

typedef struct {
    float* position;
    float* unitRay;
} Ray;

void freeScene(Scene scene) {
    for (int i = 0; i < scene.objectCount; i++) {
        free(scene.objects[i]);
    }
    free(scene.objects);
    for (int i = 0; i < scene.lightCount; i++) {
        free(scene.lights[i]);
    }
    free(scene.lights);
}

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

    int objectCount = 0;
    int lightCount = 0;
    Object **objects = malloc(sizeof(Object*) * 128);
    Light **lights = malloc(sizeof(Light*) * 128);
    float width = -1, height = -1;

    int count;
    while(ch != EOF){
        str = malloc(sizeof(char) * 20);
        fh = extractData(fh, &ch, str);
        if(strcmp(str, "sphere") == 0){
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[objectCount] = obj;

            obj->flag = 0;
            obj->Sphere = malloc(sizeof(sphere));
            while(ch != '\n' && ch != EOF){
                str = malloc(sizeof(char) * 20);
                fh = extractData(fh, &ch, str);
                if(strcmp(str, "diffuse_color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            obj->diffuse_color[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if(strcmp(str, "specular_color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            obj->specular_color[count] = atof(str);
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
                else if (strcmp(str, "reflectivity") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->reflectivity = atof(item);
                    }
                }
                else if (strcmp(str, "refractivity") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->refractivity = atof(item);
                    }
                }
                else if (strcmp(str, "ior") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->ior = atof(item);
                    }
                }
            }
            objectCount++;
        }
        if(strcmp(str, "plane") == 0){
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[objectCount] = obj;

            obj->flag = 1;
            obj->Plane = malloc(sizeof(plane));
            while(ch != '\n' && ch != EOF){
                str = malloc(sizeof(char) * 20);
                fh = extractData(fh, &ch, str);
                if(strcmp(str, "diffuse_color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            obj->diffuse_color[count] = atof(str);
                            count++;
                        }
                        if(count == 3){
                            break;
                        }
                    }
                }
                else if(strcmp(str, "specular_color") == 0){
                    count = 0;
                    while (true){
                        str = malloc(sizeof(char) * 8);
                        fh = extractData(fh, &ch, str);
                        if(strlen(str) > 0){
                            obj->specular_color[count] = atof(str);
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
                else if (strcmp(str, "reflectivity") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->reflectivity = atof(item);
                    }
                }
                else if (strcmp(str, "refractivity") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->refractivity = atof(item);
                    }
                }
                else if (strcmp(str, "ior") == 0) {
                    fgetc(fh);
                    char item[20];
                    fh = extractData(fh, &ch, item);
                    if (strlen(item) > 0) {
                        obj->ior = atof(item);
                    }
                }
            }
            objectCount++;
        }
        // parse quadrics
        if (strcmp(str, "quadric") == 0) {
            // allocate a new object
            Object *obj = malloc(sizeof(Object));
            objects[objectCount++] = obj;

            obj->flag = 2;
            obj->quadric = malloc(sizeof(Quadric));

            while (ch != '\n' && ch != EOF) {
                char item[20];
                extractData(fh, &ch, item);
                if (strcmp(item, "diffuse_color") == 0) {
                    fgetc(fh); // skip space
                    fscanf(fh, "[%f, %f, %f], ", &obj->diffuse_color[0], &obj->diffuse_color[1], &obj->diffuse_color[2]);
                } else if (strcmp(item, "specular_color") == 0) {
                    fgetc(fh); // skip space
                    fscanf(fh, "[%f, %f, %f], ", &obj->specular_color[0], &obj->specular_color[1], &obj->specular_color[2]);
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
        }
        // parse light
        else if (strcmp(str, "light") == 0) {
            Light *light = malloc(sizeof(Light));
            lights[lightCount++] = light;

            int j = 0;
            while (ch != '\n' && ch != EOF && j++ < 10) {
                char item[20];
                fscanf(fh, "%s ", item);
                if (strcmp(item, "position:") == 0) {
                    fscanf(fh, "[%f, %f, %f]", &light->position[0], &light->position[1], &light->position[2]);
                } else if (strcmp(item, "color:") == 0) {
                    fscanf(fh, "[%f, %f, %f]", &light->color[0], &light->color[1], &light->color[2]);
                } else if (strcmp(item, "radial-a0:") == 0) {
                    fscanf(fh, "%f", &light->radialA0);
                } else if (strcmp(item, "radial-a1:") == 0) {
                    fscanf(fh, "%f", &light->radialA1);
                } else if (strcmp(item, "radial-a2:") == 0) {
                    fscanf(fh, "%f", &light->radialA2);
                } else if (strcmp(item, "theta:") == 0) {
                    fscanf(fh, "%f", &light->theta);
                } else if (strcmp(item, "angular-a0:") == 0) {
                    fscanf(fh, "%f", &light->angularA0);
                } else if (strcmp(item, "direction:") == 0) {
                    fscanf(fh, "[%f, %f, %f]", &light->direction[0], &light->direction[1], &light->direction[2]);
                }

                char c = fgetc(fh);
                if (c == ',') {
                    fgetc(fh); // skip space
                } else {
                    ungetc(c, fh);
                    // we have reached the end of the line
                    break;
                }
            }
        }
        // parse camera properties
        else if (strcmp(str, "camera") == 0) {
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
    fclose(fh);
    return (Scene){
        .width = width,
        .height = height,
        .objects = objects,
        .objectCount = objectCount,
        .lights = lights,
        .lightCount = lightCount
    };
}

/**
 * This is using the method described at
 * http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
 */
float findQuadricIntersection(Ray ray, Object *obj, float *hitDest) {
    float t = 0.0f, t0 = 0.0f, t1 = 0.0f, temp = 0.0f;
    float rx, ry, rz; 
    float Aq = obj->quadric->a * powf(ray.unitRay[0], 2) + obj->quadric->b * powf(ray.unitRay[1], 2) + obj->quadric->c * powf(ray.unitRay[2], 2) +
    obj->quadric->d * ray.unitRay[0] * ray.unitRay[1] + obj->quadric->e * ray.unitRay[0] * ray.unitRay[2] + obj->quadric->f * ray.unitRay[1] * ray.unitRay[2];
    float Bq = 2 * obj->quadric->a * ray.position[0] * ray.unitRay[0] + 2 * obj->quadric->b * ray.position[1] * ray.unitRay[1] + 
    2 * obj->quadric->c * ray.position[2] * ray.unitRay[2] + obj->quadric->d * (ray.position[0] * ray.unitRay[1] + ray.position[1] * ray.unitRay[0]) + 
    obj->quadric->e * (ray.position[0] * ray.unitRay[2] + ray.position[2] * ray.unitRay[0]) + obj->quadric->f * (ray.position[1] * ray.unitRay[2] + ray.position[2] * ray.unitRay[1]) +
    obj->quadric->g * ray.unitRay[0] + obj->quadric->h * ray.unitRay[1] + obj->quadric->i * ray.unitRay[2];
    float Cq = obj->quadric->a * powf(ray.position[0], 2) + obj->quadric->b * powf(ray.position[1], 2) + obj->quadric->c * powf(ray.position[2], 1) + 
    obj->quadric->d * ray.position[0] * ray.position[1] + obj->quadric->e * ray.position[0] * ray.position[2] + obj->quadric->f * ray.position[1] * ray.position[2] +
    obj->quadric->g * ray.position[0] + obj->quadric->h * ray.position[1] + obj->quadric->i * ray.position[2] + obj->quadric->j;
    float discrim = powf(Bq, 2) - 4 * Aq * Cq;
    //t = -discrim;
    //printf("\nDiscrim : %f", t);
    if(Aq == 0.0f){
        t = -(Cq/Bq); 
    }
    else if(discrim < 0.0f){
        t = -1.0f;
    }
    else{
        discrim = powf(discrim, 0.5);
        t0 = -((Bq + discrim)/(2 * Aq));
        if(t0 > 0){
            t = t0;
        } 
        else{
            t1 = -((Bq - discrim)/(2 * Aq));
            t = t1;
        }
    }
    hitDest[0] = ray.position[0] + ray.unitRay[0] * t;
    hitDest[1] = ray.position[1] + ray.unitRay[1] * t;
    hitDest[2] = ray.position[2] + ray.unitRay[2] * t;
    return t; // TODO
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
    float d = powf(b, 2) - 4*a*c;
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
    if (t < 0.0f) {
        return -1.0f;
    }

    // Ri = [xi, yi, zi] = [x0 + xd * ti ,  y0 + yd * ti,  z0 + zd * ti]
    float rx = ray.position[0] + ray.unitRay[0] * t;
    float ry = ray.position[1] + ray.unitRay[1] * t;
    float rz = ray.position[2] + ray.unitRay[2] * t;
    hitDest[0] = rx;
    hitDest[1] = ry;
    hitDest[2] = rz;
    return t;
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
    float t =  (top / bottom);

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
    float hitPos[3];
} RayResult;

// shooter can be null
RayResult shoot(Scene scene, Ray ray, Object *shooter) {
    RayResult result = {false, NULL, INFINITY, {0, 0, 0}};
    for (int m = 0; m < scene.objectCount; m++) {
        Object *obj = scene.objects[m];
        if (obj == NULL || obj == shooter) { continue; }

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
            result.hitPos[0] = hit[0];
            result.hitPos[1] = hit[1];
            result.hitPos[2] = hit[2];
        }
    }
    return result;
}

void shade(Scene scene, Ray ray, RayResult result, float *outputColor, int reflectionCount) {
    float color[3] = {0.0f, 0.0f, 0.0f};
    float n[3]; // surface normal
    if (result.hitObject->flag == 1) {
        // plane
        n[0] = result.hitObject->Plane->normal[0];
        n[1] = result.hitObject->Plane->normal[1];
        n[2] = result.hitObject->Plane->normal[2];
    } else if (result.hitObject->flag == 0 || result.hitObject->flag == 2) {
        // sphere or quadric
        v3_subtract(n, result.hitPos, result.hitObject->position);
    } else {
        return;
    }
    v3_normalize(n, n);

    for (int i = 0; i < scene.lightCount; i++) {
        Light *light = scene.lights[i];
        //printf("Light radials: %f %f %f\n", light->radialA0, light->radialA1, light->radialA2);
        // Radial Attenuation
        float L[3];
        v3_subtract(L, light->position, result.hitPos);
        //printf("Here\n");
        //printf("L: %f, %f, %f\n", L[0], L[1], L[2]);
        float d = v3_length(L);
        float radialAttenuation = 1.0f / (light->radialA0 + light->radialA1 * d + light->radialA2 * d * d);
        v3_normalize(L, L);
        float v0[3], vl[3];
        v0[0] = -L[0];
        v0[1] = -L[1];
        v0[2] = -L[2];
        v3_from_points(vl, light->position, result.hitPos);
        // Angular Attenuation
        // TODO cone lights
        float angularAttenuation = 1.0f;
        if (light->theta > 0){
            float cosalpha = v3_dot_product(v0, vl);
            if (cosalpha < acosf(light->theta)) {
                angularAttenuation = 0.0f;
            } else {
                angularAttenuation = powf(cosalpha, light->angularA0);
            }
        }
        float x = v3_dot_product(n, L);
        float diffuse[3] = {0};
        float specular[3] = {0};
        if (x > 0.0f) {
            diffuse[0] = fmax(1.0f, result.hitObject->diffuse_color[0] * light->color[0]);
            diffuse[1] = fmax(1.0f, result.hitObject->diffuse_color[1] * light->color[1]);
            diffuse[2] = fmax(1.0f, result.hitObject->diffuse_color[2] * light->color[2]);
            v3_scale(diffuse, x);
        }
        //v3_scale(diffuse, radialAttenuation);
        float view[3];
        float reflection[3];
        view[0] = L[0];
        view[1] = L[1];
        view[2] = L[2];
        reflection[0] = n[0];
        reflection[1] = n[1];
        reflection[2] = n[2];
        float nDotRin = v3_dot_product(n, view);
        v3_scale(reflection, 2.0f * nDotRin);
        v3_subtract(reflection, reflection, view);
        float b = v3_dot_product(reflection, view);
        if (x > 0 && b > 0) {
            float y = powf(b, 20);
            specular[0] = light->color[0] * result.hitObject->specular_color[0];
            specular[1] = light->color[1] * result.hitObject->specular_color[1];
            specular[2] = light->color[2] * result.hitObject->specular_color[2];
            v3_scale(specular, y);
        }
        float I[3];
        v3_add(I, diffuse, specular);
        v3_scale(I, radialAttenuation * angularAttenuation);
        I[0] = fmin(I[0], 255);
        I[1] = fmin(I[1], 255);
        I[2] = fmin(I[2], 255);
        //float x = -v3_dot_product(L, n);
        v3_add(color, color, I);
    }

    // reflection
    {
        float view[3] = {-ray.unitRay[0], -ray.unitRay[1], -ray.unitRay[2]};
        float reflection[3] = {n[0], n[1], n[2]};
        float nDotRin = v3_dot_product(n, view);
        v3_scale(reflection, 2.0f * nDotRin);
        v3_subtract(reflection, reflection, view);

        Ray reflectionRay = {.position = result.hitPos, .unitRay = reflection};
        RayResult reflectionResult = shoot(scene, reflectionRay, result.hitObject);
        if (reflectionResult.valid && reflectionCount < 6) {
            float r = result.hitObject->reflectivity;
            float reflectedObjectColor[3];
            shade(scene, reflectionRay, reflectionResult, reflectedObjectColor, reflectionCount + 1);
            v3_scale(color, (1.0f) - r);
            v3_scale(reflectedObjectColor, r);
            v3_add(color, color, reflectedObjectColor);
        }
    }

    outputColor[0] = color[0];
    outputColor[1] = color[1];
    outputColor[2] = color[2];
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
            float p[3] = {px, py, pz};
            v3_normalize(unitRay, p);
            Ray ray = {.position = p, .unitRay = unitRay};
            // cast ray
            RayResult result = shoot(scene, ray, NULL);
            // start with black
            float *color = (float[3]) {0.0f, 0.0f, 0.0f};
            // only set the color if we hit a valid object
            if (result.valid) {
                shade(scene, ray, result, color, 0);
            }
            // store color in image
            // convert 0.0 - 1.0 color to 0 - 255
            addChannel(image, (int) fmin(255, floorf(color[0] * 255)));
            addChannel(image, (int) fmin(255, floorf(color[1] * 255)));
            addChannel(image, (int) fmin(255, floorf(color[2] * 255)));
            addChannel(image, 255);
        }
    }
    return image;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: ./raytrace <width> <height> <scene> <output_image>\n");
        return -1;
    }
    Scene scene = readFile(argv[3]);
    Image *image = render(scene, atoi(argv[1]), atoi(argv[2]));
    writeP3(image, argv[4]);
    freeScene(scene);
    freeImage(image);
    return 0;
}
