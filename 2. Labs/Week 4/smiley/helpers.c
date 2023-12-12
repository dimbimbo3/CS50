#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    //loops through every row
    for(int i = 0; i < height; i++)
    {
        //loops through every pixel on a row
        for(int j = 0; j < width; j++){
            //if the pixel is black then change it's color to yellow
            if(image[i][j].rgbtRed == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtBlue == 0x00)
            {
                image[i][j].rgbtRed = 0xff;
                image[i][j].rgbtGreen = 0xea;
                image[i][j].rgbtBlue = 0x00;
            }
        }
    }
}
