//#include <cs50.h>
#include <stdio.h>
#include <stdlib.h> //atoi, malloc
#include <string.h> //strlen
#include <ctype.h> //isupper, islower
#include <stdbool.h> //bool

#define BUFFER 1024 //chosen character limit (including null terminator)
#define A 65 //ascii value for uppercase A
#define a 97 //ascii value for lowercase a

bool only_digits(char[]); //function prototype - start
char* encrypt_text(char[], int); //function prototype - end

//MAIN - START
int main(int argc, char* argv[]) 
{
    int key;
    char input[BUFFER];
    char* cipherPtr;

    //checks that only ./caesar and one other argument were entered
    if(argc == 2 && only_digits(argv[1])){
        key = atoi(argv[1]); //converts the string to an integer

        printf("plaintext: ");
        if(fgets(input, BUFFER, stdin) != NULL){
            cipherPtr = encrypt_text(input, key);
            printf("ciphertext: %s", cipherPtr);
            free(cipherPtr);
        }

        return 0; //indicates success
    }
    else{
        printf("Usage: ./caesar key\n");

        return 1; //indicates error
    }
}//MAIN - END

/**
 * Returns a boolean value indiciating whether the given string is composed only of numbers.
*/
bool only_digits(char arg1[]){
    bool digits = true; //initialize flag to true

    //checks if each character in the string is a digit
    //if not then sets the flag to false and ends the loop
    for(int i = 0; i < strlen(arg1) && digits != false; i++){
        if(!isdigit(arg1[i])){
            digits = false;
        }
    }

    return digits;
}

/**
 * Converts given plaintext to encrypted ciphertext by incrementing each ascii character value by the key
*/
char* encrypt_text(char plain[], int cKey){
    int letterPos;
    int length = strlen(plain); //length of the string
    char* cipher = malloc(sizeof(char) * length+1); //allocate memory based on the size of the plain text (+1 for null terminator)

    //iterates through each character in the string
    for(int i = 0; i < length; i++){
        //checks if the character is uppercase
        if(isupper(plain[i])){
            //subtracts char ascii value by that of 'A' to get its place in the alphabet
            //as well as adds the cipher key to it and gets its remainder from 26 to find its new place in the alphabet
            letterPos = ((int) plain[i] - A + cKey) % 26;
            cipher [i] = (char) (letterPos + A); //adds A back to the new letter to get it's ascii value, then casts to char & stores
        }
        //checks if the character is lowercase
        else if(islower(plain[i])){
            //subtracts char ascii value by that of 'a' to get its place in the alphabet
            //as well as adds the cipher key to it and gets its remainder from 26 to find its new place in the alphabet
            letterPos = ((int) plain[i] - a + cKey) % 26;
            cipher[i] = (char) (letterPos + a); //adds a back to the new letter to get it's ascii value, then casts to char & stores
        }
        //else the character is a punctuation mark, a symbol, or a number so keep it the same
        else{
            cipher[i] = plain[i];
        }
    }

    cipher[length] = '\0'; //adds null terminator to the end of the cipher string for printf

    return cipher;
}

/*
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\caesar> ./caesar
Usage: ./caesar key
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\caesar> ./caesar HELLO
Usage: ./caesar key
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\caesar> ./caesar 1 2 3 
Usage: ./caesar key
*/

/*
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\caesar> ./caesar 13   
plaintext: be sure to drink your Ovaltine
ciphertext: or fher gb qevax lbhe Binygvar
*/