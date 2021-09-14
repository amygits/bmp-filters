/**
 * This source file contains the main function for a threaded program that
 * Loads an image(.bmp) file
 * Applies a filter to it
 * Saves it to disk
 *
 * Completion time: 10 hours
 *
 * @author Amy Ma
 * @version 3.0.09.13.21
 */

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

//TODO: finish me

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
#include "BmpProcessor.h"
#include "PixelProcessor.h"
#include <unistd.h>
#include <getopt.h>
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096

int const THREAD_COUNT = 4;

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

struct BMP_Header* bmpheader;
struct DIB_Header* dibheader;
struct Pixel** pixelBody;

struct blurArgs {
    struct Pixel** pArr;
    int startX, endX;
    int startY, endY;
};

struct cheeseArgs {
    struct Pixel** pArr;
    int endX, endY, startX, startY;
};

void* boxBlurThread(void *arg) {

    printf("entering box blur thread\n");
    struct blurArgs* args = arg;
    printf("thread args - startX: %d, endX: %d, startY: %d, endY: %d\n", args->startX, args->endX, args->startY, args->endY);
    boxBlur(args->pArr, args->startX, args->startY, args->endX, args->endY);
    printf("success - exiting box blur thread\n");
    pthread_exit(0);
    return NULL;
}

void* cheeseThread(void *arg) {

    printf("entering swiss cheese thread\n");
    struct cheeseArgs* args = arg;
    
    int shortSide, radius, xcenter, ycenter, counter;
    shortSide = args->endX;
    if (args->endY < shortSide) {
        shortSide = args->endY;
    }
    
    int maxHoles = (floor(shortSide * .1));
    counter = 0;
    // .01 - .20 (1-20% of shortest side)
    //double range = (.20-.05);
    while (holes < maxHoles) {
        double ratioRange = (.15 - .05);
        double ratioDiv = RAND_MAX / ratioRange;
        double randRatio = .05 + (rand() / ratioDiv);
        radius = floor(shortSide * randRatio);

        double xRange = ((args->endX - 1) - args->startX);
        double xDiv = RAND_MAX / xRange;
        xcenter = floor(1 + (rand() / xDiv));

        double yRange = ((args->endY - 1) - args->startY);
        double yDiv = RAND_MAX / yRange;
        ycenter = floor(1 + (rand() / yDiv));
        printf("thread args startx: %d, starty: %d, endx: %d, endy: %d, radius: %d, center: (%d, %d)\n", args->startX, args->startY, args->endX, args->endY, radius, xcenter, ycenter);
        swissCheese(pixelBody, args->endX, args->endY, radius, xcenter, ycenter);
        //counter++;
    }
    
    //printf("thread args - width: %d, height: %d, radius: %d, center: (%d, %d)\n", args->width, args->height, args->radius, args->centerX, args->centerY);
    //swissCheese(args->pArr, args->width, args->height, args->radius, args->centerX, args->centerY);
    
    printf("success - exiting swiss cheese thread\n");
    pthread_exit(0);
    return NULL;
    
}


////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

int main(int argc, char** argv) {

    pthread_t tid;

    srand(time(0));

    bmpheader = malloc(sizeof*(bmpheader));
    dibheader = malloc(sizeof*(dibheader));

    char* inputfile;
    char* outputfile;
    char* filterType;

    int height = 0;
    int width = 0;

    int opt;

    int oFlag = 0;
    int fFlag = 0;


    while ((opt = getopt(argc, argv, ":i:o:f:")) != -1) {
        switch (opt) {
            case 'i':
                inputfile = optarg;
                //printf("filename: %s\n", inputfile);
                break;
            case ':':
                printf("Please enter a file name\n");
                break;
            case 'o':
                oFlag = 1;
                outputfile = optarg;
                //printf("output filename: %s\n", outputfile);
                break;
            case 'f':
                fFlag = 1;
                filterType = optarg;
                if (strcmp(filterType, "b") == 0 ||
                        strcmp(filterType, "B") == 0 ||
                        strcmp(filterType, "c") == 0 ||
                        strcmp(filterType, "C") == 0) {
                    break;
                }//printf("filter type: %s\n", filterType);
                else {
                    printf("Unknown filter type\n");
                    exit(0);
                }
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }

    //printf("oFlag = %d, fFlag = %d\n", oFlag, fFlag);

    for (; optind < argc; optind++) {
        printf("Given extra arguments: %s\n", argv[optind]);
    }

    int dflen;
    int otlen;
    // If inputfile is valid
    if (inputfile != NULL) {
        dflen = strlen(inputfile);
        if (dflen >= 5
                && ((strcmp(&inputfile[dflen - 4], ".bmp") == 0))
                && (access(inputfile, F_OK) != -1)) {
            printf("Opening file %s\n", inputfile);
            FILE* fileImport = fopen(inputfile, "r");
            //printf("Opened\n");
            printf("Attempting reads (0/3)..\n");
            // if bmp, performs read actions
            if (strcmp(&inputfile[dflen - 4], ".bmp") == 0) {
                //printf("Attempting BMP Header read\n");
                readBMPHeader(fileImport, bmpheader);
                printf("BMP Header read success\n");
                //printf("Attempting DIB Header read\n");
                readDIBHeader(fileImport, dibheader);
                printf("DIB Header read success\n");


                width = dibheader->width;
                height = dibheader->height;
                //printf("image size: %d x %d\n", width, height);

                pixelBody = (struct Pixel**) malloc(width * sizeof (*pixelBody));
                for (int x = 0; x < width; x++) {
                    pixelBody[x] = (struct Pixel*) malloc(height * sizeof (struct Pixel));
                }

                //printf("Attempting pixel read..\n");
                readPixelsBMP(fileImport, pixelBody, width, height);
                printf("Pixel read success\n");
                printf("Read success (3/3)\n");
            }

            // if f is flagged ///////////////////////////////////////////
            if (fFlag == 1) {
                
                // If c is flagged - execute swiss cheese filter /////////////////
                if (strcmp(filterType, "c") == 0 ||
                        strcmp(filterType, "C") == 0) {
                    
                    printf("Starting filter..\n");
                    int i;
                    struct cheeseArgs *chargs = (struct cheeseArgs*) malloc(sizeof (struct cheeseArgs));
                    int newWidth = round(width / THREAD_COUNT);
                    for (i = 0; i < THREAD_COUNT; i++) {
                        chargs->pArr = pixelBody;
                        printf("thread: pixels added\n");
                        chargs->startX = 0;
                        printf("thread: startX added: %d\n", chargs->startX);
                        chargs->endX = width;
                        printf("thread: endX added: %d\n", chargs->endX);
                        chargs->startY = 0;
                        printf("thread: startY added: %d\n", chargs->startY);
                        chargs->endY = height;
                        printf("thread: endY added: %d\n", chargs->endY);
                        pthread_create(&tid, NULL, cheeseThread, (void*) chargs);
                        pthread_join(tid, NULL);
                   }
                    printf("Swiss cheese filter success\n");
                }
                
                // if b is flagged, execute blur ///////////////////////////////////
                
                if (strcmp(filterType, "b") == 0 ||
                        strcmp(filterType, "B") == 0) {
                    int i;
                    struct blurArgs *blargs = (struct blurArgs*) malloc(sizeof (struct blurArgs));
                    int newWidth = round(width / THREAD_COUNT);
                    //printf("new width: %d\n", newWidth);
                    if (newWidth <= 1){
                        newWidth = 1;
                    }
                    printf("new width: %d\n", newWidth);
                    // IF size of new width is less than thread count **
                    if (newWidth < THREAD_COUNT){
                        for (i = 0; i < width; i++) {
                        blargs->pArr = pixelBody;
                        //printf("thread: pixels added\n");
                        blargs->startX = i;
                        //printf("thread: startX added: %d\n", blargs->startX);
                        blargs->endX = (i + 1);
                        //printf("thread: endX added: %d\n", blargs->endX);
                        blargs->startY = 0;
                        //printf("thread: startY added: %d\n", blargs->startY);
                        blargs->endY = height;
                        //printf("thread: endY added: %d\n", blargs->endY);
                        pthread_create(&tid, NULL, boxBlurThread, (void*) blargs);
                        pthread_join(tid, NULL);
                        }
                        
                    free(blargs);
                    blargs = NULL;
                    }
                
                else {
                    for (i = 0; i < THREAD_COUNT; i++) {
                        blargs->pArr = pixelBody;
                        //printf("thread: pixels added\n");
                        blargs->startX = newWidth * i;
                        //printf("thread: startX added: %d\n", blargs->startX);
                        blargs->endX = (i + 1) * newWidth;
                        //printf("thread: endX added: %d\n", blargs->endX);
                        blargs->startY = 0;
                        //printf("thread: startY added: %d\n", blargs->startY);
                        blargs->endY = height;
                        //printf("thread: endY added: %d\n", blargs->endY);
                        pthread_create(&tid, NULL, boxBlurThread, (void*) blargs);
                        pthread_join(tid, NULL);
                    }
                    free(blargs);
                    blargs = NULL;
                }
            }
        }

            // if output file name is flagged
            if (oFlag == 1) {
                printf("Beginning file export\n");
                otlen = strlen(outputfile);
                //printf("output filename: %s\nlength: %d\n", outputfile, otlen);
                if (otlen == 0) {
                    outputfile = strcat(inputfile, "-copy");
                    printf("output filename: %s\n", outputfile);
                }

                FILE* fileExport = fopen(outputfile, "w");
                writeBMPHeader(fileExport, bmpheader);
                writeDIBHeader(fileExport, dibheader);
                writePixelsBMP(fileExport, pixelBody, width, height);
                fclose(fileExport);
                printf("File export complete\n");
            }
            //
            printf("closing input file %s\n", inputfile);
            fclose(fileImport);

            free(bmpheader);
            bmpheader = NULL;
            free(dibheader);
            dibheader = NULL;
            free(pixelBody);
            pixelBody = NULL;


        } else {
            printf("Data file has an invalid name or does not exist\n");
            //print_usage();
            exit(1);
        }
    } else {
        printf("No data file name provided.  This is a required field\n");
        //print_usage();
        exit(1);
    }

    return (EXIT_SUCCESS);
}


