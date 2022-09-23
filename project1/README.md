# CS 530 - Project 1

This is a P3 <-> P6 <-> P7 image converter.

## Authors

- Jadon Fowler
- Harshith Shakelli
 
## Usage

To build this project, we use a Makefile.
```
make
```

To convert an image, we can run the generated `ppmrw` executable:
```
./ppmrw <format> <inputFile> <outputFile>
```

where `format` is the format of the output file. The format
of the input file is automatically determined.

Example: converting a P6 file to a P3.
```
./ppmrw 3 ./p6image.ppm ./test/p3_output.ppm
```

## Known Issues

There are no known issues.
