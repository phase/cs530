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
#include "ppm.h"

Image *newImage(int width, int height, int maxVal) {
    Image *image = malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->maxVal = maxVal;
    image->depth = 4;
    // this will always have space for the alpha channel
    image->data = malloc(sizeof(uint8_t) * image->width * image->height * 4);
    image->length = -1;
    image->count = 1;
    return image;
}

void freeImage(Image *image) {
    free(image->data);
    free(image);
}

void addChannel(Image *image, int value) {
    if(image->length == -1){
        image->length = (image->height * image->width * 4) - (image->width * 4 * image->count);
    }
    //printf("%d ", image->length);
    image->data[image->length++] = value;
    if(image->length%(4*image->width) == 0 && image->count <= image->height){
        image->count++;
        image->length = (image->height * image->width * 4) - (image->width * 4 * image->count);
        //printf("\n");
    }
}

int getImageFormat(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) return '\0';
    fgetc(file); // ignore the P
    int format = fgetc(file);
    fclose(file);
    return format;
}

bool writeP7(Image *image, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) return false;
    fputc('P', file);
    fputc('7', file);
    fputc('\n', file);
    fputs("WIDTH ", file);
    fprintf(file, "%d", image->width);
    fputs("\nHEIGHT ", file);
    fprintf(file, "%d", image->height);
    uint8_t depth = image->depth;
    fputs("\nDEPTH ", file);
    fprintf(file, "%d", image->depth);
    fputs("\nMAXVAL ", file);
    fprintf(file, "%d", image->maxVal);
    fputs("\nTUPLTYPE RGB_ALPHA", file);
    fputs("\nENDHDR\n", file);
    if (depth == 4) {
        // this contains r,g,b,a
        fwrite(image->data, sizeof(uint8_t), image->length, file);
    } else if (depth == 3) {
        // this contains r,b,g
        int channelCount = 0;
        for (int i = 0; i < image->length; i++) {
            channelCount++;
            if (channelCount == 4) {
                // ignore the alpha channel
                channelCount = 0;
                continue;
            }
            fputc(image->data[i], file);
        }
    }
    fclose(file);
    return true;
}

bool writeP3(Image *image, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) return false;
    fputc('P', file);
    fputc('3', file);
    fputc('\n', file);
    fprintf(file, "%d", image->width);
    fputc(' ', file);
    fprintf(file, "%d", image->height);
    fputc('\n', file);
    fprintf(file, "%d\n", image->maxVal);
    int lineLength = 0;
    int channelCount = 0;
    for (int i = 0; i < 4*image->height*image->width; i++) {
        channelCount++;
        if (channelCount == 4) {
            // ignore alpha channel
            channelCount = 0;
            continue;
        }
        int len = fprintf(file, "%d", image->data[i]);
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


Image *readP3(char *fileName) {
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
    int width = -1, height = -1, maxVal = -1;
    Image *image = NULL;
    int channelCount = 0;

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

        if (isdigit(c)) { // c is a number
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

                //if (value > 255) return false; // invalid number
                if (width == -1) {
                    // we found the width
                    width = value;
                } else if (height == -1) {
                    // we found the height
                    height = value;
                } else if (maxVal == -1) {
                    maxVal = value;
                    // we have both the width and the height, build
                    image = newImage(width, height, maxVal);
                } else {
                    if (value > maxVal) return NULL; // invalid number
                    channelCount++;
                    addChannel(image, value);
                    if (channelCount == 3) {
                        // add alpha channel
                        addChannel(image, 0xFF);
                        channelCount = 0;
                    }
                }
            }
            continue; // skip whitespace
        }
        index++;
    }
    if(image->length != width * height * 4 || c != EOF){ // Checking if we have a short read or there is more data than specifed in the header. If that is the case then the file is corrupted.
        fclose(file);
        return NULL;
    }
    fclose(file);
    return image;
}

Image* readP6(char* fname){
    FILE * fh = fopen(fname, "rb"); // Opening the P6 file in read mode.
    Image * image = NULL;
    uint8_t * bytes_data;
    int length;
    int count, i;
    int width = -1;
    int height = -1;
    int max_val = -1;
    char ch1, ch2;
    char str[5];
    if (fh == NULL) // Checking if the file exists.
        return NULL;
    ch1 = fgetc(fh);
    ch2 = fgetc(fh);
    if(ch1 != 'P' && ch2 != '6'){ // Checking if the format is correct.
        fclose(fh); // If it is not we close the file;
        return NULL; // and return NULL;
    }
    // Proceed with the rest of the program if it is correct.
    ch1 = fgetc(fh);
    while((width == -1 || height == -1 || max_val == -1) && ch1 != EOF){ // While we do not get the width, height and max value of the file, we loop through this.
        ch1 = fgetc(fh);
        if(ch1 == '#'){ // Checking for comments.
            while(ch1 != '\n'){ // Getting to the end of the comment.
                ch1 = fgetc(fh);
            }
        }
        else{ // If it is not a comment.
            if(isdigit(ch1)){ // Checking if it is a digit
                count = 0;
                while(isdigit(ch1)){ // reading the entire number.
                    str[count] = ch1;
                    count++;
                    ch1 = fgetc(fh);
                }
                str[count] = '\0'; // adding the escape character at the end.
                if(width == -1){ // If width not read, read width.
                    width = atoi(str);
                }
                else if(width != -1 && height == -1){ // else if width is obtained read height.
                    height = atoi(str);
                }
                else{ // else read max val.
                    max_val = atoi(str);
                }
            }
        }
    }
    if(max_val != 255){ // Cheking if the we have reached the correct end of the header or if there is some other data. If there is some other data then the file is corrupted.
        fclose(fh);
        return NULL;
    }
    image = newImage(width, height, max_val); // creating a new image struct to store all the image data.
    bytes_data = malloc(sizeof(uint8_t) * width * height * 3); // allocating space for byte array to read all the raw data.
    length = fread(bytes_data, sizeof(uint8_t), width * height * 3, fh); // Reading all the raw data.
    ch1 = fgetc(fh);
    if(length != width * height * 3 || ch1 != EOF){ // Checking if we have a short read or there is more data than specifed in the header. If that is the case then the file is corrupted.
        fclose(fh);
        return NULL;
    }
    int channelCount = 0;
    for (int i = 0; i < length; i++) { // adding data to the image struct.
        channelCount++;
        addChannel(image, bytes_data[i]);
        if (channelCount == 3) {
            // add alpha channel
            addChannel(image, 0xFF);
            channelCount = 0;
        }
    }
    free(bytes_data); // Freeing allocated data.
    fclose(fh); // closing the file.
    return image; // returnign the image structure.
}

bool writeP6(Image * image, char *fname){
    FILE * fh;
    int skip_byte = 3;
    int count = 0;
    uint8_t * bytes_data = malloc(image->width * image->height * 3); // allocating space to the array.

    for(int i = 0; i < image->length; i++){ // reading image data into the newly allocated array.
        if(i != 0 && i%skip_byte == 0){ // Check to skip the alpha channel values.
            skip_byte += 4;
        }
        else{
            bytes_data[count] = image->data[i];
            count++;
        }
    }
    fh = fopen(fname, "wb"); // Opening the file in write mode.
    // Writing the header to the file.
    fputc('P', fh);
    fputc('6', fh);
    fputc('\n', fh);
    fprintf(fh, "%d", image->width);
    fputc(' ', fh);
    fprintf(fh, "%d", image->height);
    fputc('\n', fh);
    fprintf(fh, "%d", 255);
    fputc('\n', fh);
    fwrite(bytes_data, sizeof(uint8_t), image->width * image->height * 3, fh); // Writing the actual data into the file.
    free(bytes_data); // Freeing allocated data.
    free(image); // Freeing image data.
    fclose(fh); // Closing the file.
    return true; // returing true once the all the data has been written.
}

Image * readP7(char* fname){
    FILE * fh = fopen(fname, "rb"); // Opening the P7 file in read mode.
    Image * image = NULL;
    uint8_t * bytes_data;
    int length;
    int count, i;
    int width = -1;
    int height = -1;
    int depth = -1;
    int max_val = -1;
    char ch1, ch2;
    char str[5];
    char str2[100];
    if (fh == NULL) // Checking if the file exists.
        return NULL;
    ch1 = fgetc(fh);
    ch2 = fgetc(fh);
    if(ch1 != 'P' && ch2 != '7'){ // Checking if the format is correct.
        fclose(fh); // If it is not we close the file;
        return NULL; // and return NULL;
    }
    // Proceed with the rest of the program if it is correct.
    ch1 = fgetc(fh);
    while((width == -1 || height == -1 || depth == -1 || max_val == -1) && ch1 != EOF){ // While we do not get the width, height, depth and max value of the file, we loop through this.
        ch1 = fgetc(fh);
        if(ch1 == '#'){ // Checking for comments.
            while(ch1 != '\n'){ // Getting to the end of the comment.
                ch1 = fgetc(fh);
            }
        }
        else{ // If it is not a comment.
            if(isdigit(ch1)){ // Checking if it is a digit
                count = 0;
                while(isdigit(ch1)){ // reading the entire number.
                    str[count] = ch1;
                    count++;
                    ch1 = fgetc(fh);
                }
                str[count] = '\0'; // adding the escape character at the end.
                if(width == -1){ // If width not read, read width.
                    width = atoi(str);
                }
                else if(width != -1 && height == -1){ // else if width is obtained read height.
                    height = atoi(str);
                }
                else if(width != -1 && height != -1 && depth == -1){ // else if width and height are obtained read depth.
                    depth = atoi(str);
                }
                else{ // else read max value.
                    max_val = atoi(str);
                }
            }
        }
    }
    count = 1;
    while (count == 1){ // Loop to check if we have reached the end of the header of the P7 file.
        i = 0;
        ch1 = fgetc(fh);
        while (ch1 != '\n'){ // Obtaining the entire string untill new line character occurs.
            str2[i] = ch1;
            i++;
            ch1 = fgetc(fh);
        }
        str2[i] = '\0';
        if(i == 7 && str2[0] == 'E' && str2[1] == 'N' && str2[2] == 'D' && str2[3] == 'H' && str2[4] == 'D' && str2[5] == 'R'){ // If the string is equal to ENHDR then the set the loop flag to 0.
            count = 0;
        }

    }
    image = newImage(width, height, max_val); // creating a new image struct to store all the image data.
    bytes_data = malloc(sizeof(uint8_t) * width * height * 4); // allocating space for byte array to read all the raw data.
    length = fread(bytes_data, sizeof(uint8_t), width * height * 4, fh); // Reading all the raw data.
    ch1 = fgetc(fh);
    if(length != width * height * 4 || ch1 != EOF){ // Checking if we have a short read or there is more data than specifed in the header. If that is the case then the file is corrupted.
        fclose(fh);
        return NULL;
    }
    for (i = 0; i < length; i++) { // Storing the data in image struct.
        addChannel(image, bytes_data[i]);
    }
    free(bytes_data); // Freeing allocated data.
    fclose(fh); // closing the file.
    return image; // returning the image structure.

}
