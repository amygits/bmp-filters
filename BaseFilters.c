/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

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

//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

struct BMP_Header* bmpheader;
struct DIB_Header* dibheader;
struct Pixel** pixelBody;




////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

int main(int argc, char** argv) {
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
    
    
        while((opt = getopt(argc, argv, ":i:o:f:")) != -1){
        switch(opt) {
            case 'i':
                inputfile = optarg;
                printf("filename: %s\n", inputfile);
                break;
            case ':':
                printf("Please enter a file name\n");
                break;
            case 'o':
                oFlag = 1;
                outputfile = optarg;
                printf("output filename: %s\n", outputfile);
                break;
            case 'f':
                fFlag = 1;
                filterType = optarg;
                printf("filter type: %s\n", filterType);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }
    
    printf("oFlag = %d, fFlag = %d\n", oFlag, fFlag);
   
   for (; optind < argc; optind++){
        printf("Given extra arguments: %s\n", argv[optind]);
    }
    
    int dflen;
    int otlen;
    // If inputfile is valid
    if(inputfile != NULL){
        dflen = strlen(inputfile);
        if (dflen >= 5 
                && ((strcmp(&inputfile[dflen - 4], ".bmp") == 0))
                && (access(inputfile, F_OK) != -1)) {
            printf("Opening file %s\n", inputfile);
            FILE* fileImport = fopen(inputfile, "r");
            printf("Opened\n");
            printf("Attempting read\n");
            // if bmp, performs read actions
            if (strcmp(&inputfile[dflen - 4], ".bmp") == 0) {
                printf("Attempting BMP Header read\n");
                readBMPHeader(fileImport, bmpheader);
                printf("BMP Header read success\n");
                printf("Attempting DIB Header read\n");
                readDIBHeader(fileImport, dibheader);
                printf("DIB Header read success\n");
                
                width = dibheader->width;
                height = dibheader->height;
                printf("image size: %d x %d\n", width, height);
                
                pixelBody = (struct Pixel**) malloc(width * sizeof(*pixelBody));
                for(int x = 0; x < width; x++){
                    pixelBody[x] = (struct Pixel*) malloc(height * sizeof(struct Pixel));
                }
                
                printf("Attempting pixel read\n");
                readPixelsBMP(fileImport, pixelBody, width, height);
                printf("Pixel read success\n");
            } 
            
            // if f is flagged
            if (fFlag == 1) {
                printf("Beginning filter\n");
                boxBlur(pixelBody, width, height);
             /*if (strcmp(&filterType, "b") == 0) {
                 boxBlur(pixelBody, width, height);
             }*/
                printf("Filter complete\n");
            }
            
            // if output file name is flagged
            if (oFlag == 1){
                printf("Beginning file export\n");
                otlen = strlen(outputfile);
                printf("output filename: %s\nlength: %d\n", outputfile, otlen);
                if (otlen == 0){
                    outputfile = strcat(inputfile, "-copy");
                    printf("output filename: %s\n", outputfile);
                }
                
                FILE* fileExport = fopen(outputfile, "w");
                writeBMPHeader(fileExport, bmpheader);
                writeDIBHeader(fileExport, dibheader);
                writePixelsBMP(fileExport, pixelBody, width, height);
                printf("File export complete\n");
            }
            //
            printf("closing file %s\n", inputfile);
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