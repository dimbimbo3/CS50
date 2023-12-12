//#include <cs50.h>
#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //strlen
#include <ctype.h> //isupper, islower
#include <stdbool.h> //bool

#define BUFFER 1024 //chosen character limit (including null terminator)
#define A 65 //ascii value for uppercase A
#define a 97 //ascii value for lowercase a

char* encrypt_text(char[], char[]); //function prototype

int main(int argc, char* argv[]) 
{
    char input[BUFFER];
    char* cipherPtr;

    //checks that only ./substitution and the key were entered
    if(argc == 2){
        //checks that the argument is 26 characters in order to act as a valid key
        if(strlen(argv[1]) == 26){
            printf("plaintext: ");
            if(fgets(input, BUFFER, stdin) != NULL){
                cipherPtr = encrypt_text(input, argv[1]);
                printf("ciphertext: %s", cipherPtr);
                free(cipherPtr);
            }

            return 0; //indicates success
        }
        else{
            printf("Key must contain 26 characters.\n");

            return 1; //indicates error
        }
    }
    else{
        printf("Usage: ./substitution key\n");

        return 1; //indicates error
    }
}

/**
 * Converts given plaintext to encrypted ciphertext by substituting each letter to key character which corresponds to its position in the alphabet
*/
char* encrypt_text(char plain[], char key[]){
    int letterPos;
    int length = strlen(plain); //length of the string
    char* cipher = malloc(sizeof(char) * length+1); //allocate memory based on the size of the plain text (+1 for null terminator)

    //iterates through each character in the string
    for(int i = 0; i < length; i++){
        //checks if the character is uppercase
        if(isupper(plain[i])){
            letterPos = (int) plain[i] - A; //subtracts char ascii value by that of 'A' to get its place in the alphabet
            cipher [i] = toupper(key[letterPos]);
        }
        //checks if the character is lowercase
        else if(islower(plain[i])){
            letterPos = (int) plain[i] - a; //subtracts char ascii value by that of 'a' to get its place in the alphabet
            cipher[i] = tolower(key[letterPos]);
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
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\substitution> ./substitution      
Usage: ./substitution key
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\substitution> ./substitution ABC  
Key must contain 26 characters.
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\substitution> ./substitution 1 2 3
Usage: ./substitution key
*/

/*
PS C:\Users\Dan\CS50\3. Problem Sets\Week 2\substitution> ./substitution VCHPRZGJNTLSKFBDQWAXEUYMOI
plaintext:  hello, world
ciphertext: jrssb, ybwsp
*/