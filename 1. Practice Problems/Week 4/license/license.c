#include <stdio.h>
#include <string.h> //strcpy
#include <stdlib.h> //malloc

int main(int argc, char* argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./read infile\n");
        return 1;
    }

    // Create buffer to read plate into
    char buffer[7]; // 6 characters (+1 for newline/null terminator)

    // Create array to store each read plate number
    char *plates[8];

    //allocate memory for each pointer in the array
    for(int i = 0; i < 8; i++){
        plates[i] = malloc(sizeof(char) * 7); // 6 characters (+1 for newline/null terminator)
    }

    FILE *infile = fopen(argv[1], "r");

    int idx = 0; //index for plates array

    //loops while the returned number of characters from each line is 7 (includes the newline)
    while (fread(buffer, sizeof(char), 7, infile) == 7)
    {
        // Replace '\n' with '\0'
        buffer[6] = '\0';

        // Save plate number in array
        strcpy(plates[idx], buffer); //copy the buffer string into the plates array at the given index
        idx++; //increment index each loop
    }

    fclose(infile); //closes the file after reading it

    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", plates[i]);
        free(plates[i]); //free memory being used by this pointer
    }
}
