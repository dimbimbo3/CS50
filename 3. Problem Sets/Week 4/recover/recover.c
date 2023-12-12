#include <stdio.h>
#include <stdlib.h> //malloc
#include <stdint.h> //uint8_t data type
#include <stdbool.h> //bool data type

#define BLOCK_SIZE 512 //size (bytes) of each memory block
#define IMG_AMOUNT 50 //there are 50 jpgs on the memory card
#define NAME_MAX 8 //###.jpg + 1 for null terminator

typedef uint8_t BYTE; //8bits = 1 Byte

int main(int argc, char* argv[])
{
    //Ensures proper command line arguments were entered
    if(argc != 2){
        printf("Usage: ./recover card.raw\n");
        return 1; //error
    }

    //Opens file for reading
    FILE *input = fopen(argv[1], "r");
    if(input == NULL)
    {
        printf("Could not open file.\n");
        return 1; //error
    }

    BYTE buffer [BLOCK_SIZE]; //to store the image read in (512 bytes)
    char fileName[NAME_MAX]; //to store each file name iteration

    FILE *output = NULL; //pointer to output file
    int counter = 0; //counter for each image file

    bool reading = false; //flag to determine if a file is currently being read in
    //loop continues to read in 512-byte blocks until the end of the memory card
    while(fread(&buffer, BLOCK_SIZE, 1, input))
    {
        //checks if a jpg signature is found (marking the beginning or end of an image)
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >= 0xe0 && buffer[3] <= 0xef))
        {   
            //checks if an image is currently being read in (meaning this signature marks the end of that file)
            if(reading)
            {
                fclose(output); //close previous jpg file
            }

            sprintf(fileName, "%03i.jpg", counter); //stores a 3-digit integer with leading zeroes as the jpg name
            output = fopen(fileName, "w"); //pointer to new file for writing under the incremented name
            //error handling during file creation
            if (output == NULL)
            {
                fclose(input);
                printf("Could not create %s.\n", fileName);
                return 1; //error
            }

            reading = true; //sets flag to indicate that a new jpg is currently being read in
            counter++; //increment counter for next filename
        }

        //writes to the given output file if a signature has already been found
        if(reading)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, output);
        }
    }

    fclose(input); //closes the input file
    fclose(output); //closes the output file

    return 0; //success
}