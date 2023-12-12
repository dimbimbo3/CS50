#include "helpers.h"
#include <math.h>

// Convert image to grayscale
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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int originalRed, originalGreen, originalBlue;
    int sepiaRed, sepiaGreen, sepiaBlue;
    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through every pixel on a row
        for(int j = 0; j < width; j++){
            //stores original color values for this pixel
            originalRed = image[i][j].rgbtRed;
            originalGreen = image[i][j].rgbtGreen;
            originalBlue = image[i][j].rgbtBlue;

            //calculates each new sepia value for this pixel
            sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            if(sepiaRed > 255){
                sepiaRed = 255;
            }
            sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            if(sepiaGreen > 255){
                sepiaGreen = 255;
            }
            sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            if(sepiaBlue > 255){
                sepiaBlue = 255;
            }

            //updates given pixel
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
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
