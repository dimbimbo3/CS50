// Practice writing a function to find a max value

//#include <cs50.h>
#include <stdio.h>

int get_num(char[]);
int max(int array[], int n);

int main(void)
{
    int n = get_num("Number of elements: ");

    int arr[n];

    //loops through the given number of elements to set their values
    for (int i = 0; i < n; i++)
    {
        printf("[%i] ", i);
        arr[i] = get_num("Element: ");
    }

    printf("The max value is %i.\n", max(arr, n));
}

/**
 * Accepts a repeatable prompt and returns a number inputted by the user.
*/
int get_num(char prompt[]){
    int num; //stores user input

    //loops until user inputs a valid number
    do
    {
        printf("%s", prompt);
        //if invalid then set n to -1 so the do-while loop continues as intended
        if(scanf("%i", &num) != 1){
            num = -1;
        }

        //clears remaining characters (if any)
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    } 
    while (num < 1);

    return num;
}

/**
 * Returns the max value of the given array.
*/
int max(int array[], int n)
{
    int max = array[0]; //initially set the max to the first value

    //loops through every element following the first
    for(int i = 1; i < n; i++){
        //if the value at this index is greater than the current maximum
        //then make it the new maximum value
        if(array[i] > max){
            max = array[i];
        }
    }

    return max;
}

/*
Number of elements: 6
[0] Element: 25
[1] Element: 3
[2] Element: 72
[3] Element: 126
[4] Element: 15
[5] Element: 66
The max value is 126.
*/