// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

//#include <cs50.h>
#include <stdio.h>
#include <stdbool.h> //bool data type
#include <string.h> //strlen
#include <ctype.h> //isupper, islower, isdigit, ispunct

#define MAX 128 //limits passwords to 128 characters
#define width_(token) #token //stringizes the token on this 2nd level macro
#define width(x) width_(x) //wrapper macro to pass x value to width_

bool valid(char[]);

int main(void)
{
    char password[MAX+1]; //+1 for null terminator

    bool endLoop = false;
    //Loops until a valid password is given
    do{
        printf("Enter your password: ");
        scanf("%" width(MAX) "s", password);

        if (valid(password))
        {
            printf("Your password is valid!\n");
            endLoop = true; //sets flag to true so loop will end
        }
        else
        {
            printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
        }
    }
    while(!endLoop);
}

/**
 * Validates that the given char array contains at least one uppercase, lowercase, number, and symbol.
*/
bool valid(char password[])
{
    bool upper, lower, num, symbol = false; //initialize all flags to false
    int length = strlen(password);

    //iterates through each character in the password but stops if all flags have already been set to true
    for(int i = 0; i < length && (!upper || !lower || !num || !symbol); i++){
        if(!upper && isupper(password[i]))
            upper = true;
        else if(!lower && islower(password[i]))
            lower = true;
        else if(!num && isdigit(password[i]))
            num = true;
        else if(!symbol && ispunct(password[i]))
            symbol = true;
    }

    //if all of the flags have been set to true then the password is valid
    if(upper && lower && num && symbol)
        return true;
    else
        return false;
}

/*
Enter your password: test
Your password needs at least one uppercase letter, lowercase letter, number and symbol
Enter your password: t3st!
Your password needs at least one uppercase letter, lowercase letter, number and symbol
Enter your password: t3St!  
Your password is valid!
*/