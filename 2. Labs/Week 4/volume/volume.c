// Modifies the volume of an audio file

#include <stdint.h> //uint8_t, uint16_t
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char* argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    uint8_t iHeader[HEADER_SIZE]; //unsigned 8-bit int array to store input header (wav headers are composed of 44-bytes)

    fread(&iHeader, HEADER_SIZE, 1, input); //reads 44-bytes, 1 byte at a time, from the input file and stores in iHeader
    fwrite(&iHeader, HEADER_SIZE, 1, output); //writes 44-bytes, 1 byte at a time, from iHeader into the output file

    int16_t iBuffer; //signed 16-bit int to store each 2-byte sample from the input wav file

    //loops through every sample in the input file
    while(fread(&iBuffer, sizeof(int16_t), 1, input))
    {
        iBuffer = iBuffer * factor; //amplify the volume of each sample by the given factor
        fwrite(&iBuffer, sizeof(int16_t), 1, output); //writes the updated sample to the output file
    }

    // Close files
    fclose(input);
    fclose(output);
}

/*
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 4/volume$ ./volume input.wav output2x.wav 2.0
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 4/volume$ ./volume input.wav output1-4x.wav 0.25
*/