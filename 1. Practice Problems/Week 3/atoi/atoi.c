//#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX 10 //allow integers up to 10 digits

int convert(char input[], int n);

int main(void)
{
    int buffer = MAX + 1;
    char input[buffer];

    int n = 0;
    do{
        printf("Enter a positive integer: ");
        if(!fgets(input, buffer, stdin))
        {
            printf("Input error has occured! Terminating...");
            return 1;
        }
        //else if the input was correct and less than the buffer there should be a new line character
        else
        {
            char* newlinePtr = strchr(input, '\n'); //stores the address of the new line character
            *newlinePtr = '\0'; //replace the newline character at the pointer's address with a null terminator for strlen
        }

        n = strlen(input); //store the length of the array (also the index of null terminator)
    }
    while(n == 0); //loops while the string is empty

    //ensures that every character in the string was a digit
    for (int i = 0; i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input, n));
}

/**
 * Converts each digit in the given string to an integer
*/
int convert(char input[], int n)
{
    //subtract the ascii value of the last digit by 0's
    int last = (int) input[n-1] - '0';

    //base case
    if(n == 1){
        return last;
    }

    //reconstruct the full number (right to left) by adding the current converted digit to the next order of magnitude's (10 *) converted digit
    return last + (10 * convert(input, n-1));
}

/*
Enter a positive integer: -100
Invalid Input!
*/

/*
Enter a positive integer: 1572896
1572896
*/