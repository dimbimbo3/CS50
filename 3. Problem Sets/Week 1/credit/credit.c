//#include <cs50.h>
#include <stdio.h>
#include <stdlib.h> //atoi
#include <stdbool.h> //bool data type
#include <string.h> //strlen
#include <ctype.h> //isDigit

void flushInput();
bool digitCheck(char[]);
void validityCheck(char[]);
int charToInt(char num);
int sumOfDigits(int);

const int maximum = 16; //16 is the maximum number of digits for a credit card

int main(void) 
{
    char cardNum[maximum+1]; //+1 for null terminator

    //verifies that the user has entered only numeric characters
    do{
        printf("Number:");
        scanf("%16s", &cardNum);
    }
    while(!digitCheck(cardNum));

    //call to check the validity of the entered number
    validityCheck(cardNum);
}

/**
 * Clears the current input buffer in the case of an invalid result.
 * (Useful for input validation when using scanf.)
*/
void flushInput(){
    int c;

    //iterates through each character in buffer until its end is reached
    do {
        c = getchar(); //consume character
    } while( c != '\n' && c != EOF );
}

/**
 * Ensures that each character in the given array is a digit
*/
bool digitCheck(char num[]){

    for(int i = 0; i < strlen(num); i++){
        if(!isdigit(num[i])){
            flushInput();
            return false;
        }
    }

    return true;
}

/**
 * Checks whether a valid card number has been provided
*/
void validityCheck(char num[]){
    //if the length is less than 13 or equal to 14 then its already known to be invalid
    if(strlen(num) < 13 || strlen(num) == 14){
        printf("INVALID\n");
        return;
    }

    int total = 0;

    //iterates through every other digit starting from the 2nd to last
    for(int i = (strlen(num) - 2); i >= 0; i += -2){
        //converts the given numeric char to an int value
        int digit = charToInt(num[i]);
        //mutiples the given digit by 2
        int multiplied = digit * 2;
        //calculates the running total of each multipled value's digits
        total += sumOfDigits(multiplied);
        //printf("multiplied value=%i", multiplied);
        //printf(" running total=%i\n", total);
    }

    //iterates through every other digit starting from the last
    for(int i = (strlen(num) - 1); i >= 0; i += -2){
        //converts the given numeric char to an int value
        int digit = charToInt(num[i]);
        //calculates the running total of each non-multiplied digit
        total += digit;
        //printf("value=%i", digit);
        //printf(" running total=%i\n", total);
    }

    //if the value is a valid card number (last digit of the total is 0)
    //then determine the brand by the length and/or starting numbers
    if(total % 10 == 0){
        switch (strlen(num))
        {
        case 13:
            printf("VISA\n");
            break;
        case 15:
            printf("AMEX\n");
            break;
        case 16:
            if(charToInt(num[0]) == 4){
                printf("VISA\n");
            }
            else{
                printf("MASTERCARD\n");
            }
            break;
        default:
            printf("An unexpected error has occured.\n");
            break;
        }
    }
    else{
        printf("INVALID\n");
    }
}

/**
 * Converts a given numeric character (0-9) to an integer value for mathmatic operations
*/
int charToInt(char num){
    return (int) num - '0';
}

/**
 * Calculates the sum of each digit in a number
*/
int sumOfDigits(int value){
    int sum = 0;

    while(value > 0){
        int digit = value % 10;
        sum += digit;
        value /= 10;
    }

    return sum;
}

/*
Number:4003-6000-0000-0014
Number:foo
Number:4003600000000014
VISA
*/

/*
Number:4417123456789113    
VISA
*/