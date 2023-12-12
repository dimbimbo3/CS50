// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

//#include <cs50.h>
#include <stdio.h>
#include <ctype.h>  //toupper
#include <string.h> //strlen
#include <stdlib.h> //malloc

char* replaceVowels(int, char[]);

int main(int argc, char *argv[])
{
    //if the argument count is 1, then replace all vowels
    if(argc > 1){
        int length = strlen(argv[1]); //stores the length of the argument
        char noVowels[length+1]; //create a char array with the same size as the argument (+1 for null terminator)

        char* ptr = replaceVowels(length, argv[1]); //stores the string returned by the replace function
        strcpy(noVowels, ptr); //copies the string that is being pointed to into noVowels
        free(ptr); //frees the memory allocated by the pointer

        printf("%s\n", noVowels);//prints the coverted result

        return 0; //signifies success
    }
    else
    {
        printf("Error - Invalid number of arguments!\n");
        return 1; //signifies error
    }
}

/**
 * Replaces all vowels, except 'u', in the given word with their leetspeak equivalents.
*/
char* replaceVowels(int length, char arg[]){
    char* modified = malloc(sizeof(char) * length+1); //allocates memory up to the length of the argument (+1 for null terminator)

    //loops through each letter of the argument, replacing vowels, and stores each char of the new string into modified
    for(int i = 0; i < length; i++){
        switch(toupper(arg[i])){
            case 'A':
                modified[i] = '6';
                break;
            case 'E':
                modified[i] = '3';
                break;
            case 'I':
                modified[i] = '1';
                break;
            case 'O':
                modified[i] = '0';
                break;
            default:
                modified[i] = arg[i];
                break;
        }
    }

    modified[length] = '\0'; //places a null terminator at the end of the string

    return modified;
}

/*
./no-vowels
Error - Invalid number of arguments!
*/

/*
./no-vowels hello!           
h3ll0!
*/