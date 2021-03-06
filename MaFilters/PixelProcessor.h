/**
* A utility functions for doing pixel transformations.
*
* Completion time: 8 hours
*
* @author Amy Ma
* @version 2.0.9.12.21
*/


#ifndef PixelProcessor_H
#define PixelProcessor_H 1
#include <stddef.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

static bool corner1_pass = false;
static bool corner2_pass = false;
static bool corner3_pass = false;
static int holes = 0;

struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);

/* Computes and applies box blur algorithm for each pixel in pixel array
 * @param struct Pixel** pArr(double pointer to pixel array), int startX (starting x coordinate), int startY (starting Y coordinate)
 * @param int width, int height
 */
void boxBlur(struct Pixel** pArr, int startX, int startY, int width, int height){
    
    printf("Starting box blur..\n");
    int x;
    int y;
    int r, g, b;
    
    for (x = startX; x < width; x++) {
        for (y = startY; y < height; y++) {
            r = 0;
            g = 0;
            
            // edge case 1
            if (x == startX && (corner1_pass == false || corner2_pass == false)) {
                //printf("EDGE CASE 1\n");
                //printf("y: %d  height-1: %d\n", y, height-1);
                // corner case
                if (y == startY && corner1_pass == false) {
                    //printf("CORNER CASE 1 \n");
                    //printf("y: %d  height-1: %d\n", y, height-1);
                    r = (pArr[x][y].red + pArr[x+1][y].red + pArr[x][y+1].red + pArr[x+1][y+1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x+1][y].blue + pArr[x][y+1].blue + pArr[x+1][y+1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x+1][y].green + pArr[x][y+1].green + pArr[x+1][y+1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    corner1_pass = true;
                    continue;
                }
                // corner case
                
                if (y == height - 1 && corner2_pass == false) {
                    //printf("CORNER CASE 2\n");
                    //printf("y: %d  height-1: %d\n", y, height-1);
                    r = (pArr[x][y].red + pArr[x+1][y].red + pArr[x][y-1].red + pArr[x+1][y-1].red) / 4;
                    //printf("red blur adjust for pixel %d, %d: %d\n", x, y, r);
                    b = (pArr[x][y].blue + pArr[x+1][y].blue + pArr[x][y-1].blue + pArr[x+1][y-1].blue) / 4;
                    //printf("blue blur adjust for pixel %d, %d: %d\n", x, y, b);
                    g = (pArr[x][y].green + pArr[x+1][y].green + pArr[x][y-1].green + pArr[x+1][y-1].green) / 4;
                    //printf("green blur adjust for pixel %d, %d: %d\n", x, y, g);
                    pArr[x][y].red = r;
                    pArr[x][y].green = g;
                    pArr[x][y].blue = b;
                    corner2_pass = true;
                    continue;
                }
                
                //printf("normal edge case run\n");
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
                    //printf("CORNER CASE 3\n");
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
                    //printf("CORNER CASE 4\n");
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
            else if (y == startY) {
                //printf("Edge case 3\n");
                if (x == startX) {
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


/* makes a "hole" in an image pixel body 
 * at (x,y) x (x,y) coordinates 
 * given center of specified radius 
 */
void swissCheese(struct Pixel** pArr, int width, int height, int radius, int centerX, int centerY) {
    //printf("Swiss cheese filter starting..\n");
    //printf("width: %d, height: %d, radius: %d, center: (%d, %d)\n", width, height, radius, centerX, centerY);
    
    int x, y;
    unsigned char hole = 0;
    int xStart = centerX - radius;
    int yStart = centerY - radius;
    if (xStart < 0){
        xStart = 0;
    }
    if (yStart < 0){
        yStart = 0;
    }
    // makes a "hole" in image
    for (x = xStart; x < (centerX + radius) && (x < width); x++){
        //printf("entering loop %d\n", x);
        for(y = yStart; y < (centerY + radius) && (y < height); y++){
            //printf("entering loop %d\n", y);
            if (pow(x-centerX, 2) + pow(y-centerY, 2)< pow(radius, 2)){
                pArr[x][y].red = hole;
                pArr[x][y].green = hole;
                pArr[x][y].blue = hole;
            }
        }
    }
    holes++;
    //printf("Swiss cheese filter success\n");
}


#endif