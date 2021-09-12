/**
* A utility functions for doing pixel transformations.
*
* Completion time: -1 minutes
*
* @author Amy Ma
* @version 2.0.9.12.21
*/


#ifndef PixelProcessor_H
#define PixelProcessor_H 1
#include <stddef.h>
#include <math.h>

struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);


void boxBlur(struct Pixel** pArr, int width, int height){
    
    printf("Starting box blur..\n");
    int x;
    int y;
    int r, g, b;
    
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            r = 0;
            g = 0;
            b = 0;
            
            // edge case 1
            if (x == 0) {
                //printf("EDGE CASE 1\n");
                // corner case
                if (y == 0) {
                    //printf("CORNER CASE\n");
                    r = (pArr[x][y].red + pArr[x+1][y].red + pArr[x][y+1].red + pArr[x+1][y+1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x+1][y].blue + pArr[x][y+1].blue + pArr[x+1][y+1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x+1][y].green + pArr[x][y+1].green + pArr[x+1][y+1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    continue;
                }
                // corner case
                if (y == height - 1) {
                    //printf("CORNER CASE\n");
                    r = (pArr[x][y].red + pArr[x+1][y].red + pArr[x][y-1].red + pArr[x+1][y-1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x+1][y].blue + pArr[x][y-1].blue + pArr[x+1][y-1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x+1][y].green + pArr[x][y-1].green + pArr[x+1][y-1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    continue;
                }
                
                r = (pArr[x][y].red + pArr[x][y+1].red + pArr[x][y-1].red 
                        + pArr[x+1][y].red + pArr[x+1][y+1].red + pArr[x+1][y-1].red) / 6;
                //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                b = (pArr[x][y].blue + pArr[x][y+1].blue + pArr[x][y-1].blue 
                        + pArr[x+1][y].blue + pArr[x+1][y+1].blue + pArr[x+1][y-1].blue) / 6;
                //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                g = (pArr[x][y].green + pArr[x][y+1].green + pArr[x][y-1].green 
                        + pArr[x+1][y].green + pArr[x+1][y+1].green + pArr[x+1][y-1].green) / 6;
                //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                pArr[x][y].red = r;
                pArr[x][y].green = g;
                pArr[x][y].blue = b;
            } 
            // edge case 2
            else if (x == width - 1) {
                 //printf("EDGE CASE 2\n");
                // corner case
                if (y == 0) {
                    //printf("CORNER CASE\n");
                    r = (pArr[x][y].red + pArr[x-1][y].red + pArr[x][y+1].red + pArr[x-1][y+1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x-1][y].blue + pArr[x][y+1].blue + pArr[x-1][y+1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x-1][y].green + pArr[x][y+1].green + pArr[x-1][y+1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    continue;
                }
                // corner case
                if (y == height - 1) {
                    //printf("CORNER CASE\n");
                    r = (pArr[x][y].red + pArr[x-1][y].red + pArr[x][y-1].red + pArr[x-1][y-1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x-1][y].blue + pArr[x][y-1].blue + pArr[x-1][y-1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x-1][y].green + pArr[x][y-1].green + pArr[x-1][y-1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    continue;
                }
                
                r = (pArr[x][y].red + pArr[x][y+1].red + pArr[x][y-1].red 
                        + pArr[x-1][y].red + pArr[x-1][y+1].red + pArr[x-1][y-1].red) / 6;
                //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                b = (pArr[x][y].blue + pArr[x][y+1].blue + pArr[x][y-1].blue 
                        + pArr[x-1][y].blue + pArr[x-1][y+1].blue + pArr[x-1][y-1].blue) / 6;
                //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                g = (pArr[x][y].green + pArr[x][y+1].green + pArr[x][y-1].green 
                        + pArr[x-1][y].green + pArr[x-1][y+1].green + pArr[x-1][y-1].green) / 6;
                //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                pArr[x][y].red = r;
                pArr[x][y].green = g;
                pArr[x][y].blue = b;
                
            }
            // edge case 3
            else if (y == 0) {
                //printf("Edge case 3\n");
                if (x == 0) {
                    //printf("Corner case repeat\n");
                    continue;
                }
                if (x == width - 1) {
                    //printf("Corner case repeat\n");
                    continue;
                }
                
                r = (pArr[x][y].red + pArr[x-1][y].red + pArr[x+1][y].red
                        + pArr[x][y+1].red + pArr[x-1][y+1].red + pArr[x+1][y+1].red) / 6;
                //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                b = (pArr[x][y].blue + pArr[x-1][y].blue + pArr[x+1][y].blue
                        + pArr[x][y+1].blue + pArr[x-1][y+1].blue + pArr[x+1][y+1].blue) / 6;
                //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                g = (pArr[x][y].green + pArr[x-1][y].green + pArr[x+1][y].green
                        + pArr[x][y+1].green + pArr[x-1][y+1].green + pArr[x+1][y+1].green) / 6;
                //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
            }
            // EDGE CASE 4
            else if (y == height - 1) {
                //printf("Edge case 4\n");
                if (x == 0) {
                    //printf("Corner case repeat\n");
                    continue;
                }
                if (x == width - 1) {
                    //printf("Corner case repeat\n");
                    continue;
                }
                
                r = (pArr[x][y].red + pArr[x-1][y].red + pArr[x+1][y].red
                        + pArr[x][y-1].red + pArr[x-1][y-1].red + pArr[x+1][y-1].red) / 6;
                //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                b = (pArr[x][y].blue + pArr[x-1][y].blue + pArr[x+1][y].blue
                        + pArr[x][y-1].blue + pArr[x-1][y-1].blue + pArr[x+1][y-1].blue) / 6;
                //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                g = (pArr[x][y].green + pArr[x-1][y].green + pArr[x+1][y].green
                        + pArr[x][y-1].green + pArr[x-1][y-1].green + pArr[x+1][y-1].green) / 6;
                //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
            } 
            // Normal case  x3
            else {
                r = (pArr[x][y].red + pArr[x][y+1].red + pArr[x][y-1].red
                        + pArr[x+1][y].red + pArr[x-1][y].red 
                        + pArr[x-1][y-1].red + pArr[x+1][y-1].red + pArr[x-1][y+1].red 
                        + pArr[x+1][y+1].red)
                        / 9;
                //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                g = (pArr[x][y].green + pArr[x][y+1].green + pArr[x][y-1].green
                        + pArr[x+1][y].green + pArr[x-1][y].green 
                        + pArr[x-1][y-1].green + pArr[x+1][y-1].green + pArr[x-1][y+1].green 
                        + pArr[x+1][y+1].green)
                        / 9;
                //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                b = (pArr[x][y].blue + pArr[x][y+1].blue + pArr[x][y-1].blue
                        + pArr[x+1][y].blue + pArr[x-1][y].blue 
                        + pArr[x-1][y-1].blue + pArr[x+1][y-1].blue + pArr[x-1][y+1].blue 
                        + pArr[x+1][y+1].blue)
                        / 9;
                //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
            }
        }
    }
    
    printf("Box blur success\n");
}



void swissCheese(struct Pixel** pArr, int width, int height) {
    
    /*int radius;
    int centerX, centerY;
    centerX = width-1 / 2;
    centerY = height-1 / 2;
    int x, y;
    unsigned char hole = 0;
    
    // makes a "hole" in image
    for (x = (centerX - radius); x <= (centerX + radius); x++){
        for(y = (centerY - radius); y <= (centerY + radius); y++){
            if (pow(x-centerX, 2) + pow(y-centerY, 2)<= pow(radius, 2)){
                pArr[x][y].red = hole;
                pArr[x][y].green = hole;
                pArr[x][y].blue = hole;
            }
        }
    }*/
}


#endif