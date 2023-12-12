#include "helpers.h"
#include <math.h>
#include <stdbool.h>

/**
 * Converts image to grayscale.
*/
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float averageColor;
    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through every pixel on a row
        for(int j = 0; j < width; j++)
        {
            //calculates the average color of each pixel and then sets them all to that value
            averageColor = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            //updates given pixel
            image[i][j].rgbtBlue = round(averageColor);
            image[i][j].rgbtGreen = round(averageColor);
            image[i][j].rgbtRed = round(averageColor);
        }
    }
    return;
}

/**
 * Reflects image horizontally.
*/
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through half of the pixels on each row
        for(int j = 0; j < width/2; j++)
        {
            //updates index of pixel at the right end of the row
            int n = width - 1 - j;
            
            //temporarily stores the left end pixel
            RGBTRIPLE temp = image[i][j];
            //swaps the left end pixel with the right end pixel
            image[i][j] = image[i][n];
            //swaps the right end pixel with the stored left end pixel
            image[i][n] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    
    //Copies the original image into another structure
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through every pixel on a row
        for(int j = 0; j < width; j++)
        {
            int pixelCount = 0; //counter for total number of pixels in surrounding box
            //stores average color of the pixel
            int avgRed = 0;
            int avgGreen = 0;
            int avgBlue = 0;

            //nested loop to check the surrounding 3x3 "box" of pixels for averaging colors
            for(int row = -1; row <= 1; row++)
            {
                for(int col = -1; col <= 1; col++)
                {
                    //Current indices
                    int x = i + row;
                    int y = j + col;

                    //Checks if array will go out of bounds in either direction, if so then this pixel doesn't exist in the image
                    if(x < 0 || y < 0 || x > height - 1 || y > width - 1)
                    {
                        continue; //iterate the loop for the next pixel
                    }
                    //else the pixel is within bounds, so account for it in the average
                    else
                    {
                        pixelCount++;
                        avgRed += image[x][y].rgbtRed;
                        avgGreen += image[x][y].rgbtGreen;
                        avgBlue += image[x][y].rgbtBlue;
                    }

                }
            }

            //updates the pixel in the copy based on the colors of its surroundings
            copy[i][j].rgbtRed = round(avgRed / (float) pixelCount);
            copy[i][j].rgbtGreen = round(avgGreen / (float) pixelCount);
            copy[i][j].rgbtBlue = round(avgBlue / (float) pixelCount);
        }
    }

    //Transfers the blurred copy back into the original image
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}

//ALL BELOW - Edges

/**
 * Function to ensure the current color channel does not exceed the cap.
*/
int checkCap(int color){
    if(color > 255)
    {
        return 255;
    }
    else
    {
        return color;
    }
}

/**
 * Determines the Gx kernel multiplier based on the position of the pixel being checked in relation to the origin.
*/
int get_xMultiplier(int originRow, int originCol, int row, int col){
    //determines which value to use from Gx by checking the current column compared to the original pixel
    //column to the left of origin
    if(col < originCol)
    {
        //cases for each row

        //row is above the origin or beneath the origin
        if(row < originRow || row > originRow )
        {
            return -1;
        }
        //row of the origin
        else
        {
            return -2;
        }
    }
    //column to the right of origin
    else if(col > originCol)
    {
        //cases for each row

        //row is above the origin or beneath the origin
        if(row < originRow || row > originRow )
        {
            return 1;
        }
        //row of the origin
        else
        {
            return 2;
        }
    }
    //column of the origin
    else
    {
        return 0; //all values in this column are the same
    }
}

/**
 * Determines the Gy kernel multiplier based on the position of the pixel being checked in relation to the origin.
*/
int get_yMultiplier(int originRow, int originCol, int row, int col){
    //determines which value to use from Gx by checking the current row compared to the original pixel
    //row is above the origin
    if(row < originRow)
    {
        //cases for each column

        //column is to the left or right of the origin
        if(col > originRow || col < originRow )
        {
            return -1;
        }
        //row of the origin
        else
        {
            return -2;
        }
    }
    //row is beneath the origin
    else if(row > originRow)
    {
        //cases for each row

        //column is to the left or right of the origin
        if(col > originRow || col < originRow )
        {
            return 1;
        }
        //row of the origin
        else
        {
            return 2;
        }
    }
    //row of the origin
    else
    {
        return 0; //all values on this row are the same
    }
}

/**
 * Detects edges of an image.
*/
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    
    //Copies the original image into another structure
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through every pixel on a row
        for(int j = 0; j < width; j++)
        {
            //variables for storing the Gx sum
            int gxRedSum, gxGreenSum, gxBlueSum;
            gxRedSum = gxGreenSum = gxBlueSum = 0;
            //variables for storing the Gy sum
            int gyRedSum, gyGreenSum, gyBlueSum;
            gyRedSum = gyGreenSum = gyBlueSum = 0;

            //nested loop to check the surrounding 3x3 "box" of pixels for Gx & Gy channel sums
            for(int row = -1; row <= 1; row++)
            {
                bool edge = false; //flag to determine if the given pixel is on the edge of the image

                for(int col = -1; col <= 1; col++)
                {
                    //Current indices
                    int x = i + row;
                    int y = j + col;

                    //Checks if array will go out of bounds in either direction, if so then the origin pixel is on the edge of the image
                    if(x < 0 || y < 0 || x > height - 1 || y > width - 1)
                    {
                        //consider all color channels as black for this pixel, so nothing to add (+0)
                        continue;
                    }
                    //else the pixel is within bounds, so account for it in the sum
                    else
                    {
                        int xMultiplier = get_xMultiplier(i, j, x, y);
                        gxRedSum += image[x][y].rgbtRed * xMultiplier;
                        gxGreenSum += image[x][y].rgbtGreen * xMultiplier;
                        gxBlueSum += image[x][y].rgbtBlue * xMultiplier;

                        int yMultiplier = get_yMultiplier(i, j, x, y);
                        gyRedSum += image[x][y].rgbtRed * yMultiplier;
                        gyGreenSum += image[x][y].rgbtGreen * yMultiplier;
                        gyBlueSum += image[x][y].rgbtBlue * yMultiplier;
                    }

                }

                //if the origin pixel was determined to be on the edge
                //then stop checking surrounding channel sums
                if(edge)
                {
                    break; //move on to the next pixel
                }
            }

            //calculates the square root of Gx^2 + Gy^2 for each color channel
            int red = round(sqrt((float) pow(gxRedSum, 2) + pow(gyRedSum, 2)));
            int green = round(sqrt((float) pow(gxGreenSum, 2) + pow(gyGreenSum, 2)));
            int blue = round(sqrt((float) pow(gxBlueSum, 2) + pow(gyBlueSum, 2)));

            //ensures channels do not exceed the cap
            red = checkCap(red);
            green = checkCap(green);
            blue = checkCap(blue);

            //updates the pixel in the copy accordingly
            copy[i][j].rgbtRed = red;
            copy[i][j].rgbtGreen = green;
            copy[i][j].rgbtBlue = blue;
        }
    }

    //Transfers the blurred copy back into the original image
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}
