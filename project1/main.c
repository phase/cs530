/**
 * CS 530 - Project 1
 * Authors: Jadon Fowler, Harshith Shakelli
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// structure holding the image info
typedef struct {
    int width, height;
    int *data;
    int length;
} Image;

// Allocate a new image
Image *newImage(int width, int height) {
    Image *image = malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->data = malloc(image->width * image->height);
    image->length = 0;
    return image;
}

// add a pixel value to the end of the image's data buffer
void addPixel(Image *image, int value) {
    image->data[image->length++] = value;
}

int fputAscii(int value, FILE *file) {
    char ascii[12]; // buffer for the ASCII representation
    int len = snprintf(ascii, sizeof(ascii), "%d", value);
    for (int i = 0; i < len; i++) {
        fputc(ascii[i], file);
    }
    return len;
}

bool writeP3(Image *image, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) return false;
    fputc('P', file);
    fputc('3', file);
    fputc('\n', file);
    fputAscii(image->width, file);
    fputc(' ', file);
    fputAscii(image->height, file);
    fputc('\n', file);
    int lineLength = 0;
    for (int i = 0; i < image->length; i++) {
        int len = fputAscii(image->data[i], file);
        lineLength += len + 1;
        if (lineLength >= 65 || i == image->length - 1) {
            lineLength = 0;
            fputc('\n', file);
        } else {
            fputc(' ', file);
        }
    }
    fclose(file);
    return true;
}

/**
 * Read a P3 file
 * @return true if successful, false otherwise
 */
Image* readP3(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) return NULL;

    char c; // current character
    int index = 0; // index of the character in the file
    // flags for parsing different elements
    int parsingNumFlag = 0, parsingCommentFlag = 0;

    // buffer for ascii numbers - holds at most 7 digits
    int MAX_DIGITS = 7;
    char buffer[7];
    int bufferIndex = 0;
    // width and height of the image
    // width appears first in the file, then height, then the pixels
    int width = -1, height = -1;
    Image *image = NULL;

    // loop over every character in the file
    // this will parse different sections by setting the flags above
    while ((c = fgetc(file)) != EOF) {
        // check magic number
        if (index == 0 && c != 'P') return NULL;
        if (index == 1 && c != '3') return NULL;
        if (index == 0 || index == 1) {
            index++;
            continue;
        }

        // handle comments
        if (c == '#') {
            // if this is a #, this is the start of a comment
            parsingCommentFlag = 1;
            continue;
        } else if (parsingCommentFlag) {
            if (c == '\n') {
                // this is the end of a comment
                parsingCommentFlag = 0;
            }
            continue;
        }

        if (isnumber(c)) { // c is a number
            parsingNumFlag = 1;
            // bail out if the index is greater than the buffer size
            // this will only happen if we hit a number that is too large
            if (bufferIndex >= MAX_DIGITS) return NULL;

            buffer[bufferIndex++] = c;
        } else if (isspace(c)) { // c is a space, tab, newline, etc
            if (parsingNumFlag == 1) {
                // we hit a whitespace character while parsing a number,
                // finish parsing the number
                parsingNumFlag = 0;
                bufferIndex = 0;
                // this is the fully parsed number
                int value = atoi(buffer);

                // reset buffer
                for (int i = 0; i < MAX_DIGITS; i++) { buffer[i] = '\0'; }

                if (value > 255) return false; // invalid number
                if (width == -1) {
                    // we found the width
                    width = value;
                } else if (height == -1) {
                    // we found the height
                    height = value;
                    // we have both the width and the height, build
                    image = newImage(width, height);
                } else {
                    addPixel(image, value);
                }
            }
            continue; // skip whitespace
        }
        index++;
    }
    fclose(file);
    return image;
}

int main(int argc, char *argv[]) {
    // call readPX based on args
    printf("P3 / P6 / P7 image reader / writer\n");
    if (argc != 4) {
        printf("Invalid arguments! Expected ./ppmrw <format> <input> <output>\n");
    }

    char *format = argv[1];
    char *inputFile = argv[2];
    char *outputFile = argv[3];
    if (format[0] == '3') {
        printf("Converting to P3\n");
        Image* image = readP3(inputFile);
        writeP3(image, outputFile);
    } else {
        printf("Unknown format %s\n", format);
    }
    return 0;
}
