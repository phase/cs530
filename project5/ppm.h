#ifndef CS530_PPM_H
#define CS530_PPM_H

#include <stdint.h>
#include <stdbool.h>

// structure holding the image info
typedef struct {
    int width, height, maxVal;
    uint8_t *data;
    int length;
    uint8_t depth;
} Image;

/**
 * Allocate a new image
 */
Image *newImage(int width, int height, int maxVal);

void freeImage(Image *image);

/**
 * add a channel value to the end of the image's data buffer
 */
void addChannel(Image *image, int value);

int getImageFormat(char *fileName);

bool writeP3(Image *image, const char *fileName);

bool writeP6(Image *image, char *fname);

bool writeP7(Image *image, const char *fileName);

Image *readP3(char *fileName);

Image *readP6(char *fname);

Image *readP7(char *fname);

#endif //CS530_PPM_H
