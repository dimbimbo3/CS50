//#include <cs50.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //malloc

#define BUFFER 1024 //max allowable number of characters (including null terminator)

const int BITS_IN_BYTE = 8;

int* convert_to_binary(int);
void read_backwards(int*);
void print_bulb(int bit);

int main(void)
{
    char input[BUFFER];
    int* binaryPtr;
    int ascii;

    printf("Message: ");
    if(fgets(input, BUFFER, stdin) != NULL){
        for(int i = 0; input[i] != '\n'; i++){
            ascii = input[i];

            binaryPtr = convert_to_binary(ascii);
            read_backwards(binaryPtr);
            free(binaryPtr);

            printf("\n");
        }
    }
}

int* convert_to_binary(int val){
    int* binary = malloc(sizeof(int) * BITS_IN_BYTE); //a character is only 8 bits so allocate accordingly

    //loops to convert ascii value from decimal to binary
    for(int i = 0; i < BITS_IN_BYTE; i++){
        //if the ascii value is still greater than zero
        if(val > 0){
            binary[i] = val % 2; //store the remainder
            val /= 2; //divide by 2 to get the next bit
        }
        //else the ascii value has already been divided to 0
        else{
            binary[i] = 0; //prepends remaining 0s to the binary number
        }
    }

    return binary;
}

void read_backwards(int* binary){
    int startIndex = BITS_IN_BYTE - 1; //start from the end of the binary number

    //increment through each bit in the binary number right to left
    for(int i = startIndex; i >= 0; i--){
        print_bulb(binary[i]);
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        //printf("0");

        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        //printf("1");

        // Light emoji
        printf("\U0001F7E1");
    }
}

/*
Message: HI!
01001000
01001001
00100001
*/

/*
Message: HI!
⚫🟡⚫⚫🟡⚫⚫⚫
⚫🟡⚫⚫🟡⚫⚫🟡
⚫⚫🟡⚫⚫⚫⚫🟡
*/