//#include <cs50.h>
#include <stdio.h>
#include <stdbool.h> // Boolean data type

bool prime(int number);

int main(void)
{
    int min;
    do
    {
        printf("Minimum: ");
        scanf("%i", &min);
    }
    while (min < 1);

    int max;
    do
    {
        printf("Maximum: ");
        scanf("%i", &max);
    } 
    while (min >= max);
    
    printf("Prime Numbers:\n");

    for (int i = min; i <= max; i++)
    {
        if (prime(i))
        {
            printf("%i\n", i);
        }
    }
}

//Determines if the given number is prime or not
bool prime(int number)
{
    bool prime = true;

    //if the number is 1 then its not prime
    if(number == 1){
        return false;
    }
    //if the number is 2 or 3 then its prime
    else if(number == 2 || number == 3){
        return true;
    }
    //else divide it by every number from 2 to n and ensure there is a remainder
    else{
        for(int i = 2; i < number && prime; i++){
            if(number % i == 0){
                return false;
            }
        }
    }

    //if the number was determined to be prime
    //then return as true
    return true;
}

/*
Minimum: 1
Maximum: 100
Prime Numbers:
2
3
5
7
11
13
17
19
23
29
31
37
41
43
47
53
59
61
67
71
73
79
83
89
97
*/