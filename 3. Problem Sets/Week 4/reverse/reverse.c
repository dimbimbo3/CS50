#include <stdint.h> //uint8_t (1 byte)
#include <stdio.h> //printf, file IO

#include "wav.h" //WAVHEADER struct

#define HEADER_SIZE 44 //a wav header is 44bytes

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char* argv[])
{
    // Ensure proper usage
    if(argc != 3){
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE* inPtr = fopen(argv[1], "r");
    if(inPtr == NULL){
        printf("Error when opening file. Terminating...\n");
        return 1;
    }

    // Read header
    WAVHEADER iHeader;
    fread(&iHeader, HEADER_SIZE, 1, inPtr); //stores the first 44-bytes of the input file
    int startPos = ftell(inPtr); //stores the end of the header and, thus, starting position of audio data

    // Use check_format to ensure WAV format
    if(check_format(iHeader) != 1){
        printf("Input is not a proper wav file. Terminating...\n");
        return 1;
    }

    // Open output file for writing
    FILE* outPtr = fopen(argv[2], "w");
    if(outPtr == NULL){
        printf("Error creating '%s'. Terminating...\n", argv[2]);
        return 1;
    }

    // Write header to output file
    fwrite(&iHeader, HEADER_SIZE, 1, outPtr); //writes 44-byte header to the output file

    // Use get_block_size to calculate size of each block of audio
    const int BLOCK_SIZE = get_block_size(iHeader);

    uint8_t buffer[BLOCK_SIZE]; //initialize buffer for reading in each audio block from the input file

    fseek(inPtr, -BLOCK_SIZE, SEEK_END); //moves input pointer to the end of the file and then goes backwards a block

    // Write reversed audio to output file
    while(fread(&buffer, BLOCK_SIZE, 1, inPtr))
    {
        fwrite(&buffer, BLOCK_SIZE, 1, outPtr); //write a block of audio data to the output file

        fseek(inPtr, -BLOCK_SIZE, SEEK_CUR); //moves input pointer from its current position back a block (to the start of the block just read)

        //if the pointer has returned to the start of the audio data then end the loop
        if(ftell(inPtr) == startPos)
        {
            break;
        }
        //else move the pointer back another block for further reading/writing
        else
        {
            fseek(inPtr, -BLOCK_SIZE, SEEK_CUR); //moves input pointer from its current position back a block (to the block before the one just read)
        }
    }


    //Close both files
    fclose(inPtr);
    fclose(outPtr);
}

/**
 * Accepts a wav header and returns true or false based on if it's valid or not.
*/
int check_format(WAVHEADER header)
{
    int valid = 0; //flag indicating whether the header is valid (initialized to false)

    if(header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        valid = 1; //mark flag to true
    }

    return valid;
}

/**
 * Calculates the block size for the given wav header and returns the result.
*/
int get_block_size(WAVHEADER header)
{
    int bytesPerSample = (header.bitsPerSample / 8); //determines how many bytes are in each sample (8bits = 1byte)
    return header.numChannels * bytesPerSample;
}