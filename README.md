This is the code for the first lab-- CS551 :: PPM Image Transformations

Learning Objectives

Upon completion of this assignment, you should be able:
1. To develop, compile, run and test C programs in a Linux environment 
2. To navigate Linux command lines reliably

The mechanisms you will practice using include:
• Linux command lines: manual pages, Linux commands
• C Programming: structs, pointers, memory allocation, getopt

Program Specification

NAME
ppmcvt – convert ppm files

SYNOPSIS
ppmcvt [bg:i:r:smt:n:o:] file

DESCRIPTION
ppmcvt manipulates input Portable Pixel Map (PPM) files and outputs a new image based on its given options. Only one option that specifies a transformation can be used at a time.

In the synopsis, options followed by a ‘:’ expect a subsequent parameter. The options are:
-b convert input file to a Portable Bitmap (PBM) file. (DEFAULT)

-g: convert input file to a Portable Gray Map (PGM) file using the specified max grayscale
pixel value [1-65535]. 

-i: isolate the specified RGB channel. Valid channels are “red”, “green”, or “blue”.  

-r: remove the specified RGB channel. Valid channels are “red”, “green”, or “blue”.

-s apply a sepia transformation

-m vertically mirror the first half of the image to the second half 

-t:reduce the input image to a thumbnail based on the given scaling factor [1-8].

-n: tile thumbnails of the input image based on the given scaling factor [1-8].

-o: write output image to the specified file. Existent output files will be overwritten.

